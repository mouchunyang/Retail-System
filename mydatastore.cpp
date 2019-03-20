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
	for (unsigned int i = 0; i < productList.size(); i++){
		productList[i]->dump(ofile);
	}
    for (std::map<User*, std::vector<Product*> >::iterator it = userList.begin();
        it != userList.end(); ++it){
        it->first->dump(ofile);
    }
    for (std::map<std::string, std::vector<Review> >::iterator it = reviewList.begin();
        it != reviewList.end(); ++it){
        ofile << it->first << std::endl;
        for (unsigned int i = 0; i < it->second.size(); i++){
            it->second[i].dump(ofile);  
        }
        
    }

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

bool MyDataStore::validUser(std::string username) const{
    for (std::map<User*, std::vector<Product*> >::const_iterator it = userList.begin();
        it != userList.end(); it++){
        if (it->first->getName() == username){
            return true;
        }
    }
    return false;
}





