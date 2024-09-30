#include "hashtable.h"
#include <iostream>
#include <sstream>
#include <string>
class InvertedIndex {
public:
  InvertedIndex() : count(0), docs(), index() {}

  void add_document(std::string name, std::string content) {
    count++;
    docs.set(count, std::make_unique<std::string>(name));

    // iterate by whitespace
    std::istringstream iss(content);
    std::string word;
    while (iss >> word) {
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      std::cout << word << std::endl;
    }
  }

private:
  int count;
  HashMap<int, std::string> docs;
  HashMap<int, HashMap<int, int>> index;
};