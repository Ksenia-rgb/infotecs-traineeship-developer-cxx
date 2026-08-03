#include "logger.hpp"

#include <ctime>
#include <iomanip>
#include <optional>
#include <unordered_map>

namespace
{
  std::ostream& out_now_time(std::ostream& out)
  {
    std::time_t now_c = std::time(nullptr);
    return out << std::put_time(std::localtime(&now_c), "%d.%m.%Y %H:%M:%S");
  }

  std::optional< std::string > levelToStr(tecslog::Level level)
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

  std::optional< tecslog::Level > strToLevel(const std::string& level)
  {
    static const std::unordered_map< std::string, tecslog::Level > level_map = {
      {"INFO", tecslog::Level::INFO},
      {"WARNING", tecslog::Level::WARNING},
      {"ERROR", tecslog::Level::ERROR}
    };

    auto level_iter = level_map.find(level);
    if (level_iter != level_map.end())
    {
      return level_iter->second;
    }
    return {};
  }
}

const std::string tecslog::Logger::default_filename_ = "tecslog.log";
const tecslog::Level tecslog::Logger::default_level_ = tecslog::Level::INFO;

tecslog::Logger::Logger():
  config_filename_(default_filename_),
  open_filename_(),
  level_(default_level_),
  out_(),
  mutex_file(),
  mutex_level()
{}

tecslog::Logger::~Logger()
{
  if (isFileOpen())
  {
    out_.flush();
    out_.close();
  }
}

tecslog::Logger& tecslog::Logger::instance()
{
  static Logger logger_instance;
  return logger_instance;
}

void tecslog::Logger::setLevel(Level level)
{
  if (level_ != level)
  {
    std::lock_guard< std::mutex > lock(mutex_level);
    level_ = level;
  }
}

void tecslog::Logger::setFile(const std::string& filename)
{
  if (config_filename_ != filename)
  {
    std::lock_guard< std::mutex > lock(mutex_file);
    config_filename_ = filename;
  }
}

bool tecslog::Logger::isFileOpen()
{
  return out_.is_open();
}

void tecslog::Logger::reset()
{
  std::lock_guard< std::mutex > lock_level(mutex_level);
  std::lock_guard< std::mutex > lock_file(mutex_file);
  config_filename_ = default_filename_;
  open_filename_ = "";
  level_ = default_level_;
  out_.close();
  out_ = std::ofstream();
}

std::error_code tecslog::Logger::info(const std::string& message)
{
  return log(Level::INFO, message);
}

std::error_code tecslog::Logger::warning(const std::string& message)
{
  return log(Level::WARNING, message);
}

std::error_code tecslog::Logger::error(const std::string& message)
{
  return log(Level::ERROR, message);
}

std::error_code tecslog::Logger::log(Level level, const std::string& message)
{
  std::optional< std::string > str_level = levelToStr(level);
  if (!str_level.has_value())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (level >= level_)
  {
    return uncheckLevelLog(str_level.value(), message);
  }
  return {};
}

std::error_code tecslog::Logger::log(const std::string& str_level, const std::string& message)
{
  std::optional< Level > level = strToLevel(str_level);
  if (!level.has_value())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (level.value() >= level_)
  {
    return uncheckLevelLog(str_level, message);
  }
  return {};
}

std::error_code tecslog::Logger::uncheckLevelLog(const std::string& str_level, const std::string& message)
{
  std::error_code file_code = ensureFileOpen();
  if (file_code)
  {
    return file_code;
  }

  std::lock_guard< std::mutex > lock(mutex_file);
  out_now_time(out_) << " [" << str_level << "] " << message << '\n';
  if (!out_)
  {
    return std::make_error_code(std::errc::io_error);
  }
  return {};
}

std::error_code tecslog::Logger::ensureFileOpen()
{
  if (out_.is_open() && config_filename_ == open_filename_)
  {
    return {};
  }
  if (!out_.is_open() && config_filename_ == open_filename_)
  {
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }
  out_.close();
  out_.open(config_filename_, std::ios::app);
  open_filename_ = config_filename_;
  return !out_.is_open() ? std::make_error_code(std::errc::no_such_file_or_directory) : std::error_code{};
}
