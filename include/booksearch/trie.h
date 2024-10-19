#include <memory>
#include <string>
#include <vector>
#define TRIE_ALPHABET_SIZE 26

class Trie {
public:
  Trie() : root(new Node()) {}

  inline void insertKey(const std::string& key) {
    Node* curr = root;

    for (char c : key) {
      if (curr->children[c - 'a'] == nullptr) {
        curr->children[c - 'a'] = new Node();
      }
      curr = curr->children[c - 'a'];
    }
    curr->is_leaf = true;
  }

  bool searchKey(const std::string& key) {
    Node* curr = root;
    for (char c : key) {
      if (curr->children[c - 'a'] == nullptr) return false;

      curr = curr->children[c - 'a'];
    }
    return curr->is_leaf;
  }

  std::vector<std::string> suggestWords(const std::string& key) {
    std::vector<std::string> results;
    Node* curr = root;
    std::string prefix = "";
    for (char c : key) {
      if (curr->children[c - 'a'] == nullptr) {
        break;
      }
      prefix += c;
      curr = curr->children[c - 'a'];
    }
    collectAllWords(curr, prefix, results);

    return results;
  }

private:
  struct Node {
    char data;
    Node* children[TRIE_ALPHABET_SIZE];
    bool is_leaf;

    Node() : data('\0'), is_leaf(false) {
      for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        children[i] = nullptr;
      }
    }
  };

  Node* root;

  void collectAllWords(Node* node, std::string current_word, std::vector<std::string>& results) {
    if (node == nullptr) return;

    if (node->is_leaf) {
      results.push_back(current_word);
    }

    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
      if (node->children[i] != nullptr) {
        char next_char = 'a' + i;
        collectAllWords(node->children[i], current_word + next_char, results);
      }
    }
  }
};
