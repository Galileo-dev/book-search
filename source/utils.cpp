#include <booksearch/utils.h>

#include <algorithm>
#include <iostream>

void paginate_results(const Vector<SearchResult>& results, const size_t results_per_page) {
  size_t current_page = 0;

  auto print_page = [&](size_t page) {
    size_t start_index = page * results_per_page;
    size_t end_index = std::min(start_index + results_per_page, results.size());

    spdlog::info(SEPARATOR);
    for (size_t i = start_index; i < end_index; ++i) {
      spdlog::info("{:<30} | {}", results[i].doc.name, results[i].freq);
    }
    spdlog::info(SEPARATOR);
  };

  print_page(current_page);

  while ((current_page + 1) * results_per_page < results.size()) {
    std::string input;
    spdlog::info("Show the next {} results? (y/n)", results_per_page);
    std::cin >> input;

    if (input == "y" || input == "Y") {
      ++current_page;
      print_page(current_page);
    } else {
      break;
    }
  }
}
