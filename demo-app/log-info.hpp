#ifndef LOG_INFO_HPP
#define LOG_INFO_HPP

#include <iostream>
#include <string>

namespace demo
{
  struct LogInfo
  {
    std::string message;
    std::string level;
    std::string default_level;

    LogInfo(const std::string& default_level_new);
  };
  std::istream& operator>>(std::istream& in, LogInfo& info);
}

#endif
