#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "util.h"
#include "heap.h"
#include "review.h"
#include "hash.h"
#include <map>

class MyDataStore : public DataStore{
public:
	MyDataStore();
	~MyDataStore();

	/**
     * Adds a product to the data store
     */
	void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    void addCart(std::string username, Product* p);

    void viewCart(std::string username);

    void buyCart(std::string username);

    void addReview(const std::string& prodName,
               int rating,
               const std::string& username,
               const std::string& date,
               const std::string& review_text);
    
    bool validUser(std::string username, std::string password) const;

    std::vector<Review> viewRev(std::string prodName) const;
    
    std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);

    double basicSimilarity(std::string username1, std::string username2);

    //return a vector that contains pairs of ratings and usernames
    std::map<std::string, double> refineSimilarity(std::string username);
private:
	std::vector<Product*> productList;
	std::map<User*, std::vector<Product*> > userList;
    std::map<std::string, std::vector<Review> > reviewList;
    

    struct comparebyRating{
        bool operator()(std::pair<std::string, double> p1, std::pair<std::string, double> p2){
            if (p1.second > p2.second){
                return true;
            }
            else if (p1.second == p2.second){
                if (p1.first < p2.first){
                    return true;
                }
            }
            return false;
        }
    };
};







#endif