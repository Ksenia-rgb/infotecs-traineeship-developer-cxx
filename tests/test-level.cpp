#include "test-runner.hpp"

#include <sstream>

#include <tecslog/tecslog.hpp>

TEST_CASE(TestCorrectSetLevel)
{
  tecslog::init("logs/moday.log", tecslog::Level::WARNING);
  std::error_code code = tecslog::setLevel(tecslog::Level::INFO);
  return code.value() == 0;
}

TEST_CASE(TestCorrectSetStrLevel)
{
  tecslog::init("logs/moday.log", "WARNING");
  std::error_code code = tecslog::setLevel("INFO");
  return code.value() == 0;
}

TEST_CASE(TestIncorrectSetStrLevel)
{
  tecslog::init("logs/moday.log", "WARNING");
  std::error_code code = tecslog::setLevel("INCORRECT");
  return code.value() != 0;
}

TEST_CASE(TestPrintPossibleLevels)
{
  tecslog::init("logs/moday.log", tecslog::Level::WARNING);
  
  std::stringstream sout;
  tecslog::printPossibleLevels(sout);
  return sout.str() == "0 INFO\n1 WARNING\n2 ERROR";
}

TEST_CASE(TestCorrectParseLevel)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("ERROR");
  return level.has_value() && level.value() == tecslog::Level::ERROR;
}

TEST_CASE(TestIncorrectParseLevel)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("INCORRECT");
  return !level.has_value();
}


