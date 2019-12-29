
#include "Ginseng.h"
#include <iostream>

int main()
{
  // Ginseng cli("$test>");
  Ginseng cli("$test>", [](){
    std::cout << "WELCOME TO MY AWESOME CLI!" << "\n";
    std::cout << "Type \"help\" to start" << "\n";
    std::cout << "" << "\n";
    std::cout << "HAVE FUN!" << "\n";

  });
  // Ginseng cli;
  
  Help hello_h("Says hello back at you", "[name]");
  cli.add_command("hello", [](std::vector<std::string> args) -> int
  { 
    if (args.size() < 2) return Exit::INVALID_ARGUMENTS;
    std::cout << "HELLO " << args[1] << std::endl; 
    return Exit::SUCCESS;
  }, hello_h);

  Help goodbye_h("Says goodbye!", "[]");
  cli.add_command("goodbye", [](std::vector<std::string> args) -> int
  { 
    std::cout << "GOODBYE!" << std::endl; 
    return Exit::SUCCESS;
  }, goodbye_h);

  cli.start();
  
  return 0;
}