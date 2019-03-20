#include "clothing.h"


Clothing::Clothing(const std::string category, const std::string name, 
	double price, int qty, std::string size, std::string brand): 
    Product(category, name, price, qty) {
	this->size_ = size;
	this->brand_ = brand;
}

Clothing::~Clothing(){ }

std::set<std::string> Clothing::keywords() const{
    std::set<std::string> setName = parseStringToWords(name_);
    std::set<std::string> setBrand = parseStringToWords(brand_);

    return setUnion(setName, setBrand);
}

std::string Clothing::displayString() const{
	std::string price = std::to_string(price_);
	price.erase ( price.find_last_not_of('0') + 1, std::string::npos ); 
	std::string qty = std::to_string(qty_);
	std::string rating = std::to_string(getAverageRating());
	rating.erase ( rating.find_last_not_of('0') + 1, std::string::npos);

	std::string str = name_ + "\nSize: " + size_ + " Brand: " + brand_ + 
	  "\n" + price + ' ' + qty + " left\n" + "Rating: " + rating;
	return str;
}

void Clothing::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ 
	  << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << std::endl;
}