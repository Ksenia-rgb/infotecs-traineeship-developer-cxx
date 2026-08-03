#include <tecslog/tecslog.hpp>

int main()
{
  tecslog::init("resources/log.txt", tecslog::Level::INFO);
  tecslog::info("lib check message");
}
