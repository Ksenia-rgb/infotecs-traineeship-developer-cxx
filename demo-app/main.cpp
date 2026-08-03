#include <tecslog/tecslog.hpp>

int main()
{
  tecslog::init("resources/monday.log", tecslog::Level::INFO);
  tecslog::info("lib check message");
}
