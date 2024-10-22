#include <booksearch/Trie.h>

#include <cctype>

// Constructor
Trie::Trie() : root(std::make_unique<Node>()) {}

// Node Constructor
Trie::Node::Node() : is_leaf(false) {}

// Insert a key into the Trie
void Trie::insertKey(const std::string& key) {
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

// Search for a key in the Trie
bool Trie::searchKey(const std::string& key) {
  Node* curr = root.get();
  for (char c : key) {
    int index = charToIndex(c);
    if (!curr->children[index]) return false;

    curr = curr->children[index].get();
  }
  return curr->is_leaf;
}

// Suggest words based on a prefix
std::vector<std::string> Trie::suggestWords(const std::string& key) {
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

// Collect all words in the Trie
void Trie::collectAllWords(Node* node, std::string current_word,
                           std::vector<std::string>& results) {
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

// Convert character to Trie index
int Trie::charToIndex(char c) {
  if (std::islower(c)) {
    return c - 'a';  // 0 to 25
  } else if (std::isdigit(c)) {
    return 26 + (c - '0');  // 26 to 35
  }
  return -1;
}

// Convert Trie index back to character
char Trie::indexToChar(int index) {
  if (index >= 0 && index < 26) {
    return 'a' + index;  // a to z
  } else if (index >= 26 && index < 36) {
    return '0' + (index - 26);  // 0 to 9
  }
  return '\0';
}
