#include <functional>
#include <iostream>
#include <optional>
#include <ostream>
#include <unordered_map>

struct CLISettings {
  bool help{false};
  bool verbose{false};
  std::optional<std::string> document_path;
  std::optional<std::string> search_term;
};

template <class Out, class T>
Out &operator<<(Out &out, std::optional<T> const &v) {
  if (v)
    out << "optional(" << *v << ')';
  else
    out << "nullopt";
  return out;
}

#include <iostream>
inline void usage(const char *progName) {
  std::cout << progName << " [options]" << std::endl
            << "Options:" << std::endl
            << "-h | --help        Print this help" << std::endl
            << "-v | --verbose     Enable verbose logging" << std::endl
            << "-a | --add         Add a document to the index" << std::endl
            << "-s | --search      Search through the index" << std::endl;
}

inline std::ostream &operator<<(std::ostream &os, const CLISettings &settings) {
  return os << "help: " << settings.help << std::endl
            << "verbose: " << settings.verbose << std::endl
            << "document_path: " << settings.document_path << std::endl
            << "search_term:" << settings.search_term << std::endl;
}

typedef std::function<void(CLISettings &)> NoArgHandle;

#define S(str, f, v)                                                           \
  {                                                                            \
    str, [](CLISettings &s) { s.f = v; }                                       \
  }
const std::unordered_map<std::string, NoArgHandle> NoArgs{
    S("--help", help, true),       S("-h", help, true),

    S("--verbose", verbose, true), S("-v", verbose, true),

    S("--quiet", verbose, false),
};
#undef S

typedef std::function<void(CLISettings &, const std::string &)> OneArgHandle;

#define S(str, f, v)                                                           \
  {                                                                            \
    str, [](CLISettings &s, const std::string &arg) { s.f = v; }               \
  }

const std::unordered_map<std::string, OneArgHandle> OneArgs{
    S("--add", document_path, arg),
    S("--search", search_term, arg),
};
#undef S

inline CLISettings parse_settings(int argc, const char *argv[]) {
  CLISettings settings;

  // argv[0] is usually the program name, so start at 1
  for (int i{1}; i < argc; i++) {
    std::string opt{argv[i]};

    if (auto j{NoArgs.find(opt)}; j != NoArgs.end())
      j->second(settings); // Yes, handle it!
    else if (auto k{OneArgs.find(opt)}; k != OneArgs.end())
      if (++i < argc)
        k->second(settings, {argv[i]});
      else
        throw std::runtime_error{"missing param after " + opt};
    else
      std::cerr << "unrecognized command-line option " << opt << std::endl;
  }
  return settings;
}