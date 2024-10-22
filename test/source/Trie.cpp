
#include "booksearch/Trie.h"

#include <doctest/doctest.h>

TEST_CASE("Trie constructor") { Trie trie; }

TEST_CASE("LinkedList insertKey and searchKey") {
  Trie trie;

  SUBCASE("Single word insert and search") {
    trie.insertKey("hello");
    CHECK(trie.searchKey("hello"));
  }

  SUBCASE("Multiple word insert and search") {
    trie.insertKey("hello");
    trie.insertKey("hell");
    trie.insertKey("help");
    CHECK(trie.searchKey("hello"));
    CHECK(trie.searchKey("hell"));
    CHECK(trie.searchKey("help"));
  }

  SUBCASE("Search for non-inserted word") {
    trie.insertKey("hello");
    CHECK(!trie.searchKey("world"));
  }
}

TEST_CASE("Trie suggestWords") {
  Trie trie;

  SUBCASE("Suggest words with common prefix") {
    trie.insertKey("hello");
    trie.insertKey("hell");
    trie.insertKey("help");

    Vector<std::string> suggestions = trie.suggestWords("hel");
    CHECK(suggestions.size() == 3);
    CHECK(suggestions[0] == "hell");
    CHECK(suggestions[1] == "hello");
    CHECK(suggestions[2] == "help");
  }
}