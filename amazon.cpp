#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
#include "msort.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};

struct ReviewRatingSorter
{
    bool operator()(Product* p1, Product* p2) {
        return (p1->getAverageRating() > p2->getAverageRating());
    }    
};

void displayProducts(vector<Product*>& hits, char sortFlag);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    string loginUser = "";
    //TODO
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                char sortFlag;
                ss >> sortFlag;
                if ((sortFlag != 'r') && (sortFlag != 'n')){
                    continue;
                }
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits, sortFlag);
            }
            else if ( cmd == "OR" ) {
                char sortFlag;
                ss >> sortFlag;
                if ((sortFlag != 'r') && (sortFlag != 'n')){
                    continue;
                }
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits, sortFlag);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else if (cmd == "ADD"){
                unsigned hits_result_index = 0;
                ss >> hits_result_index;
                if (hits_result_index > hits.size() || hits_result_index < 1){
                    cout << "Invalid request" << endl;
                }
                else{
                    ds.addCart(loginUser, hits[hits_result_index-1]); 
                }       
            }
            else if (cmd == "VIEWCART"){
                if (loginUser != "") ds.viewCart(loginUser);
                else cout << "No current user" << endl;
            }
            else if (cmd == "BUYCART"){
                if (loginUser != "") ds.buyCart(loginUser);
                else cout << "No current user" << endl;
            }
            else if (cmd == "LOGIN"){
                string username;
                ss >> username;
                if (ds.validUser(username)) loginUser = username;
            }
            else if (cmd == "LOGOUT"){
                loginUser = "";
            }
            else if (cmd == "ADDREV"){
                unsigned int search_hit_number, rating;
                string date, review_text = "";
                ss >> search_hit_number >> rating >> date >> review_text; // no review_text is ok
                if (search_hit_number > hits.size() || search_hit_number < 1){
                    cout << "Invalid search_hit_number" << endl;
                }
                else if (loginUser == "") {
                    cout << "No current user" << endl;
                }
                else if (rating < 1 || rating > 5){
                    cout << "Invalid rating" << endl;
                } 
                else{
                    if (date.length() == 10 && date[4] == '-' && date[7] == '-'){
                        stringstream ss2(date);
                        int year = 0;
                        int month, dd = 0;
                        ss2 << year << month << dd;
                        month = 0 - month;
                        dd = 0 - dd;
                        review_text = trim(review_text);
                        switch(month){
                            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                                if (dd >= 1 && dd <= 31 ){
                                    ds.addReview(hits[search_hit_number]->getName(), rating, loginUser, 
                                        date, review_text);
                                }
                                break;
                            case 4: case 6: case 9: case 11:
                                if (dd >= 1 && dd <= 30){
                                    ds.addReview(hits[search_hit_number]->getName(), rating, loginUser, 
                                        date, review_text);
                                }
                                break;
                            case 2:
                                if (dd >= 1 && dd <= 30){
                                    ds.addReview(hits[search_hit_number]->getName(), rating, loginUser, 
                                        date, review_text);
                                }
                                break;
                            default:
                                cout << "Wrong format" << endl;
                        }
                    }      
                }
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits, char sortFlag)
{
    int resultNo = 1;
    if (sortFlag == 'n'){
        ProdNameSorter ps;
        mergeSort(hits, ps);
    }
    else{
        ReviewRatingSorter rs;
        mergeSort(hits, rs);
    }
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
