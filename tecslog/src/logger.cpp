#include "logger.hpp"

#include <ctime>
#include <iomanip>
#include <unordered_map>

namespace
{
  std::ostream& out_now_time(std::ostream& out)
  {
    std::time_t now_c = std::time(nullptr);
    return out << std::put_time(std::localtime(&now_c), "%d.%m.%Y %H:%M:%S");
  }

  std::string levelToStr(tecslog::Level level)
  {
    static const std::unordered_map< tecslog::Level, std::string > level_map = {
      {tecslog::Level::INFO, "INFO"},
      {tecslog::Level::WARNING, "WARNING"},
      {tecslog::Level::ERROR, "ERROR"}
    };

    auto level_iter = level_map.find(level);
    if (level_iter != level_map.end())
    {
      return level_iter->second;
    }
    return {};
  }
}

const std::string tecslog::Logger::default_filename_ = "tecslog.txt";
const tecslog::Level tecslog::Logger::default_level_ = tecslog::Level::INFO;

tecslog::Logger::Logger():
  filename_(default_filename_),
  level_(default_level_),
  out_(filename_),
  mutex_level(),
  mutex_file()
{}

tecslog::Logger& tecslog::Logger::instance()
{
  static Logger logger_instance;
  return logger_instance;
}

void tecslog::Logger::setLevel(Level level)
{
  std::lock_guard< std::mutex > lock(mutex_level);
  level_ = level;
}

void tecslog::Logger::setFile(const std::string& filename)
{
  if (filename_ != filename)
  {
    std::lock_guard< std::mutex > lock(mutex_file);
    filename_ = filename;
    out_.open(filename);
  }
}

void tecslog::Logger::reset()
{
  std::lock_guard< std::mutex > lock_level(mutex_level);
  std::lock_guard< std::mutex > lock_file(mutex_file);
  filename_ = default_filename_;
  level_ = default_level_;
}

std::error_code tecslog::Logger::info(const std::string& message)
{
  return baseLog(Level::INFO, message);
}

std::error_code tecslog::Logger::warning(const std::string& message)
{
  return baseLog(Level::WARNING, message);
}

std::error_code tecslog::Logger::error(const std::string& message)
{
  return baseLog(Level::ERROR, message);
}

std::error_code tecslog::Logger::baseLog(Level level, const std::string& message)
{
  std::lock_guard< std::mutex > lock(mutex_file);

  if (!out_.is_open())
  {
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }

  std::string str_level = levelToStr(level);
  if (str_level.empty())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (level >= level_)
  {
    out_now_time(out_) << " " << str_level << " " << message << std::endl;
  }
  return {};
}