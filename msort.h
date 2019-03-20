#include <vector>
#include <iostream>
//debug
/*
void print(std::vector<int> v)
{
  for(unsigned int i = 0; i < v.size(); i++) std::cout << v[i] << " ";
  std::cout << std::endl;
}
*/

template <class T, class Comparator>
void merge(std::vector<T> myArray, std::vector<T> &other, int s1, int e1, 
	int s2, int e2, Comparator comp){
	//debug
	//std::cout << s1 << e1 << s2 << e2 << std::endl;
    int start = s1;
    int length1 = e1 - s1;
    int length2 = e2 - s2;
    std::vector<T> temp;
    for (int i = 0; i < (length1 + length2); i++){
        if (s1 >= e1){
        	temp.push_back(other[s2]);
        	s2++;
        }
        else if (s2 >= e2){
        	temp.push_back(other[s1]);
        	s1++;
        }
        else if (comp(other[s1], other[s2])) { // a < b
            temp.push_back(other[s1]);
            s1++;
        }
        else{
        	temp.push_back(other[s2]);
        	s2++;
        }
    }
    for (int i = 0; i < (length1 + length2); i++){
    	other[i+start] = temp[i];
    }
    //debug
    //std::cout << s1 << e2 << s2 << e2 << std::endl;
    //print(other);
    //std::cout << "----------" << std::endl;
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp){
    std::vector<T> other(myArray);
    msort(myArray, other, 0, myArray.size(), comp);
    myArray = other;
}

template <class T, class Comparator>
void msort(std::vector<T>& myArray, std::vector<T>& other, int start, int end, Comparator comp){
	//debug
	//std::cout << "start" << start << " end" << end << std::endl;
	if ((end - start) > 1){  //list's size > 1
		int mid = (start+end)/2;
        msort(myArray, other, start, mid, comp);
        msort(myArray, other, mid, end, comp);
        merge(myArray, other, start, mid, mid, end, comp);
	}
}

/*
template <typename T>
struct Comp1 { //normal
  bool operator()(T a, T b)
  {
    return (a > b);
  }
};
*/



