#pragma once

#include <string>
#include <unordered_set>

class TextCleaner {
public:
  TextCleaner();

  std::string clean_word(const std::string& word) const;
  bool is_stop_word(const std::string& word) const;

private:
  std::unordered_set<std::string> stop_words;
};
