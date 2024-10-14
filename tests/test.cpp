#include "test_declarations.h"
#include "test_utils.h"
#include <iostream>

bool greater_than_five(int a) { return a > 5; }

void test_test() {
  IS_TRUE(!greater_than_five(0));
  IS_TRUE(!greater_than_five(5));
  IS_TRUE(greater_than_five(10));
}

int main(void) {
  test_test();
  test_linkedlist();
  // test_hashtable();
  test_vector();
}