#pragma once

#include <string>
#include <booksearch/InvertedIndex.h>
#include <booksearch/Trie.h>

class IndexManager {
public:
    IndexManager(const std::string& indexPath);
    void loadIndex(InvertedIndex& searchEngine, Trie& autoComplete);
    void saveIndex(const InvertedIndex& searchEngine, const Trie& autoComplete);

private:
    std::string indexPath;
};
