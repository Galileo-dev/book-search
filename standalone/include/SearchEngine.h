#pragma once

#include <booksearch/InvertedIndex.h>
#include <booksearch/Trie.h>

#include <string>

class SearchEngine {
public:
  SearchEngine();
  void addDocuments(const std::string& documentPath);
  void search(const std::string& key);
  void debug();
  InvertedIndex& getInvertedIndex();
  Trie& getTrie();

private:
  InvertedIndex invertedIndex;
  Trie autoComplete;
  void updateAutoComplete();
  Vector<std::string> tokenize(const std::string& query);
  template <typename T> Vector<T> intersectOp(const Vector<T>& vec1, const Vector<T>& vec2) {
    Vector<T> result;
    for (size_t i = 0; i < vec1.size(); ++i) {
      for (size_t j = 0; j < vec2.size(); ++j) {
        if (vec1[i] == vec2[j]) {
          result.push_back(vec1[i]);
          break;
        }
      }
    }
    return result;
  }

  Vector<SearchResult> intersectOp(const Vector<SearchResult>& vec1,
                                   const Vector<SearchResult>& vec2);
  Vector<SearchResult> unionOp(const Vector<SearchResult>& vec1, const Vector<SearchResult>& vec2);
  Vector<SearchResult> subtractOp(const Vector<SearchResult>& vec1,
                                  const Vector<SearchResult>& vec2);
  Vector<SearchResult> getAllDocuments();
};