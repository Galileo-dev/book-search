
#include "booksearch/linkedlist.h"
#include <doctest/doctest.h>

TEST_CASE("LinkedList constructor") { LinkedList<int> list; }

TEST_CASE("LinkedList addValue and popValue") {
  LinkedList<int> list;

  SUBCASE("Single value addition and pop") {
    list.addValue(10);
    CHECK(list.popValue() == 10);
  }

  SUBCASE("Multiple value addition and pop") {
    list.addValue(20);
    list.addValue(30);
    CHECK(list.popValue() == 30);
    CHECK(list.popValue() == 20);
  }
}

TEST_CASE("LinkedList popValue") {
  LinkedList<int> list;

  SUBCASE("Pop after two additions") {
    list.addValue(5);
    list.addValue(15);
    CHECK(list.popValue() == 15);
    CHECK(list.popValue() == 5);
  }

  SUBCASE("Pop after single addition") {
    list.addValue(25);
    CHECK(list.popValue() == 25);
  }
}
