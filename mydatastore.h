#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "util.h"
#include "review.h"
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
    
    bool validUser(std::string username) const;

private:
	std::vector<Product*> productList;
	std::map<User*, std::vector<Product*> > userList;
    std::map<std::string, std::vector<Review> > reviewList;

};







#endif