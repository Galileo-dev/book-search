#include "test_utils.h"
#include "vector.h"

void test_constructor() {
  Vector<int> vec;
  IS_TRUE(vec.begin() == vec.end()); // Initially empty
}

void test_push_back() {
  Vector<int> vec;
  vec.push_back(10);
  IS_TRUE(vec[0] == 10);

  vec.push_back(20);
  vec.push_back(30);
  IS_TRUE(vec[2] == 30);
  IS_TRUE(vec[1] == 20);
}

void test_reserve() {
  Vector<int> vec;
  vec.reserve(100); // Make sure capacity grows correctly
  vec.push_back(10);
  IS_TRUE(vec[0] == 10);
}

void test_resize() {
  Vector<int> vec = {1, 2, 3};
  vec.push_back(4);
  vec.push_back(5);
  IS_TRUE(vec[4] == 5);
  IS_TRUE(vec[3] == 4);
}

void test_equality_operator() {
  Vector<int> vec1 = {1, 2, 3};
  Vector<int> vec2 = {1, 2, 3};
  IS_TRUE(vec1 == vec2); // Should be true

  vec2.push_back(4);
  IS_TRUE(!(vec1 == vec2)); // Should be false after modification
}

int test_vector() {
  test_constructor();
  test_push_back();
  test_reserve();
  test_resize();
  test_equality_operator();

  std::cout << "All Vector tests completed." << std::endl;
  return 0;
}
