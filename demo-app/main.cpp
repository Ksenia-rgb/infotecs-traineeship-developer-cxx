#include <iomanip>
#include <iostream>
#include <functional>
#include <system_error>
#include <thread>
#include <unordered_map>

#include <tecslog/tecslog.hpp>

#include "io-commands.hpp"
#include "log-info.hpp"
#include "thread-safe-queue.hpp"

namespace
{
  void logFromQueue(demo::ThreadSafeQueue< demo::LogInfo >& queue)
  {
    while (!queue.isForceStopped())
    {
      demo::LogInfo* log_info = queue.front();
      if (log_info)
      {
        queue.pop();
        tecslog::log(log_info->level, log_info->message);
      }
    }
  }
}

int main(int argc, char** argv)
{
  using namespace std::placeholders;

  if (argc != 3)
  {
    std::cerr << "Incorrect programm argument count\n";
    return 1;
  }
  tecslog::init(argv[1], argv[2]);

  demo::ThreadSafeQueue< demo::LogInfo > queue;
  std::thread logger(logFromQueue, std::ref(queue));

  demo::LogInfo info;
  std::unordered_map< std::string, std::function< std::error_code(std::istream&, std::ostream&) > > command_map;
  command_map["silence"] = std::bind(demo::silenceCommand, _1, _2, std::ref(info));
  command_map["default"] = std::bind(demo::defaultCommand, _1, _2);
  command_map["log"] = std::bind(demo::logCommand, _1, _2, std::ref(info), std::ref(queue));
  command_map["help"] = std::bind(demo::helpCommand, _1, _2);

  std::istream& in = std::cin;
  std::ostream& out = std::cout;

  std::string command;
  while (in >> command)
  {
    auto command_iter = command_map.find(command);
    if (command_iter != command_map.end())
    {
      std::error_code code = command_iter->second(in, out);
      if (code)
      {
        std::cerr << code.message() << '\n';
      }
    }
    else
    {
      std::cerr << "[INVALID COMMAND]\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  queue.forceStop();
  logger.join();
}
