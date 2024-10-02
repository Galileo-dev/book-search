#include "hashtable.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
class InvertedIndex {
public:
  InvertedIndex() : count(0), docs(), index() {}

  int add_document(std::string name, std::istream &stream) {
    count++; // increment document id
    docs.set(count, std::make_unique<std::string>(name));

    std::string word;
    while (stream >> word) {
      // Convert word to lowercase
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);

      // find word
      HashMap<int, int> *entry = index.get(word);
      if (entry) {
        int *freq = entry->get(count);
        if (freq != nullptr) {
          *freq += 1;
        } else {
          entry->set(count, std::make_unique<int>(1));
        }
      } else { // if term does not exist, create a new entry
        auto new_entry = std::make_unique<HashMap<int, int>>();
        new_entry->set(count, std::make_unique<int>(1));
        index.set(word, std::move(new_entry));
      }
    }
    return 0;
  }

  std::vector<std::pair<std::string, int>> search(std::string key) {
    // Convert key to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    HashMap<int, int> *freq_list = index.get(key);
    if (freq_list == nullptr) {
      return {};
    }

    // collect doc ids from frequency list
    std::vector<int> doc_ids = freq_list->keys();

    std::sort(doc_ids.begin(), doc_ids.end(),
              [&freq_list](int doc_a_id, int doc_b_id) {
                return freq_list->get(doc_a_id) < freq_list->get(doc_b_id);
              });

    std::vector<std::pair<std::string, int>> doc_names;
    std::transform(doc_ids.begin(), doc_ids.end(),
                   std::back_inserter(doc_names), [&](int doc_id) {
                     auto name_it = docs.get(doc_id);
                     auto freq_it = freq_list->get(doc_id);
                     return (name_it != nullptr && freq_it != nullptr)
                                ? std::make_pair(*name_it, *freq_it)
                                : std::make_pair(
                                      "",
                                      0); // Return a default pair if not found
                   });
    doc_names.erase(
        std::remove_if(doc_names.begin(), doc_names.end(),
                       [](const auto &pair) { return pair.first.empty(); }),
        doc_names.end());

    return doc_names;
  }

  int add_document_from_file(std::string name, std::string path) {
    // iterate by whitespace
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Error adding document: " << std::strerror(errno)
                << std::endl;
      return 1;
    }
    int result = add_document(name, file);
    file.close();
    return result;
  }

private:
  int count;
  HashMap<int, std::string> docs;
  HashMap<std::string, HashMap<int, int>> index;
};