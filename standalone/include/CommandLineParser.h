#pragma once

#include <string>
#include <optional>
#include <cxxopts.hpp>

struct CommandLineOptions {
    bool showHelp = false;
    bool showVersion = false;
    std::optional<std::string> documentPath;
    std::optional<std::string> searchTerm;
    std::optional<std::string> indexInputPath;
    std::optional<std::string> indexOutputPath;
    bool debugMode = false;
};

class CommandLineParser {
public:
    CommandLineParser(int argc, const char* argv[]);
    CommandLineOptions parse();

private:
    int argc;
    const char** argv;
    cxxopts::Options options;
};
