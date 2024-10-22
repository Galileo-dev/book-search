#include <booksearch/InvertedIndex.h>

Doc::Doc() {}

Doc::Doc(const std::string &name, const std::string &path) : name(name), path(path) {}

SearchResult::SearchResult() {}

SearchResult::SearchResult(const Doc &doc, const int &freq) : doc(doc), freq(freq) {}

InvertedIndex::InvertedIndex(const TextCleaner &cleaner)
    : count(0), docs(), index(), cleaner(cleaner) {}

int InvertedIndex::addDocument(std::string name, std::istream &stream) {
  count++;  // increment document id
  docs.set(count, std::make_unique<Doc>(name, name));

  std::string word;
  while (stream >> word) {
    word = cleaner.clean_word(word);

    if (!word.empty() && !cleaner.is_stop_word(word)) {
      auto *entry = index.get(word);
      if (entry) {
        int *freq = entry->get(count);
        if (freq != nullptr) {
          *freq += 1;
        } else {
          entry->set(count, std::make_unique<int>(1));
        }
      } else {
        auto new_entry = std::make_unique<HashTable<int, int>>();
        new_entry->set(count, std::make_unique<int>(1));
        index.set(word, std::move(new_entry));
      }
    }
  }
  return 0;
}

int InvertedIndex::addDocument(std::string name, std::string path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error adding document: " << std::strerror(errno) << std::endl;
    return 1;
  }
  int result = addDocument(name, file);
  file.close();
  return result;
}

Vector<SearchResult> InvertedIndex::search(std::string key) {
  auto *freq_list = index.get(key);
  if (freq_list == nullptr) {
    return {};
  }
  Vector<SearchResult> results;
  auto doc_ids = freq_list->keys();
  for (int doc_id : doc_ids) {
    int *freq = freq_list->get(doc_id);
    if (freq != nullptr) {
      auto *doc = docs.get(doc_id);
      if (doc != nullptr) {
        SearchResult result(*doc, *freq);
        results.push_back(result);
      }
    }
  }
  return results;
}

Vector<std::string> InvertedIndex::keys() { return index.keys(); }

Vector<int> InvertedIndex::docIds() { return docs.keys(); }

Doc *InvertedIndex::getDoc(int doc_id) { return docs.get(doc_id); }
