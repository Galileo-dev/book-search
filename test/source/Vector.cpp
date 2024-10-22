#include "booksearch/Vector.h"

#include <doctest/doctest.h>

TEST_CASE("Vector constructor") {
  Vector<int> vec;
  CHECK(vec.begin() == vec.end());  // Empty
}

TEST_CASE("Vector push_back") {
  Vector<int> vec;

  SUBCASE("Single value push") {
    vec.push_back(10);
    CHECK(vec[0] == 10);
  }

  SUBCASE("Multiple value push") {
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    CHECK(vec[2] == 30);
    CHECK(vec[1] == 20);
    CHECK(vec[0] == 10);
  }
}

TEST_CASE("Vector reserve") {
  Vector<int> vec;
  vec.push_back(10);
  CHECK(vec[0] == 10);

  SUBCASE("Reserve and add") {
    vec.reserve(10);
    vec.push_back(20);
    CHECK(vec[0] == 10);
    CHECK(vec[1] == 20);
  }
}

TEST_CASE("Vector resize") {
  Vector<int> vec = {1, 2, 3};

  SUBCASE("Resize and add values") {
    vec.push_back(4);
    vec.push_back(5);
    CHECK(vec[3] == 4);
    CHECK(vec[4] == 5);
  }
}

TEST_CASE("Vector equality operator") {
  Vector<int> vec1 = {1, 2, 3};
  Vector<int> vec2 = {1, 2, 3};

  SUBCASE("before modification") { CHECK(vec1 == vec2); }

  SUBCASE("after modification") {
    vec2.push_back(4);
    CHECK_FALSE(vec1 == vec2);
  }
}
