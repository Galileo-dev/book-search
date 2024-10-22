#include <spdlog/spdlog.h>

#include "CommandLineParser.h"
#include "IndexManager.h"
#include "SearchEngine.h"

#define DEFAULT_INDEX_PATH "./index.bin"

int main(int argc, const char* argv[]) {
  spdlog::set_pattern("%v");

  CommandLineParser parser(argc, argv);
  auto options = parser.parse();

  if (options.showHelp) {
    return 0;
  }

  std::string indexPath = options.indexInputPath.value_or(DEFAULT_INDEX_PATH);

  SearchEngine searchEngine;

  IndexManager indexManager(indexPath);
  indexManager.loadIndex(searchEngine.getInvertedIndex(), searchEngine.getTrie());

  if (options.documentPath) {
    searchEngine.addDocuments(options.documentPath.value());
  }

  if (options.debugMode) {
    searchEngine.debug();
  }

  if (options.searchTerm) {
    searchEngine.search(options.searchTerm.value());
  }

  std::string outputPath = options.indexOutputPath.value_or(DEFAULT_INDEX_PATH);
  IndexManager outputManager(outputPath);
  outputManager.saveIndex(searchEngine.getInvertedIndex(), searchEngine.getTrie());

  return 0;
}
