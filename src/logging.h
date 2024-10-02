// Adapted from:
// https://github.com/bschiffthaler/BSlogger/blob/9b0e8e33819e518a847a55bf8fba6a0b6820f903/src/bslogger_examples.cpp

#include <ostream>
#define LOG_DEBUG 0
#define LOG_ERROR 1
#define LOG_INFO 2
#define LOG_INIT_COUT() Logger log(std::cout, __PRETTY_FUNCTION__)

class Logger {
public:
  Logger(std::ostream &out, std::string name)
      : _output(out), _name(name), _log_level(LOG_ERROR) {}

  Logger &operator()(unsigned msg_level) {
    _msg_level = msg_level;
    return *this;
  }
  template <typename T>
  friend Logger &operator<<(Logger &logger, const T &msg) {
    if (logger._should_log()) {
      if (logger._log_level < LOG_ERROR) {
        // only include function signature if LOG_DBEUG
        logger._output << "[" << logger._name << "]: " << msg;
      } else {
        logger._output << msg;
      }
    }
    return logger;
  }

  friend Logger &operator<<(Logger &logger,
                            std::ostream &(*manip)(std::ostream &)) {
    if (logger._should_log()) {
      logger._output << manip;
    }
    return logger;
  }

  void set_log_level(unsigned log_level) { _log_level = log_level; }

private:
  std::ostream &_output;
  std::string _name;
  unsigned _log_level;
  unsigned _msg_level;

  bool _should_log() const {
    if (_log_level == LOG_ERROR && _msg_level == LOG_ERROR) {
      return true;
    } else if (_log_level == LOG_INFO &&
               (_msg_level == LOG_INFO || _msg_level == LOG_ERROR)) {
      return true;
    } else if (_log_level == LOG_DEBUG) {
      return true;
    }
    return false;
  }
};