#include "SearchEngine.h"

#include <booksearch/Vector.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "Utils.h"

namespace fs = std::filesystem;

SearchEngine::SearchEngine() : searchEngine(TextCleaner()) {}

void SearchEngine::addDocuments(const std::string& documentPath) {
  fs::directory_entry entry(documentPath);

  if (!entry.exists()) {
    spdlog::error("Invalid path provided: {}", documentPath);
    return;
  }

  Vector<fs::directory_entry> entries;
  if (entry.is_directory()) {
    for (const auto& e : fs::directory_iterator(documentPath)) {
      entries.push_back(e);
    }
  } else if (entry.is_regular_file()) {
    entries.push_back(entry);
  }

  for (const auto& e : entries) {
    if (e.is_regular_file() && e.path().extension() == ".txt") {
      std::string file_path = e.path().string();
      std::string base_filename = e.path().filename().string();

      searchEngine.add_document_from_file(base_filename, file_path);
      spdlog::info("Added document: {}, Path: {}", base_filename, file_path);
    }
  }

  updateAutoComplete();
}

void SearchEngine::updateAutoComplete() {
  for (const auto& key : searchEngine.keys()) {
    autoComplete.insertKey(key);
  }
}

void SearchEngine::search(const std::string& key) {
  spdlog::info("Searching for {}...", key);
  auto results = searchEngine.search(key);

  if (results.empty()) {
    spdlog::info(SEPARATOR);
    spdlog::info("No results found for: '{}'", key);
    spdlog::info(SEPARATOR);
    // Use trie to show similar words
    auto suggestions = autoComplete.suggestWords(key);
    if (!suggestions.empty()) {
      spdlog::info("Did you mean one of the following?");
      for (const auto& s : suggestions) {
        spdlog::info("  {} {}", BULLET, s);
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

void SearchEngine::debug() {
  spdlog::info("Keys:");
  for (const auto& key : searchEngine.keys()) {
    spdlog::info("  {} {}", BULLET, key);
  }
}

InvertedIndex& SearchEngine::getInvertedIndex() { return searchEngine; }

Trie& SearchEngine::getTrie() { return autoComplete; }
