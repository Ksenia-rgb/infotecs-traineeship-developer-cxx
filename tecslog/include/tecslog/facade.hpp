#ifndef TECSLOG_FACADE_HPP
#define TECSLOG_FACADE_HPP

#include <string>
#include <system_error>

#include "types.hpp"

namespace tecslog
{
  void init(const std::string& filename, Level level);
  void setLevel(Level level);
  std::error_code info(const std::string& message);
  std::error_code warning(const std::string& message);
  std::error_code error(const std::string& message);
}

#endif
