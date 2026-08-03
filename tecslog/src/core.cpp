#include "../include/tecslog/core.hpp"

#include <ctime>
#include <iomanip>
#include <fstream>

namespace
{
  std::ostream& out_now_time(std::ostream& out)
  {
    std::time_t now_c = std::time(nullptr);
    return out << std::put_time(std::localtime(&now_c), "%d.%m.%Y %H:%M:%S");
  }
  std::string levelToStr(tecslog::Level level)
  {
    std::string str_levels[] = {"INFO", "WARNING", "ERROR"};
    if (level >= 0 && level < tecslog::Level::LAST)
    {
      return str_levels[level];
    }
    return {};
  }
}

int tecslog::Logger::info(const std::string& message)
{
  return baseLog(Level::INFO, message);
}

int tecslog::Logger::warning(const std::string& message)
{
  return baseLog(Level::WARNING, message);
}

int tecslog::Logger::error(const std::string& message)
{
  return baseLog(Level::ERROR, message);
}

int tecslog::Logger::baseLog(Level level, const std::string& message)
{
  std::string str_level = levelToStr(level);
  if (str_level.empty())
  {
    return 1;
  }
  if (level >= min_level_)
  {
    out_now_time(out_) << " " << str_level << " " << message << std::endl;
  }
  return 0;
}

int tecslog::info(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.info(message);
}

int tecslog::warning(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.warning(message);
}

int tecslog::error(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.error(message);
}