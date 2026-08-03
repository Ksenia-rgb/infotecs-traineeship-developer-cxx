#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>

#include <tecslog/types.hpp>

namespace tecslog
{
  class Logger
  {
  public:
    static Logger& instance();

    void setFile(const std::string& filename);
    bool isFileOpen();
    void setLevel(Level min_level);
    void reset();

    std::error_code info(const std::string& message);
    std::error_code warning(const std::string& message);
    std::error_code error(const std::string& message);
  private:
    static const std::string default_filename_;
    static const Level default_level_;

    std::string filename_;
    Level level_;
    std::ofstream out_;

    std::mutex mutex_level;
    std::mutex mutex_file;

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::error_code baseLog(Level level, const std::string& message);
  };
}

#endif
