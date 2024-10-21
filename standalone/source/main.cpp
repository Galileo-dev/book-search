#include <spdlog/spdlog.h>

#include <cereal/archives/binary.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "booksearch/inverted_index.h"
#include "booksearch/trie.h"

#define DEFAULT_INDEX_PATH "./index.bin"
constexpr char SEPARATOR[] = "--------------------------------------------------";
constexpr char BULLET[] = "•";

int main(int argc, const char *argv[]) {
  spdlog::set_pattern("%v");
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
  ("o,output", "Path to save index", cxxopts::value<std::string>())
  ("d,debug", "Show some debug info");
  // clang-format on

  auto settings = options.parse(argc, argv);

  TextCleaner textCleaner;
  // load our index
  InvertedIndex searchEngine(textCleaner);
  Trie autoComplete;

  std::ifstream is(DEFAULT_INDEX_PATH);
  if (is.is_open()) {
    cereal::BinaryInputArchive iarchive(is);
    iarchive(searchEngine, autoComplete);
  }

  if (settings.count("help")) {
    spdlog::info(options.help());
    return 0;
  }

  // Main commands
  if (settings.count("add")) {
    std::string document_path = settings["add"].as<std::string>();
    std::string base_filename = document_path.substr(document_path.find_last_of("/\\") + 1);

    searchEngine.add_document_from_file(base_filename, document_path);
    for (auto &key : searchEngine.keys()) {
      autoComplete.insertKey(key);
    }
    spdlog::info("Added document: {}, Path: {}", base_filename, document_path);
  }

  if (settings.count("debug")) {
    spdlog::info("Keys:");
    for (auto &key : searchEngine.keys()) {
      spdlog::info("  {} {}", BULLET, key);
    }
  }

  if (settings.count("search")) {
    std::string key = settings["search"].as<std::string>();
    spdlog::info("Searching for {}...", key);
    auto results = searchEngine.search(key);

    if (results.empty()) {
      spdlog::info(SEPARATOR);
      spdlog::info("No results found for: '{}'", key);
      spdlog::info(SEPARATOR);
      std::vector<std::string> suggestions = autoComplete.suggestWords(key);
      if (!suggestions.empty()) {
        spdlog::info("Did you mean one of the following?");
        for (const auto &s : suggestions) {
          spdlog::info("  {} {}", BULLET, s);  // Add bullet points with indentation
        }
      } else {
        spdlog::info("No similar terms found.");
      }
      spdlog::info(SEPARATOR);
    } else {
      spdlog::info(SEPARATOR);
      spdlog::info("Search results for '{}':", key);
      spdlog::info("{:<30} | {}", "Document Name", "Frequency");
      spdlog::info(SEPARATOR);
      for (const auto &result : results) {
        spdlog::info("{:<30} | {}", result.doc.name, result.freq);
      }
      spdlog::info(SEPARATOR);
    }
  }

  // save the index
  std::ofstream os(DEFAULT_INDEX_PATH);
  cereal::BinaryOutputArchive oarchive(os);
  oarchive(searchEngine, autoComplete);
}
