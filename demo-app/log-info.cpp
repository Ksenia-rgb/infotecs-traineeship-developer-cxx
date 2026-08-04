#include "log-info.hpp"

#include <iomanip>

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
  if (in.fail() && !in.eof() && info.silence_level.has_value())
  {
    info.level = info.silence_level.value();
    in.clear();
  }
  return in;
}

