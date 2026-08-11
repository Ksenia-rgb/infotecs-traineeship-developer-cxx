#include "test-runner.hpp"

#include <tecslog/tecslog.hpp>

TEST_CASE(TecslogСorrectInitTest)
{
  std::error_code code = tecslog::init("moday.log", tecslog::Level::WARNING);
  return code.value() == 0;
}

TEST_CASE(TecslogCorrectInitWithStrLevelTest)
{
  std::error_code code = tecslog::init("moday.log", "INFO");
  return code.value() == 0;
}

TEST_CASE(TecslogIncorrectInitWithStrLevelTest)
{
  std::error_code code = tecslog::init("moday.log", "INCORRECT");
  return code.value() != 0;
}

TEST_CASE(TecslogIncorrectInitWithNonExistedFileTest)
{
  std::error_code code = tecslog::init("logs/unreal-directory/moday.log", tecslog::Level::ERROR);
  return code.value() != 0;
}


int main()
{
  return test::runAllTests();
}
