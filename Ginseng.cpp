#include "Ginseng.h"
#include <iostream>
#include <string>
#include <sstream>

Ginseng::Ginseng(std::string t_delim) : delimiter(t_delim)
{

}

void Ginseng::clear_screen()
{
  std::printf("\033c");
}

void Ginseng::print_help()
{
  for (auto cmd : commands)
  {
    std::cout << "\t" << cmd.second.name << "\t" << cmd.second.description << "\n";
  }

  std::cout << "\t"
            << "exit \t Terminates the execution" << std::endl;
}

void Ginseng::print_delimiter()
{
  std::cout << "\033[1;33m" << delimiter << " \033[0m";
}


std::vector<std::string> Ginseng::parse(std::string str)
{
  std::vector<std::string> strings;
  std::istringstream src(str);
  std::string s;    

  while (std::getline(src, s, ' ')) {
      strings.push_back(s);
  }

  return strings;
}


void Ginseng::start()
{
  clear_screen();

  while (true)
  {
    print_delimiter();
    std::string cmd_str;
    std::getline(std::cin, cmd_str);
    std::vector<std::string> args = parse(cmd_str);
    
    if (args[0] == std::string("exit") || args[0] == std::string("quit"))
      break;
    
    if (args[0] == "help") 
    {
      std::cout << std::endl;
      print_help();
      continue;
    }

    if (commands.find(args[0]) != commands.end())
    {
      std::cout << std::endl;
      Command cmd = commands.at(args[0]);
      cmd.cb(args);
    }
    else
    {
      std::cout << "Command not found." << std::endl;
    }
  }
}


void Ginseng::add_command(std::string name, std::function<void(std::vector<std::string>)> cb, std::string description)
{
  commands[name] = Command(name, cb, description);
}