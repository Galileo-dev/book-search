#pragma once

#include <booksearch/Vector.h>

#include <cereal/types/memory.hpp>
#include <memory>
#include <string>

#define TRIE_ALPHABET_SIZE 36  // 26 Letters + 10 digits

class Trie {
public:
  Trie();

  void insertKey(const std::string& key);
  bool searchKey(const std::string& key);
  Vector<std::string> suggestWords(const std::string& key);

  template <class Archive> void serialize(Archive& archive) { archive(root); }

private:
  struct Node {
    std::unique_ptr<Node> children[TRIE_ALPHABET_SIZE];
    bool is_leaf;

    Node();
    template <class Archive> void serialize(Archive& archive) { archive(children, is_leaf); }
  };

  std::unique_ptr<Node> root;

  void collectAllWords(Node* node, std::string current_word, Vector<std::string>& results);
  int charToIndex(char c);
  char indexToChar(int index);
};
