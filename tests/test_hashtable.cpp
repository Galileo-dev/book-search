#include "hashtable.h"
#include "test_utils.h"
// #include "vector.h"
#include <iostream>
#include <memory>
#include <string>

void test_basic_set_get() {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("key1", std::move(value1));
  map.set("key2", std::move(value2));

  IS_TRUE(*map.get("key1") == 100);
  IS_TRUE(*map.get("key2") == 200);

  IS_TRUE(map.get("key3") == nullptr);
}

void test_keys() {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("key1", std::move(value1));
  map.set("key2", std::move(value2));
  // Vector<std::string> expected = {"key1", "key2"};
  // IS_TRUE(map.keys() == expected);
}

void test_update_value() {
  HashTable<std::string, int> map;
  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(300);

  map.set("key1", std::move(value1));
  IS_TRUE(*map.get("key1") == 100);

  map.set("key1", std::move(value2));
  IS_TRUE(*map.get("key1") == 300);
}

void test_expand() {
  HashTable<std::string, int> map;

  for (int i = 0; i < 9; ++i) {
    auto value = std::make_unique<int>(i);
    map.set("key" + std::to_string(i), std::move(value));
  }

  for (int i = 0; i < 9; ++i) {
    IS_TRUE(*map.get("key" + std::to_string(i)) == i);
  }

  IS_TRUE(map.get("key0") != nullptr);
  IS_TRUE(map.get("key8") != nullptr);
}

void test_collision_handling() {
  HashTable<std::string, int> map;

  // Should collide based on:
  // https://github.com/pstibrany/fnv-1a-64bit-collisions
  IS_TRUE(hash_key("8yn0iYCKYHlIj4-BwPqk") == hash_key("GReLUrM4wMqfg9yzV3KQ"))

  auto value1 = std::make_unique<int>(100);
  auto value2 = std::make_unique<int>(200);

  map.set("8yn0iYCKYHlIj4-BwPqk", std::move(value1));
  map.set("GReLUrM4wMqfg9yzV3KQ", std::move(value2));

  IS_TRUE(*map.get("8yn0iYCKYHlIj4-BwPqk") == 100);
  IS_TRUE(*map.get("GReLUrM4wMqfg9yzV3KQ") == 200);
}

int test_hashtable() {
  test_basic_set_get();
  test_keys();
  test_update_value();
  test_expand();
  test_collision_handling();

  std::cout << "All HashTable tests completed." << std::endl;
  return 0;
}
