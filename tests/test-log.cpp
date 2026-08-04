#include "test-runner.hpp"

#include <fstream>
#include <sstream>

#include <tecslog/tecslog.hpp>

TEST_CASE(TestInfoLogWithCorrectFile)
{
  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::error("Error mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();
  return code.value() == 0 && sout.str().find("[ERROR] Error mess") != std::string::npos;
}

TEST_CASE(TestInfoLogWithIncorrectFile)
{
  std::string filename = "folder/unexist/test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::error("Error mess");

  return code.value() != 0;
}

TEST_CASE(TestLogWithLowerLevel)
{
  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::error("Error mess");
  code = tecslog::info("First info mess");
  code = tecslog::setLevel(tecslog::Level::INFO);
  code = tecslog::info("Second info mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  bool error_found = sout.str().find("[ERROR] Error mess") != std::string::npos;
  bool info_found = sout.str().find("[INFO] First info mess") != std::string::npos;
  bool info2_found = sout.str().find("[INFO] Second info mess") != std::string::npos;

  return code.value() == 0 && error_found && !info_found && info2_found;
}

TEST_CASE(TestLogWithInorrectStrLevel)
{
  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::log("BOOL", "Info mess");
  code = tecslog::log("INCORRECT", "Warn mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  return code.value() != 0 && sout.str().empty();
}
