#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

string convToLower(string src);
string &ltrim(string &s);

struct Comp2 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    return (a > b);
  }
};
struct Comp3 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    return (convToLower(a) < convToLower(b));
  }
};
struct Comp4 {
  bool operator()(std::string a, std::string b)
  {
    // Complete the code
    return (ltrim(a) < ltrim(b));
  }
};

void outputWords(std::ostream& ostr, const std::vector<std::string>& words)
{
  for(unsigned i = 0; i < words.size()-1; i++){
    ostr << words[i] << ":";
  }
  if(words.size() > 0){
    ostr << words.back();
  }
  ostr << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Provide an output file name." << endl;
    return 1;
  }
  ofstream ofile(argv[1]);
  if(ofile.fail()) {
    cout << "Cannot open file for writing." << endl;
    return 1;
  }
  vector<string> mywords =
    { "abc", "Acc", "aBc", "Zxy", "zyx", "  efg" };

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z).  This should still be case sensitive.
  vector<string> order1 = mywords;
  std::sort(order1.begin(), order1.end());
  outputWords(ofile, order1);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order but descending from largest to smallest
  // (i.e. z to a).  This should still be case sensitive
  vector<string> order2 = mywords;
  Comp2 c2;
  std::sort(order2.begin(), order2.end(), c2);
  outputWords(ofile, order2);


  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z) but case INSENSITIVE (i.e. abc < Abd)
  vector<string> order3 = mywords;
  Comp3 c3;
  std::sort(order3.begin(), order3.end(), c3);
  outputWords(ofile, order3);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z), case sensitive and ignoring
  // spaces (only ' ') before the word.
  vector<string> order4 = mywords;
  Comp4 c4;
  std::sort(order4.begin(), order4.end(), c4);
  outputWords(ofile, order4);

  ofile.close();
  return 0;
}

//helper function
string convToLower(string src){
    transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

string &ltrim(string &s) {
    s.erase(s.begin(), 
	    find_if(s.begin(), 
			 s.end(), 
			 not1(ptr_fun<int, int>(isspace))));
    return s;
}

