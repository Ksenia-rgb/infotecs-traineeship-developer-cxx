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
    std::string level;
    std::optional< std::string > silence_level;
  };
  std::istream& operator>>(std::istream& in, LogInfo& info);
}

#endif
