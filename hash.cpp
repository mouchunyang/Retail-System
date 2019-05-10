#include "hash.h"
#include <stdexcept>

unsigned long long convertToHashCode(std::string str){
    if (str.length() > 8) throw std::length_error("");
    unsigned long long int temp = hash_Helper(str, str.length()-1); 
    unsigned int w4 = temp%65521;
    temp = temp/65521;
    unsigned int w3 = temp%65521;
    temp = temp/65521;
    unsigned int w2 = temp%65521;
    temp = temp/65521;
    unsigned int w1 = temp%65521;
    return (45912 * w1 + 35511 * w2 + 65169 * w3 + 4625 * w4) % 65521;
}

unsigned long long hash_Helper(std::string str, int index){
    if (index > 0){
        return ((unsigned int)str[index]) + 128*hash_Helper(str, index-1);
    }
    else{
        return (unsigned int)str[index];
    }
}