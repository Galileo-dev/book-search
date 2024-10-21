#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>

#include "hashtable.h"
#include "text_cleaner.h"
#include "vector.h"
class InvertedIndex {
public:
  InvertedIndex(const TextCleaner &cleaner) : count(0), docs(), index(), cleaner(cleaner) {}

  struct Doc {
    std::string name;
    std::string path;

    Doc() {}
    Doc(const std::string &name, const std::string &path) : name(name), path(path) {}

    template <class Archive> void serialize(Archive &archive) { archive(name, path); }
  };

  struct SearchResult {
    Doc doc;
    int freq;

    SearchResult() {}
    SearchResult(const Doc &doc, const int &freq) : doc(doc), freq(freq) {}
  };

  int add_document(std::string name, std::istream &stream) {
    count++;  // increment document id
    docs.set(count, std::make_unique<Doc>(name, name));
    int total_words = 0;
    std::string word;
    while (stream >> word) {
      word = cleaner.clean_word(word);

      if (!word.empty() && !cleaner.is_stop_word(word)) {
        total_words++;
        // find word
        auto *entry = index.get(word);
        if (entry) {
          int *freq = entry->get(count);
          if (freq != nullptr) {
            *freq += 1;
          } else {
            entry->set(count, std::make_unique<int>(1));
          }
        } else {  // if term does not exist, create a new entry
          auto new_entry = std::make_unique<HashTable<int, int>>();
          new_entry->set(count, std::make_unique<int>(1));
          index.set(word, std::move(new_entry));
        }
      }
    }
    doc_word_count.set(count, std::make_unique<int>(total_words));
    return 0;
  }

  int add_document_from_file(std::string name, std::string path) {
    // iterate by whitespace
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Error adding document: " << std::strerror(errno) << std::endl;
      return 1;
    }
    int result = add_document(name, file);
    file.close();
    return result;
  }

  Vector<SearchResult> search(std::string key) {
    // Convert key to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    auto *freq_list = index.get(key);
    if (freq_list == nullptr) {
      return {};
    }

    // collect doc ids from frequency list
    auto doc_ids = freq_list->keys();

    // Sort doc ids by frequency in descending order
    std::sort(doc_ids.begin(), doc_ids.end(), [&freq_list](int doc_a_id, int doc_b_id) {
      return *freq_list->get(doc_a_id) > *freq_list->get(doc_b_id);
    });

    Vector<SearchResult> results;
    for (int doc_id : doc_ids) {
      auto doc_it = docs.get(doc_id);
      auto freq_it = freq_list->get(doc_id);
      if (doc_it != nullptr) {
        SearchResult result(*doc_it, *freq_it);
        results.push_back(result);
      }
    }
    return results;
  }

  Vector<std::string> keys() { return index.keys(); }

  template <class Archive> void serialize(Archive &archive) { archive(count, index, docs); }

private:
  int count;
  HashTable<int, Doc> docs;
  HashTable<int, int> doc_word_count;
  HashTable<std::string, HashTable<int, int>> index;
  const TextCleaner &cleaner;
};