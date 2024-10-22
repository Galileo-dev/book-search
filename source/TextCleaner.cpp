#include <booksearch/Stopwords.h>
#include <booksearch/TextCleaner.h>

#include <algorithm>
#include <cctype>

TextCleaner::TextCleaner() { stop_words = get_stopwords(); }

std::string TextCleaner::clean_word(const std::string& word) const {
  std::string clean;
  for (char ch : word) {
    if (std::isalnum(ch)) {
      clean += std::tolower(ch);
    }
  }
  return clean;
}

bool TextCleaner::is_stop_word(const std::string& word) const {
  return stop_words.find(word) != stop_words.end();
}
