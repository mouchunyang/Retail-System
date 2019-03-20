#include "msort.h"
#include <iostream>

using namespace std;



int main(){
    Comp1<int> c1;
    vector<int> v;
    v.push_back(38);
    v.push_back(27);
    v.push_back(43);
    v.push_back(3);
    v.push_back(9);
    v.push_back(82);
    v.push_back(10);
    print(v);
    cout << "------------------" << endl;
    mergeSort(v, c1);
    print(v);
}