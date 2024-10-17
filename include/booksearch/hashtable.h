#pragma once

#include <memory.h>

#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <string>

#include "vector.h"

template <typename K, typename V, typename Hash = std::hash<K>> class HashTable {
public:
  HashTable() : entries(INITIAL_CAPACITY), capacity(INITIAL_CAPACITY), length(0) {}

  ~HashTable() = default;

  // Return a pointer to the value if found, otherwise nullptr.
  V *get(const K &key) const {
    size_t index = hasher(key) & (capacity - 1);

    while (entries[index].occupied) {  // Check if the slot is occupied
      if (entries[index].key == key) {
        return entries[index].value.get();
      }
      index = (index + 1) & (capacity - 1);  // end of array, wrap around.
    }
    return nullptr;
  }

  // Insert or update the key-value pair.
  void set(K key, std::unique_ptr<V> value) {
    if (length >= capacity / 2) {
      expand();
    }
    set_entry(std::move(key), std::move(value));
  }

  Vector<K> keys() {
    Vector<K> key_list;
    key_list.reserve(length);

    for (const auto &entry : entries) {
      if (entry.occupied) {
        key_list.push_back(entry.key);
      }
    }

    return key_list;
  }

  template <class Archive> void serialize(Archive &archive) { archive(entries, capacity, length); }

private:
  static constexpr size_t INITIAL_CAPACITY = 16;

  struct Entry {
    K key;
    std::unique_ptr<V> value;
    bool occupied = false;

    template <class Archive> void serialize(Archive &ar) { ar(key, value, occupied); }
  };

  Vector<Entry> entries;  // Array of slots
  size_t capacity;        // size of _entries array
  size_t length;          // number of items in hashtable
  Hash hasher;

  // Expand hash table to twice its current size.
  void expand() {
    size_t new_capacity = capacity * 2;
    Vector<Entry> new_entries(new_capacity);

    for (auto &entry : entries) {
      if (entry.occupied) {
        size_t index = hasher(entry.key) & (new_capacity - 1);
        while (new_entries[index].occupied) {
          index = (index + 1) & (capacity - 1);
        }
        new_entries[index] = std::move(entry);
      }
    }

    entries = std::move(new_entries);
    capacity = new_capacity;
  }

  // Internal method to set or update an entry.
  void set_entry(K &&key, std::unique_ptr<V> &&value) {
    size_t index = hasher(key) & (capacity - 1);

    while (entries[index].occupied) {
      if (entries[index].key == key) {
        entries[index].value = std::move(value);
        return;
      }
      index = (index + 1) & (capacity - 1);
    }

    entries[index].key = std::move(key);
    entries[index].value = std::move(value);
    entries[index].occupied = true;
    length++;
  }
};
