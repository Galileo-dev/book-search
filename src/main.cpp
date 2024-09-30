#include "logging.h"
#include "settings.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int main(int argc, const char *argv[]) {
  LOG_INIT_COUT();
  log.set_log_level(LOG_DEBUG);

  CLISettings settings = parse_settings(argc, argv);

  if (settings.help) {
    usage(argv[0]);
  }
}