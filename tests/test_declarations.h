
#ifndef TEST_DECLARATIONS_H
#define TEST_DECLARATIONS_H
#define IS_TRUE(x)                                                             \
  {                                                                            \
    if (!(x))                                                                  \
      std::cout << __FUNCTION__ << " failed on line " << __LINE__              \
                << std::endl;                                                  \
  }

void test_linkedlist();
void test_hashmap();

#endif // TEST_DECLARATIONS_HPP