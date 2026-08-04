#ifndef IO_COMMANDS_HPP
#define IO_COMMANDS_HPP

#include <iostream>

#include "log-info.hpp"

namespace demo
{
  std::error_code silenceCommand(std::istream& in, std::ostream& out, LogInfo& info);
  std::error_code defaultCommand(std::istream& in, std::ostream& out);
  std::error_code helpCommand(std::ostream& out);
}

#endif
