#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>

#include "stopwords.h"

class TextCleaner {
public:
  TextCleaner() {}

  std::string clean_word(const std::string& word) const {
    std::string clean;
    for (char ch : word) {
      if (std::isalnum(ch)) {
        clean += std::tolower(ch);  // Convert to lowercase and remove punctuation
      }
    }
    return clean;
  }

  bool is_stop_word(const std::string& word) const {
    return stop_words.find(word) != stop_words.end();
  }

private:
};