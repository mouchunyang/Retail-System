#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>


/** Complete the setIntersection and setUnion functions below
 *  in this header file (since they are templates).
 *  Both functions should run in time O(n*log(n)) and not O(n^2)
 */
template <typename T>
std::set<T> setIntersection(std::set<T>& s1, std::set<T>& s2)
{
	std::set<T> intersection;
    for (typename std::set<T>::iterator it = s1.begin();
    	it != s1.end(); ++it){
        T data = *it;
        if (s2.find(data)!=s2.end()){ //s1's data found in s2
            intersection.insert(data);
        }
    }
    return intersection;
}

template <typename T>
std::set<T> setUnion(std::set<T>& s1, std::set<T>& s2)
{
    std::set<T> unionSet;
    for (typename std::set<T>::iterator it = s1.begin();
    	it != s1.end(); ++it){
    	unionSet.insert(*it);
    }
    for (typename std::set<T>::iterator it = s2.begin();
    	it != s2.end(); ++it){
    	if (s1.find(*it) == s1.end()){ //not found in s1
            unionSet.insert(*it);    
    	}    	
    }
    return unionSet;
}

/***********************************************/
/* Prototypes of functions defined in util.cpp */
/***********************************************/

std::string convToLower(std::string src);

std::set<std::string> parseStringToWords(std::string line);

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// Removes any leading whitespace
std::string &ltrim(std::string &s) ;

// Removes any trailing whitespace
std::string &rtrim(std::string &s) ;

// Removes leading and trailing whitespace
std::string &trim(std::string &s) ;
#endif
