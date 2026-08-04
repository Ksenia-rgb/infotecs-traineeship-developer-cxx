#ifndef IO_COMMANDS_HPP
#define IO_COMMANDS_HPP

#include <iostream>

#include <tecslog/tecslog.hpp>

#include "log-info.hpp"

namespace demo
{
  std::error_code silenceCommand(std::istream& in, LogInfo& info);
  std::error_code defaultCommand(std::istream& in);
  std::error_code helpCommand(std::ostream& out);
  std::error_code logCommand(std::istream& in, LogInfo& info);
}

#endif
