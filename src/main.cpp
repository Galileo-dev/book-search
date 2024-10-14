#include "inverted_index.h"
#include "logging.h"
#include "settings.h"
#include <cereal/archives/json.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#define DEFAULT_INDEX_PATH "./index.json"

int main(int argc, const char *argv[]) {
  LOG_INIT_COUT();
  log.set_log_level(LOG_INFO);

  CLISettings settings = parse_settings(argc, argv);

  // load our index
  InvertedIndex searchEngine;

  std::ifstream is(DEFAULT_INDEX_PATH);
  if (is.is_open()) {
    cereal::JSONInputArchive archive(is);
    archive(searchEngine);
  }

  if (settings.help) {
    usage(argv[0]);
  }

  // Main commands
  if (settings.document_path.has_value()) {
    std::string path = settings.document_path.value();
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);

    searchEngine.add_document_from_file(path, path);
    log(LOG_INFO) << "Added document: " << base_filename << " Path: " << path
                  << std::endl;
  }

  if (settings.search_term.has_value()) {
    auto results = searchEngine.search(settings.search_term.value());

    if (results.empty()) {
      log(LOG_INFO) << "No such term found!" << std::endl;
    }

    for (const auto &result : results) {
      log(LOG_INFO) << result.first << ", " << result.second << std::endl;
    }
  }

  // save the index
  std::ofstream os(DEFAULT_INDEX_PATH);
  cereal::JSONOutputArchive archive(os);
  archive(searchEngine);
}
