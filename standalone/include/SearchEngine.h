#pragma once

#include <string>
#include <booksearch/InvertedIndex.h>
#include <booksearch/Trie.h>

class SearchEngine {
public:
    SearchEngine();
    void addDocuments(const std::string& documentPath);
    void search(const std::string& key);
    void debug();
    InvertedIndex& getInvertedIndex();
    Trie& getTrie();

private:
    InvertedIndex searchEngine;
    Trie autoComplete;
    void updateAutoComplete();
};