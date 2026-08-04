#include "test-runner.hpp"

#include <tecslog/tecslog.hpp>

TEST_CASE(TestIncorrectInit)
{
  std::error_code code = tecslog::init("logs/moday.log", tecslog::Level::WARNING);
  return code.value() == 0;
}

TEST_CASE(TestCorrectInitWithStrLevel)
{
  std::error_code code = tecslog::init("logs/moday.log", "INFO");
  return code.value() == 0;
}

TEST_CASE(TestIncorrectInitWithStrLevel)
{
  std::error_code code = tecslog::init("logs/moday.log", "INCORRECT");
  return code.value() != 0;
}



int main()
{
  return test::runAllTests();
}
