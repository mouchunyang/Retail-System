#include "mydatastore.h"

MyDataStore::MyDataStore(){ }

MyDataStore::~MyDataStore(){
    for (unsigned int i = 0; i < productList.size(); i++){
        delete productList[i];
    }
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
          it != userList.end(); ++it){
        delete it->first;
    }
}

void MyDataStore::addProduct(Product *p){
    productList.push_back(p);
}

void MyDataStore::addUser(User *u){
    std::vector<Product*> cart;
    userList.insert(std::pair<User*, std::vector<Product*> >(u, cart));
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, 
    int type){
    std::vector<std::set<std::string> >keywords;
    for (unsigned int i = 0; i < productList.size(); i++){
        keywords.push_back(productList[i]->keywords());
    }

    std::set<int> currentResultIdx;
    //find a single term in all the keywords
    for (unsigned int i = 0; i < terms.size(); i++){
        std::string term = terms[i];
        std::set<int> productIdx;
        for (unsigned int j = 0; j < keywords.size(); j++){
            if (keywords[j].find(term) != keywords[j].end()){ //found -> match 
                //found the keyword
                productIdx.insert(j);
            }
            else{
                //not found -> do nothing
            }
        }
        if (type == 0){  ///and search 
            if (i == 0){
                //avoid anding with an empty set everytime
                currentResultIdx = productIdx; 
            }
            else{
                currentResultIdx = setIntersection(currentResultIdx, productIdx);
            }
        }
        else { //or search
            currentResultIdx = setUnion(currentResultIdx, productIdx);
        }  
    }

    std::vector<Product*> v;
    for (std::set<int>::iterator it = currentResultIdx.begin();
        it!= currentResultIdx.end(); ++it){
        v.push_back(productList[*it]);
    }
    return v;
}

void MyDataStore::dump(std::ostream& ofile){
    ofile << "<products>" << std::endl;
    for (unsigned int i = 0; i < productList.size(); i++){
        productList[i]->dump(ofile);
    }
    ofile << "</products>" << std::endl;
    ofile << "<users>" << std::endl;
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
        it != userList.end(); ++it){
        it->first->dump(ofile);
    }
    ofile << "</users>" << std::endl;
    ofile << "<reviews>" << std::endl;
    for (std::map<std::string, std::vector<Review> >::iterator it = reviewList.begin();
        it != reviewList.end(); ++it){
        for (unsigned int i = 0; i < it->second.size(); i++){
            ofile << it->first << std::endl;
            Review myrev = it->second[i];
            ofile << myrev.rating << " " << myrev.username << " " << myrev.date 
              << " " << myrev.reviewText << std::endl;  
        }
        
    }
    ofile << "</reviews>" << std::endl;

}

void MyDataStore::addCart(std::string username, Product* p){
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
        it != userList.end(); ++it){
        if (it->first->getName() == username){
            it->second.push_back(p);
            return;
        }
    }
    std::cout << "Invalid username" << std::endl;
    return;
}

void MyDataStore::viewCart(std::string username){
   for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
        it != userList.end(); ++it){
        if (it->first->getName() == username){
            for (unsigned int i = 0; i < it->second.size(); i++){
                std::cout << "Item " << i+1 << std::endl;
                std::cout << it->second[i]->displayString() << std::endl;  
            }
            return;         
        }
    }
    std::cout << "Invalid username" << std::endl;
}

void MyDataStore::buyCart(std::string username){
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
        it != userList.end(); ++it){
        if (it->first->getName() == username){
            std::vector<Product*> newCart; // the Cart for the unpurchased products
            for (unsigned int i = 0; i < it->second.size(); i++){
                Product *p = it->second[i];
                if (it->first->getBalance() > p->getPrice() && p->getQty() > 0){ 
                    it->first->deductAmount(p->getPrice());
                    p->subtractQty(1);
                }
                else{
                    newCart.push_back(it->second[i]);
                }
            }
            it->second = newCart;
            return;         
        }
    }
    std::cout << "Invalid username" << std::endl;
}

void MyDataStore::addReview(const std::string& prodName,
               int rating,
               const std::string& username,
               const std::string& date,
               const std::string& review_text){
    for (unsigned int i = 0; i < productList.size(); i++){ //in the productList
        if (productList[i]->getName() == prodName){
            for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
                it != userList.end(); ++it){
                if (it->first->getName() == username){ // in the userlist
                    productList[i]->addRating(rating); //add rating to the product
                    Review myReview = Review(rating, username, date, review_text);
                    std::map<std::string, std::vector<Review> >::iterator it2 = 
                        reviewList.find(prodName);
                    if (it2 == reviewList.end()) { //not yet in the map
                        std::vector<Review> myvec;
                        myvec.push_back(myReview);
                        reviewList.insert(std::pair<std::string, 
                            std::vector<Review> >(prodName, myvec));
                    }
                    else{
                        it2->second.push_back(myReview);
                    }
                    return;  
                }
            }
        }
    }
    
}

bool MyDataStore::validUser(std::string username, std::string password) const{
    for (std::map<User*, std::vector<Product*> >::const_iterator it = userList.begin();
        it != userList.end(); it++){
        if (it->first->getName() == username && it->first->getHashcode() == convertToHashCode(password)){
            return true;
        }
    }
    return false;
}

std::vector<Review> MyDataStore::viewRev(std::string prodName) const{
   std::map<std::string, std::vector<Review> >::const_iterator it = reviewList.find(prodName);
   if (it != reviewList.end()){
    return it->second;
   }
   else{
    std::vector<Review> empty;
    return empty;
   }
}

double MyDataStore::basicSimilarity(std::string username1, std::string username2){
    if (username1 == username2){
        return 0.0;
    }
    std::vector<double> ratingDifference;
    for(std::map<std::string, std::vector<Review> >::iterator it = reviewList.begin();
        it != reviewList.end(); ++it){
        std::vector<Review> prodReviews = it->second;
        int index1 = -1;
        int index2 = -1;
        for (unsigned int i = 0; i < prodReviews.size(); i++){
            if (prodReviews[i].username == username1){
                index1 = i;
            }
            else if (prodReviews[i].username == username2){
                index2 = i;
            }
        }
        if (index1 >= 0 && index2 >= 0){ //both users have rated a product
            ratingDifference.push_back(abs(prodReviews[index1].rating - 
                prodReviews[index2].rating)/4.0);
        }
    }
    if (!ratingDifference.empty()){
        double sum_of_elements = 0.0;
        for(unsigned int i = 0; i < ratingDifference.size(); i++){
            sum_of_elements += ratingDifference[i];
        }
        double similarity = sum_of_elements/ratingDifference.size();
        //std::cout << sum_of_elements << ' ' << ratingDifference.size() << '=' << similarity << std::endl;
        return similarity;
    }
    else{
        return 1.0;
    }
}

std::map<std::string, double> MyDataStore::refineSimilarity(std::string username){
    std::map<std::string, double> refinedSimilarities;
    Heap<std::string> PQ;
    PQ.push(0.0, username);
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin(); 
        it != userList.end(); ++it){
        if (it->first->getName() != username){
            PQ.push(1.0, it->first->getName());
        }
    }
    while (!PQ.empty()){
        std::string curr = PQ.top();
        double curr_p = PQ.getPriority(curr);
        refinedSimilarities.insert(std::pair<std::string, double> (curr, curr_p));
        PQ.pop();
        for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin(); 
            it != userList.end(); ++it){
            double weight = basicSimilarity(curr, it->first->getName());
            if (weight < 1.0 && it->first->getName() != curr){ //neighbor
                double priority;
                try{
                    priority = PQ.getPriority(it->first->getName());    
                }
                catch(std::exception){
                    //jump if this node is not in PQ
                    continue;
                }
                if (curr_p+weight < priority){
                    PQ.decreaseKey(curr_p+weight, it->first->getName());
                }
            }
        }
    }
    return refinedSimilarities;
}

std::vector<std::pair<std::string, double> > MyDataStore::makeSuggestion(std::string currentUser){
    std::vector<std::pair<std::string, double> > suggestions;
    std::map<std::string, double> similaritiesMap= refineSimilarity(currentUser);
    for (std::map<std::string, std::vector<Review> >::iterator it = reviewList.begin();
        it != reviewList.end(); ++it){ 
        //process a single products's review
        std::vector<Review> reviewVec = it->second;
        bool thisProductReviewed = false;
        for (unsigned int i = 0; i < reviewVec.size(); ++i){
            if (reviewVec[i].username == currentUser){
                thisProductReviewed = true;
                break;
            }
        }
        if (!thisProductReviewed){
            double RP = 0.0;
            double W = 0.0;
            for (unsigned int i = 0; i < reviewVec.size(); ++i){
                int r = reviewVec[i].rating;
                double s = similaritiesMap[reviewVec[i].username];
                RP += r*(1 - s);
                W += (1 - s);
            }
            double rating = RP / W;
            if (W == 0.0){
                rating = 0;
            }
            //debug
            //std::cout << it->first << RP << ' ' << W << std::endl; 

            suggestions.push_back(std::pair<std::string, double>(it->first, rating));
        }
    }
    comparebyRating pc;
    std::stable_sort(suggestions.begin(), suggestions.end(), pc);
    return suggestions;
}




