#include <tecslog/tecslog.hpp>

#include "logger.hpp"

std::error_code tecslog::init(const std::string& filename, Level min_level)
{
  Logger& logger = Logger::instance();
  logger.setFile(filename);
  logger.setLevel(min_level);

  if (!logger.isFileOpen())
  {
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }
  return {};
}

void tecslog::setLevel(Level level)
{
  Logger& logger = Logger::instance();
  logger.setLevel(level);
}

std::error_code tecslog::info(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.info(message);
}

std::error_code tecslog::warning(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.warning(message);
}

std::error_code tecslog::error(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.error(message);
}