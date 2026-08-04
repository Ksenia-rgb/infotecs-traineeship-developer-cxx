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
  std::unordered_map< std::string, std::function< std::error_code() > > command_map = {
    {"silence", std::bind(demo::silenceCommand, std::ref(std::cin), std::ref(std::cout), std::ref(info))},
    {"default", std::bind(demo::defaultCommand, std::ref(std::cin), std::ref(std::cout))},
    {"help", std::bind(demo::helpCommand, std::ref(std::cout))}
  };

  std::string buffer;
  while (std::cin >> std::quoted(buffer))
  {
    std::error_code code;
    auto command_iter = command_map.find(buffer);
    if (command_iter != command_map.end())
    {
      code = command_iter->second();
    }
    else
    {
      info.message = buffer;
      if (std::cin >> buffer)
      {
        info.level = buffer;
        queue.push(std::move(info));
      }
      else if (info.silence_level.has_value())
      {
        info.level = info.silence_level.value();
        queue.push(std::move(info));
      }
      else
      {
        std::cerr << "<INVALID COMMAND> specify log level or use silence command\n";
      }
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  queue.forceStop();
  logger.join();
}
