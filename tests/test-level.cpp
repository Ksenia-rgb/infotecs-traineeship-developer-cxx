#include <sstream>

#include <tecslog/tecslog.hpp>

#include "test-runner.hpp"

TEST_CASE(TecslogCorrectSetLevelTest)
{
  tecslog::init("test.log", tecslog::Level::WARNING);
  std::error_code code = tecslog::setLevel(tecslog::Level::INFO);
  return code.value() == 0;
}

TEST_CASE(TecslogCorrctSetSameLevelTest)
{
  tecslog::init("test.log", "ERROR");
  std::error_code code = tecslog::setLevel(tecslog::Level::ERROR);
  return code.value() == 0;
}

TEST_CASE(TecslogCorrectSetStrLevelTest)
{
  tecslog::init("test.log", "WARNING");
  std::error_code code = tecslog::setLevel("INFO");
  return code.value() == 0;
}

TEST_CASE(TecslogIncorrectSetStrLevelTest)
{
  tecslog::init("test.log", "WARNING");
  std::error_code code = tecslog::setLevel("INCORRECT");
  return code.value() != 0;
}

TEST_CASE(TecslogPrintPossibleLevelsTest)
{
  tecslog::init("test.log", tecslog::Level::WARNING);
  
  std::stringstream sout;
  tecslog::printPossibleLevels(sout);
  return sout.str() == "0 INFO\n1 WARNING\n2 ERROR";
}

TEST_CASE(TecslogCorrectParseLevelTest)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("ERROR");
  return level.has_value() && level.value() == tecslog::Level::ERROR;
}

TEST_CASE(TecslogIncorrectParseLevelTest)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("INCORRECT");
  return !level.has_value();
}


