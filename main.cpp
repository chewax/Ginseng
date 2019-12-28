
#include "Ginseng.h"
#include <iostream>

int main()
{
  Ginseng cli("$test>");
  
  cli.add_command("hello", [](std::vector<std::string> args){ 

    for(auto a: args) 
    {
      std::cout << a << ", ";
    }
    std::cout << "\n";

    std::cout << "HELLO TO YOU TOO SIR!" << std::endl; 

  }, "Says hello back at you");

  cli.start();
  
  return 0;
}