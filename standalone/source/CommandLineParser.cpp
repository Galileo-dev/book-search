#include "CommandLineParser.h"

#include <spdlog/spdlog.h>

CommandLineParser::CommandLineParser(int argc, const char* argv[])
    : argc(argc), argv(argv), options(*argv, "A program to index and search your *Legal* books!") {
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
}

CommandLineOptions CommandLineParser::parse() {
  CommandLineOptions cmdOptions;
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    cmdOptions.showHelp = true;
    spdlog::info(options.help());
  }

  if (result.count("version")) {
    cmdOptions.showVersion = true;
  }

  if (result.count("add")) {
    cmdOptions.documentPath = result["add"].as<std::string>();
  }

  if (result.count("search")) {
    cmdOptions.searchTerm = result["search"].as<std::string>();
  }

  if (result.count("input")) {
    cmdOptions.indexInputPath = result["input"].as<std::string>();
  }

  if (result.count("output")) {
    cmdOptions.indexOutputPath = result["output"].as<std::string>();
  }

  if (result.count("debug")) {
    cmdOptions.debugMode = true;
  }

  return cmdOptions;
}
