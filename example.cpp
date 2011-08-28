#include "reactor.hpp"
#include <iostream>

void print_hello_world(){
  std::cout << "Hello World" << std::endl;
  Reactor *reactor = Reactor::getInstance();
  reactor->stop();
}

int main(int argc, char** argv)
{
  Reactor *reactor = Reactor::getInstance();
  reactor->callLater(0, &print_hello_world);
  reactor->run();
  return 0;
}
