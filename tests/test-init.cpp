#include <tecslog/tecslog.hpp>

#include "test-runner.hpp"

TEST_CASE(TecslogСorrectInitTest)
{
  std::error_code code = tecslog::init("test.log", tecslog::Level::WARNING);
  return code.value() == 0;
}

TEST_CASE(TecslogCorrectInitWithStrLevelTest)
{
  std::error_code code = tecslog::init("test.log", "INFO");
  return code.value() == 0;
}

TEST_CASE(TecslogIncorrectInitWithStrLevelTest)
{
  std::error_code code = tecslog::init("test.log", "INCORRECT");
  return code.value() != 0;
}

TEST_CASE(TecslogIncorrectInitWithNonExistedFileTest)
{
  std::error_code code = tecslog::init("logs/unreal-directory/test.log", tecslog::Level::ERROR);
  return code.value() != 0;
}
