#include "settings.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int main(int argc, const char *argv[]) {
  CLISettings settings = parse_settings(argc, argv);

  if (settings.help) {
    usage(argv[0]);
  }
}