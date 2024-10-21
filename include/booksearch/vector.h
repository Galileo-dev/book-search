#pragma once

#include <cereal/types/array.hpp>
#include <cereal/types/memory.hpp>
#include <cstddef>
#include <initializer_list>
#include <ostream>
template <typename T> class Vector {
public:
  using iterator = T *;
  using const_iterator = const T *;

  Vector() : _data(0), _size(0), _capacity(0) {}
  Vector(size_t capacity) : _data(new T[capacity]), _size(capacity), _capacity(capacity) {
    for (size_t i = 0; i < _size; ++i) {
      _data[i] = T();
    }
  }
  Vector(std::initializer_list<T> list) {
    _size = list.size();
    _capacity = _size;
    _data = new T[_capacity];

    size_t i = 0;
    for (const auto &element : list) {
      _data[i++] = element;
    }
  }

  Vector(Vector &&other) noexcept
      : _data(other._data), _size(other._size), _capacity(other._capacity) {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
  }

  Vector &operator=(Vector &&other) noexcept {
    if (this != &other) {
      delete[] _data;
      _data = other._data;
      _size = other._size;
      _capacity = other._capacity;
      other._data = nullptr;
      other._size = 0;
      other._capacity = 0;
    }
    return *this;
  }

  ~Vector() { delete[] _data; }

  void push_back(const T &value) {
    if (_size >= _capacity) {
      size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
      reserve(new_capacity);
    }
    _data[_size++] = value;
  }

  void reserve(size_t capacity) {
    if (capacity <= _capacity) return;  // No need to reserve more
    T *new_data = new T[capacity];

    for (size_t i = 0; i < _size; ++i) {
      new_data[i] = std::move(_data[i]);  // Use move semantics if appropriate
    }

    delete[] _data;
    _data = new_data;
    _capacity = capacity;
  }

  void resize(int size) {
    reserve(size);
    _size = size;
  }

  T &operator[](size_t index) { return _data[index]; }
  const T &operator[](size_t index) const { return _data[index]; }

  friend std::ostream &operator<<(std::ostream &out, Vector<T> const &v) {
    for (size_t i = 0; i < v._size; ++i) {
      out << v._data[i] << " ";
    }
    out << std::endl;
    return out;
  }

  iterator begin() { return _data; }
  iterator end() { return _data + _size; }
  const_iterator begin() const { return _data; }
  const_iterator end() const { return _data + _size; }
  template <class Archive> void serialize(Archive &ar) {
    ar(_size, _capacity);
    for (auto &v : *this) {
      ar(v);
    }
  }

  friend bool operator==(const Vector<T> &l, const Vector<T> &r) {
    if (l._size != r._size) {
      return false;
    }
    for (size_t i = 0; i < l._size; ++i) {
      if (l._data[i] != r._data[i]) {
        return false;
      }
    }
    return true;
  }

  bool empty() const { return _size == 0; }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }

private:
  T *_data;
  size_t _size;
  size_t _capacity;
};