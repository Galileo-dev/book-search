#pragma once

#include <booksearch/HashTable.h>
#include <booksearch/TextCleaner.h>
#include <booksearch/Vector.h>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>

struct Doc {
  std::string name;
  std::string path;

  Doc();
  Doc(const std::string &name, const std::string &path);

  template <class Archive> void serialize(Archive &archive) { archive(name, path); }

  bool operator==(const Doc &other) const { return name == other.name; }
};

struct SearchResult {
  Doc doc;
  int freq;

  SearchResult();
  SearchResult(const Doc &doc, const int &freq);

  bool operator==(const SearchResult &other) const { return doc == other.doc; }
};

class InvertedIndex {
public:
  InvertedIndex(const TextCleaner &cleaner);

  int addDocument(std::string name, std::istream &stream);
  int addDocument(std::string name, std::string path);
  Vector<SearchResult> search(std::string key);
  Vector<std::string> keys();
  Vector<int> docIds();
  Doc *getDoc(int doc_id);

  template <class Archive> void serialize(Archive &archive) { archive(count, index, docs); };

private:
  int count;
  HashTable<int, Doc> docs;
  HashTable<std::string, HashTable<int, int>> index;
  const TextCleaner &cleaner;
};
