#include "movie.h"

Movie::Movie(const std::string category, const std::string name, 
	double price, int qty, std::string genre, std::string rating): 
    Product(category, name, price, qty) {
	this->genre_ = genre;
	this->rating_ = rating;
}

Movie::~Movie(){ }

std::set<std::string> Movie::keywords() const{
    std::set<std::string> setName = parseStringToWords(name_);
    std::set<std::string> setGenre = parseStringToWords(genre_);

    return setUnion(setName, setGenre);
}

std::string Movie::displayString() const{
	std::string price = std::to_string(price_);
	price.erase ( price.find_last_not_of('0') + 1, std::string::npos ); 
	std::string qty = std::to_string(qty_);
	//std::string rating = std::to_string(getAverageRating());
	//rating.erase ( rating.find_last_not_of('0') + 1, std::string::npos);
	
	std::string str = name_ + "\nGenre: " + genre_ + " Rating: " + rating_ + 
	  "\n" + price + ' ' + qty + " left.\n";
	return str;
}

void Movie::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ 
	  << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << std::endl;
}