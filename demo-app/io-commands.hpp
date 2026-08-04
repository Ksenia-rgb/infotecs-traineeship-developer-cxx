#ifndef IO_COMMANDS_HPP
#define IO_COMMANDS_HPP

#include <iostream>

#include <tecslog/tecslog.hpp>

#include "log-info.hpp"
#include "thread-safe-queue.hpp"

namespace demo
{
  std::error_code silenceCommand(std::istream& in, LogInfo& info);
  std::error_code defaultCommand(std::istream& in);
  std::error_code logCommand(std::istream& in, LogInfo& info, ThreadSafeQueue< LogInfo >& queue);
  std::error_code helpCommand(std::ostream& out);
}

#endif
