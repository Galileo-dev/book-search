#include "linkedlist.h"
#include "test_utils.h"

void test_constructor() { LinkedList<int> list; }

void test_addValue() {
  LinkedList<int> list;
  list.addValue(10);
  IS_TRUE(list.popValue() == 10);

  list.addValue(20);
  list.addValue(30);
  IS_TRUE(list.popValue() == 30); // LIFO
  IS_TRUE(list.popValue() == 20); // LIFO
}

void test_popValue() {
  LinkedList<int> list;
  list.addValue(5);
  list.addValue(15);

  IS_TRUE(list.popValue() == 15);
  IS_TRUE(list.popValue() == 5);

  list.addValue(25);
  IS_TRUE(list.popValue() == 25);
}

int test_linkedlist() {
  test_constructor();
  test_addValue();
  test_popValue();

  std::cout << "All LinkedList tests completed." << std::endl;
  return 0;
}