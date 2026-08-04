#include "io-commands.hpp"

std::error_code demo::silenceCommand(std::istream& in, LogInfo& info)
{
  std::string str_level;
  in >> str_level;
  if (in)
  {
    auto level = tecslog::parseLevel(str_level);
    if (!level.has_value())
    {
      return std::make_error_code(std::errc::invalid_argument);
    }
    LogInfo::silence_level = level;
  }
  return {};
}

std::error_code demo::defaultCommand(std::istream& in)
{
  std::string level;
  in >> level;
  if (in)
  {
    return tecslog::setLevel(level);
  }
  return std::make_error_code(std::errc::io_error);
}

std::error_code demo::helpCommand(std::ostream& out)
{
  out << "Commands:\n"
    << "\tsilence <level>\t=Specify silence log level\n"
    << "\tdefault <level>\t=Specify default log level, logs with a lower level will not be recorded\n"
    << "\nIf the input does not contain any of the described commands, the input is considered a log.\n"
    << "The log should contain <message> and <level>.\n"
    << "The level may not be specified if it was specified by the `silence` command.\n"
    << "In such case log will have level, specified as `silence` level\n"
    << "If you did not specify <level> and not use `silence`, command is invalid\n";
  return {};
}

std::error_code demo::logCommand(std::istream& in, LogInfo& info)
{
  std::string buffer;
  std::cin >> buffer;

  auto level = tecslog::parseLevel(buffer); 
  if (std::cin && level.has_value())
  {
    info.level = level.value();
  }
  else if (info.silence_level.has_value())
  {
    info.level = info.silence_level.value();
  }
  else
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  return {};
}
