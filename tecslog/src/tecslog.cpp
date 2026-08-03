#include <tecslog/tecslog.hpp>

#include "logger.hpp"

void tecslog::init(const std::string& filename, Level level)
{
  Logger& logger = Logger::instance();
  logger.setFile(filename);
  logger.setLevel(level);
}

void tecslog::init(const std::string& filename, const std::string& level)
{
  Logger& logger = Logger::instance();
  logger.setFile(filename);
  logger.setLevel(level);
}

void tecslog::setLevel(Level level)
{
  Logger& logger = Logger::instance();
  logger.setLevel(level);
}

void tecslog::setLevel(const std::string& level)
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

std::error_code tecslog::log(Level level, const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.log(level, message);
}

std::error_code tecslog::log(const std::string& level, const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.log(level, message);
}
