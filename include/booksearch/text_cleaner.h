#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>

class TextCleaner {
public:
  TextCleaner() {
    // Initialize the set of stopwords
    stop_words = {"the", "is", "at", "which", "on"};
  }

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
  std::unordered_set<std::string> stop_words;  // Stop words set
};