#ifndef LOG_INFO_HPP
#define LOG_INFO_HPP

#include <iostream>
#include <optional>
#include <string>

namespace demo
{
  struct LogInfo
  {
    std::string message;
    tecslog::Level level;
    static std::optional< tecslog::Level > silence_level;
  };
}

#endif
