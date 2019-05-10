#include "book.h"

Book::Book(const std::string category, const std::string name, 
	double price, int qty, std::string isbn, std::string author): 
    Product(category, name, price, qty) {
	this->isbn_ = isbn;
	this->author_ = author;
}

Book::~Book(){ }

std::set<std::string> Book::keywords() const{
    std::set<std::string> setName = parseStringToWords(name_);
    std::set<std::string> setIsbn = parseStringToWords(isbn_);
    std::set<std::string> setAuthor = parseStringToWords(author_);

    std::set<std::string> temp = setUnion(setIsbn, setAuthor);
    //printset(setUnion(setName, temp));
    return setUnion(setName, temp);
}

std::string Book::displayString() const{
    
	std::string price = std::to_string(price_);
	price.erase ( price.find_last_not_of('0') + 1, std::string::npos ); 
	std::string qty = std::to_string(qty_);
	//std::string rating = std::to_string(getAverageRating());
	//rating.erase ( rating.find_last_not_of('0') + 1, std::string::npos);
	
	std::string str = name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + 
	  "\n" + price + ' ' + qty + " left.\n";
	return str;
}

void Book::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ 
	  << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << std::endl;
}

bool Book::isMatch(std::vector<std::string>& searchTerms) const{
	return false;
}
