#include "io-commands.hpp"

#include <iomanip>
#include <sstream>

namespace
{
  std::error_code readWordTillEnd(std::istream& in, std::string& str)
  {
    std::string buffer;
    std::getline(in, buffer);
    std::stringstream sin(buffer);

    if (!(sin >> str))
    {
      return std::make_error_code(std::errc::io_error);
    }
    return {};
  }
}

std::error_code demo::silenceCommand(std::istream& in, LogInfo& info)
{
  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code)
  {
    return code;
  }

  auto level = tecslog::parseLevel(str_level);
  if (!level.has_value())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  LogInfo::silence_level = level;
  std::cout << "[OK] silence level is specified on level " << str_level << '\n';
  return {};
}

std::error_code demo::defaultCommand(std::istream& in)
{
  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code)
  {
    return code;
  }

  code = tecslog::setLevel(str_level);
  if (code)
  {
    return code;
  }
  std::cout << "[OK] default level is specified on level " << str_level << '\n';
  return {};
}

std::error_code demo::logCommand(std::istream& in, LogInfo& info, ThreadSafeQueue< LogInfo >& queue)
{
  in >> std::quoted(info.message);
  if (!in)
  {
    return std::make_error_code(std::errc::io_error);
  }

  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code)
  {
    return code;
  }

  auto level = tecslog::parseLevel(str_level); 
  if (level.has_value())
  {
    info.level = level.value();
  }
  else if (LogInfo::silence_level.has_value())
  {
    info.level = LogInfo::silence_level.value();
  }
  else
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  queue.push(info);
  std::cout << "[OK] log with message: \"" << info.message << "\" and level: " << str_level << '\n';
  return {};
}

std::error_code demo::helpCommand(std::ostream& out)
{
  out << "Commands:\n"
    << "\tsilence <level>\t= Specify silence log level\n"
    << "\tdefault <level>\t= Specify default log level, logs with a lower level will not be recorded\n"
    << "\tlog <message> <level>\t= Log message with level\n"
    << "\tThe <level> may not be specified if it was specified by the `silence` command.\n"
    << "\tIn such case log will have <level>, specified as `silence` <level>\n"
    << "\tIf you did not specify <level> and not use `silence`, command is invalid\n";
  return {};
}
