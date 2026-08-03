#ifndef TECSLOG_CORE_HPP
#define TECSLOG_CORE_HPP

#include <string>

namespace tecslog
{
  enum Level
  {
    INFO,
    WARNING,
    ERROR,
    LAST
  };

  class Logger
  {
  public:
    static Logger& instance();
    static Logger& instance(const std::string& filename, Level min_level);

    void setLevel(Level min_level);

    int info(const std::string& message);
    int warning(const std::string& message);
    int error(const std::string& message);
  private:
    std::ostream& out_;
    Level min_level_;

    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    int baseLog(Level level, const std::string& message);
  };

  void init(const std::string& filename, Level min_level);
  int info(const std::string& message);
  int warning(const std::string& message);
  int error(const std::string& message);
}

#endif
