#ifndef HASH_H
#define HASH_H

#include <string>

unsigned long long convertToHashCode(std::string str);
unsigned long long hash_Helper(std::string str, int index);

#endif