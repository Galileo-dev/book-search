#include <spdlog/spdlog.h>

#include <cereal/archives/json.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "booksearch/inverted_index.h"

#define DEFAULT_INDEX_PATH "./index.json"

int main(int argc, const char *argv[]) {
  cxxopts::Options options(*argv, "A program to index and search your *Legal* books!");

  std::string document_path;
  std::string search_term;
  std::string index_output_path;
  std::string index_input_path;

  // clang-format off
  options.add_options()
  ("h,help", "Show help")
  ("v,version", "Print the current version number")
  ("a,add", "Add a document to the index", cxxopts::value<std::string>())
  ("s,search", "Search through the index", cxxopts::value<std::string>())
  ("i,input", "Path to load index", cxxopts::value<std::string>())
  ("o,output", "Path to save index", cxxopts::value<std::string>());
  // clang-format on

  auto settings = options.parse(argc, argv);

  // load our index
  InvertedIndex searchEngine;

  std::ifstream is(DEFAULT_INDEX_PATH);
  if (is.is_open()) {
    cereal::JSONInputArchive archive(is);
    archive(searchEngine);
  }

  if (settings.count("help")) {
    spdlog::info(options.help());
    return 0;
  }

  // Main commands
  if (settings.count("document_path")) {
    std::string document_path = settings["document_path"].as<std::string>();
    std::string base_filename = document_path.substr(document_path.find_last_of("/\\") + 1);

    searchEngine.add_document_from_file(base_filename, document_path);
    spdlog::info("Added document: {}, Path: {}", base_filename, document_path);
  }

  if (settings.count("search_term")) {
    auto results = searchEngine.search(settings["search_term"].as<std::string>());

    if (results.empty()) {
      spdlog::info("No such term found!");
    }

    for (const auto &result : results) {
      spdlog::info("{}, {}", result.first, result.second);
    }
  }

  // save the index
  std::ofstream os(DEFAULT_INDEX_PATH);
  cereal::JSONOutputArchive archive(os);
  archive(searchEngine);
}
