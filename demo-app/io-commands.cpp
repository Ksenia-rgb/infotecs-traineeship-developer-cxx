#include "io-commands.hpp"

#include <tecslog/tecslog.hpp>

std::error_code demo::silenceCommand(std::istream& in, std::ostream& out, LogInfo& info)
{
  std::string level;
  in >> level;
  if (in)
  {
    info.silence_level = level;
  }
  return {};
}

std::error_code demo::defaultCommand(std::istream& in, std::ostream& out)
{
  std::string level;
  in >> level;
  if (in)
  {
    tecslog::setLevel(level);
  }
  return {};
}

std::error_code demo::helpCommand(std::ostream& out)
{
  out << "Commands:\n"
    << "\tsilence <level>\t=Specify silence log level\n"
    << "\tdefault <level>\t=Specify default log level, logs with a lower level will not be recorded\n"
    << "\nIf the input does not contain any of the described commands, the input is considered a log.\n"
    << "The log should contain <message> and <level>.\n"
    << "The level may not be specified if it was specified by the silence command.\n"
    << "In such case log will have level, specified as silence level\n";
  return {};
}
