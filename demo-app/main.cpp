#include <atomic>
#include <iostream>
#include <functional>
#include <thread>

#include <tecslog/tecslog.hpp>

#include "thread-safe-queue.hpp"
#include "log-info.hpp"

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

  demo::LogInfo info("INFO");
  while (std::cin >> info)
  {
    queue.push(std::move(info));
  }
  queue.forceStop();
  logger.join();
}
