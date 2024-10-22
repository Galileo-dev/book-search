#include <booksearch/InvertedIndex.h>
#include <booksearch/Trie.h>
#include <booksearch/utils.h>
#include <cereal/archives/binary.hpp>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#define DEFAULT_INDEX_PATH "./index.bin"

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
  InvertedIndex searchEngine(textCleaner);
  Trie autoComplete;

  // load our index
  std::ifstream is(DEFAULT_INDEX_PATH);
  if (is.is_open()) {
    cereal::BinaryInputArchive iarchive(is);
    iarchive(searchEngine, autoComplete);
  }

  if (settings.count("help")) {
    spdlog::info(options.help());
    return 0;
  }

  if (settings.count("add")) {
    std::string document_path = settings["add"].as<std::string>();

    std::filesystem::directory_entry entry(document_path);

    if (!entry.exists()) {
      spdlog::error("Invalid path provided: {}", document_path);
      return -1;
    }

    Vector<std::filesystem::directory_entry> entries;
    if (entry.is_directory()) {
      for (const auto &e : std::filesystem::directory_iterator(document_path)) {
        entries.push_back(e);
      }
    } else if (entry.is_regular_file()) {
      entries.push_back(entry);
    }

    for (const auto &e : entries) {
      if (e.is_regular_file() && e.path().extension() == ".txt") {
        std::string file_path = e.path().string();
        std::string base_filename = e.path().filename().string();

        searchEngine.add_document_from_file(base_filename, file_path);
        spdlog::info("Added document: {}, Path: {}", base_filename, file_path);
      }
    }

    // update autocomplete trie
    for (const auto &key : searchEngine.keys()) {
      autoComplete.insertKey(key);
    }
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
      // use trie to show similar words
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
      paginate_results(results, 10);
    }
  }

  // save the index
  std::ofstream os(DEFAULT_INDEX_PATH);
  cereal::BinaryOutputArchive oarchive(os);
  oarchive(searchEngine, autoComplete);
}
