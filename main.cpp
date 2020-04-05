
#include "Ginseng.h"
#include <iostream>

int main()
{
  // Ginseng repl("$test>");
  	Ginseng repl("$test>", []()
	{
		std::cout << "\nWELCOME TO MY AWESOME REPL!" << "\n";
		std::cout << "Type \"help\" to start" << "\n";
		std::cout << "" << "\n";
		std::cout << "HAVE FUN!" << "\n";
	},
	[]()
	{
		std::cout << "\nBye My friend!" << "\n";
	});
  // Ginseng repl;
  
  Help hello_h("Says hello back at you", "[name]");
  repl.add_command("hello", [](std::vector<std::string> args) -> int
  { 
    if (args.size() < 2) return Exit::INVALID_ARGUMENTS;
    std::cout << "HELLO " << args[1] << std::endl; 
    return Exit::SUCCESS;
  }, hello_h);

  Help goodbye_h("Says goodbye!", "[]");
  repl.add_command("goodbye", [](std::vector<std::string> args) -> int
  { 
    std::cout << "GOODBYE!" << std::endl; 
    return Exit::SUCCESS;
  }, goodbye_h);

  repl.start();
  
  return 0;
}