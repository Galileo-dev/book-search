#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <iostream> // Include this to ensure std::cout is available

#define IS_TRUE(x)                                                             \
  {                                                                            \
    if (!(x))                                                                  \
      std::cout << __FUNCTION__ << " failed on line " << __LINE__              \
                << std::endl;                                                  \
  }

#endif // TEST_UTILS_HPP