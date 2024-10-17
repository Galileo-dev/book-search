#include "booksearch/hashtable.h"
#include "booksearch/hash.h"
#include <doctest/doctest.h>
#include <iostream>
#include <memory>
#include <string>

TEST_CASE("HashTable basic set and get") {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("key1", std::move(value1));
  map.set("key2", std::move(value2));

  CHECK(map.get("key1") != nullptr);
  CHECK(*map.get("key1") == 100);
  CHECK(map.get("key2") != nullptr);
  CHECK(*map.get("key2") == 200);

  CHECK(map.get("key3") == nullptr);
}

TEST_CASE("HashTable keys test") {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("key1", std::move(value1));
  map.set("key2", std::move(value2));

  Vector<std::string> expected = {"key1", "key2"};
  CHECK(map.keys() == expected);
}

TEST_CASE("HashTable update value") {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(300);

  map.set("key1", std::move(value1));
  CHECK(map.get("key1") != nullptr);
  CHECK(*map.get("key1") == 100);

  map.set("key1", std::move(value2));
  CHECK(map.get("key1") != nullptr);
  CHECK(*map.get("key1") == 300);
}

TEST_CASE("HashTable expansion") {
  HashTable<std::string, int> map;

  for (int i = 0; i < 9; ++i) {
    auto value = std::make_unique<int>(i);
    map.set("key" + std::to_string(i), std::move(value));
  }

  for (int i = 0; i < 9; ++i) {
    CHECK(*map.get("key" + std::to_string(i)) == i);
  }

  CHECK(map.get("key0") != nullptr);
  CHECK(map.get("key8") != nullptr);
}

TEST_CASE("HashTable collision handling") {
  HashTable<std::string, int, FNV1AHasher> map;

  // Should collide based on:
  // https://github.com/pstibrany/fnv-1a-64bit-collisions
  FNV1AHasher hasher;
  CHECK(hasher("8yn0iYCKYHlIj4-BwPqk") == hasher("GReLUrM4wMqfg9yzV3KQ"));

  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("8yn0iYCKYHlIj4-BwPqk", std::move(value1));
  map.set("GReLUrM4wMqfg9yzV3KQ", std::move(value2));

  CHECK(map.get("8yn0iYCKYHlIj4-BwPqk") != nullptr);
  CHECK(*map.get("8yn0iYCKYHlIj4-BwPqk") == 100);
  CHECK(map.get("GReLUrM4wMqfg9yzV3KQ") != nullptr);
  CHECK(*map.get("GReLUrM4wMqfg9yzV3KQ") == 200);
}
