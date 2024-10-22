#include "SearchEngine.h"

#include <booksearch/Vector.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "Utils.h"

namespace fs = std::filesystem;

SearchEngine::SearchEngine() : invertedIndex(TextCleaner()) {}

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

      invertedIndex.addDocument(base_filename, file_path);
      spdlog::info("Added document: {}, Path: {}", base_filename, file_path);
    }
  }

  updateAutoComplete();
}

void SearchEngine::updateAutoComplete() {
  for (const auto& key : invertedIndex.keys()) {
    autoComplete.insertKey(key);
  }
}

void SearchEngine::search(const std::string& query) {
  spdlog::info("Searching for {}...", query);

  Vector<std::string> tokens = tokenize(query);

  if (tokens.empty()) {
    spdlog::info("Empty query ¯\\_(ツ)_/¯");
    return;
  }

  Vector<SearchResult> results;
  std::string prevOperator = "AND";
  bool expectingOperator = false;

  for (auto& token : tokens) {
    if (token == "AND" || token == "OR" || token == "NOT") {
      prevOperator = token;
      expectingOperator = false;
    } else {
      // It's a term
      Vector<SearchResult> termResults;
      if (prevOperator == "NOT") {
        auto termDocs = invertedIndex.search(token);
        auto allDocs = getAllDocuments();
        termResults = subtractOp(allDocs, termDocs);
        prevOperator = "AND";
      } else {
        termResults = invertedIndex.search(token);
      }

      if (results.empty()) {
        results = termResults;
      } else {
        if (prevOperator == "AND") {
          results = intersectOp(results, termResults);
        } else if (prevOperator == "OR") {
          results = unionOp(results, termResults);
        }
      }

      expectingOperator = true;
    }
  }

  if (results.empty()) {
    spdlog::info(SEPARATOR);
    spdlog::info("No results found for: '{}'", query);
    spdlog::info(SEPARATOR);
    auto suggestions = autoComplete.suggestWords(query);
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
    std::sort(results.begin(), results.end(),
              [](const SearchResult& a, const SearchResult& b) { return a.freq > b.freq; });

    spdlog::info(SEPARATOR);
    spdlog::info("Search results for '{}':", query);
    spdlog::info("{:<30} | {}", "Document Name", "Frequency");
    spdlog::info(SEPARATOR);
    paginate_results(results, 10);
  }
}

void SearchEngine::debug() {
  spdlog::info("Keys:");
  for (const auto& key : invertedIndex.keys()) {
    spdlog::info("  {} {}", BULLET, key);
  }
}

InvertedIndex& SearchEngine::getInvertedIndex() { return invertedIndex; }

Trie& SearchEngine::getTrie() { return autoComplete; }

Vector<std::string> SearchEngine::tokenize(const std::string& query) {
  std::istringstream iss(query);
  Vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

Vector<SearchResult> SearchEngine::getAllDocuments() {
  Vector<SearchResult> allDocs;
  auto doc_ids = invertedIndex.docIds();
  for (int doc_id : doc_ids) {
    Doc* doc = invertedIndex.getDoc(doc_id);
    if (doc != nullptr) {
      allDocs.push_back(SearchResult(*doc, 1));
    }
  }
  return allDocs;
}

Vector<SearchResult> SearchEngine::intersectOp(const Vector<SearchResult>& vec1,
                                               const Vector<SearchResult>& vec2) {
  Vector<SearchResult> result;
  for (const auto& res1 : vec1) {
    for (const auto& res2 : vec2) {
      if (res1.doc == res2.doc) {
        int total_freq = res1.freq + res2.freq;
        result.push_back(SearchResult(res1.doc, total_freq));
        break;
      }
    }
  }
  return result;
}

Vector<SearchResult> SearchEngine::unionOp(const Vector<SearchResult>& vec1,
                                           const Vector<SearchResult>& vec2) {
  Vector<SearchResult> result = vec1;

  for (const auto& res2 : vec2) {
    bool found = false;
    for (auto& res1 : result) {
      if (res2.doc == res1.doc) {
        res1.freq += res2.freq;
        found = true;
        break;
      }
    }
    if (!found) {
      result.push_back(res2);
    }
  }
  return result;
}

Vector<SearchResult> SearchEngine::subtractOp(const Vector<SearchResult>& vec1,
                                              const Vector<SearchResult>& vec2) {
  Vector<SearchResult> result;
  for (const auto& res1 : vec1) {
    bool found = false;
    for (const auto& res2 : vec2) {
      if (res1.doc == res2.doc) {
        found = true;
        break;
      }
    }
    if (!found) {
      result.push_back(res1);
    }
  }
  return result;
}
