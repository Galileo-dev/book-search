#include <cereal/types/memory.hpp>
#include <memory>
#include <string>
#include <vector>
#define TRIE_ALPHABET_SIZE 36  // 26 Letters + 10 digits

class Trie {
public:
  Trie() : root(std::make_unique<Node>()) {}

  inline void insertKey(const std::string& key) {
    Node* curr = root.get();

    for (char c : key) {
      int index = charToIndex(c);
      if (!curr->children[index]) {
        curr->children[index] = std::make_unique<Node>();
      }
      curr = curr->children[index].get();
    }
    curr->is_leaf = true;
  }

  bool searchKey(const std::string& key) {
    Node* curr = root.get();
    for (char c : key) {
      int index = charToIndex(c);
      if (!curr->children[index]) return false;

      curr = curr->children[index].get();
    }
    return curr->is_leaf;
  }

  std::vector<std::string> suggestWords(const std::string& key) {
    std::vector<std::string> results;
    Node* curr = root.get();
    std::string prefix = "";
    for (char c : key) {
      int index = charToIndex(c);
      if (!curr->children[index]) {
        break;
      }
      prefix += c;
      curr = curr->children[index].get();
    }
    collectAllWords(curr, prefix, results);

    return results;
  }

  template <class Archive> void serialize(Archive& archive) { archive(root); }

private:
  struct Node {
    std::unique_ptr<Node> children[TRIE_ALPHABET_SIZE];
    bool is_leaf;

    Node() : is_leaf(false) {}
    template <class Archive> void serialize(Archive& archive) { archive(children, is_leaf); }
  };

  std::unique_ptr<Node> root;

  void collectAllWords(Node* node, std::string current_word, std::vector<std::string>& results) {
    if (node == nullptr) return;

    if (node->is_leaf) {
      results.push_back(current_word);
    }

    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
      if (node->children[i] != nullptr) {
        char next_char = indexToChar(i);
        collectAllWords(node->children[i].get(), current_word + next_char, results);
      }
    }
  }

  int charToIndex(char c) {
    if (std::islower(c)) {
      return c - 'a';  // 0 to 25
    } else if (std::isdigit(c)) {
      return 26 + (c - '0');  // 26 to 35
    }
    return -1;
  }

  char indexToChar(int index) {
    if (index >= 0 && index < 26) {
      return 'a' + index;  // a to z
    } else if (index >= 26 && index < 36) {
      return '0' + (index - 26);  // 0 to 9
    }
    return '\0';
  }
};
