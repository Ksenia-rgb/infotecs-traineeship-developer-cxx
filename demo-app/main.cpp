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
  if (argc != 3)
  {
    std::cerr << "Incorrect programm argument count\n";
    return 1;
  }
  tecslog::init(argv[1], argv[2]);

  demo::ThreadSafeQueue< demo::LogInfo > queue;
  std::thread logger(logFromQueue, std::ref(queue));

  demo::LogInfo info;
  std::unordered_map< std::string, std::function< std::error_code() > > command_map;
  command_map["silence"] = std::bind(demo::silenceCommand, std::ref(std::cin), std::ref(info));
  command_map["default"] = std::bind(demo::defaultCommand, std::ref(std::cin));
  command_map["help"] = std::bind(demo::helpCommand, std::ref(std::cout));

  std::string buffer;
  while (std::cin >> std::quoted(buffer))
  {
    std::error_code code;
    if (auto command_iter = command_map.find(buffer); command_iter != command_map.end())
    {
      code = command_iter->second();
    }
    else
    {
      info.message = buffer;
      code = demo::logCommand(std::cin, info);
      if (!code)
      {
        queue.push(std::move(info));
      }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    if (code)
    {
      std::cerr << code.message() << '\n';
    }
  }

  queue.forceStop();
  logger.join();
}
