#include "log-info.hpp"

#include <iomanip>

demo::LogInfo::LogInfo(const std::string& default_level_new):
  message(),
  level(),
  default_level(default_level_new)
{}

std::istream& demo::operator>>(std::istream& in, LogInfo& info)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> std::quoted(info.message);
  if (!in)
  {
    return in;
  }

  in >> info.level;
  if (!in)
  {
    info.level = info.default_level;
    in.clear();
  }
  return in;
}

