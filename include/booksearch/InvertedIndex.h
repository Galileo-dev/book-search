#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <algorithm>

#include <booksearch/HashTable.h>
#include <booksearch/TextCleaner.h>
#include <booksearch/Vector.h>

struct Doc {
  std::string name;
  std::string path;

  Doc();
  Doc(const std::string &name, const std::string &path);

  template <class Archive> void serialize(Archive &archive){
    archive(name, path);
  }
};

struct SearchResult {
  Doc doc;
  int freq;

  SearchResult();
  SearchResult(const Doc &doc, const int &freq);
};

class InvertedIndex {
public:
  InvertedIndex(const TextCleaner &cleaner);

  int add_document(std::string name, std::istream &stream);
  int add_document_from_file(std::string name, std::string path);
  Vector<SearchResult> search(std::string key);
  Vector<std::string> keys();

  template <class Archive> void serialize(Archive &archive){
    archive(count, index, docs);
  };

private:
  int count;
  HashTable<int, Doc> docs;
  HashTable<std::string, HashTable<int, int>> index;
  const TextCleaner &cleaner;
};
