#include "Ginseng.h"
#include <iostream>
#include <sstream>
#include <string>

#define COL_WIDTH 15

Ginseng::Ginseng(std::string delim, std::function<void()> greet, std::function<void()> farewell) 
  : delimiter(delim), greet_handler(greet), farewell_handler(farewell)
{
}

void Ginseng::clear_screen()
{
  std::printf("\033c");
}

std::string Ginseng::pad_left(const std::string& str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.begin(), length - str.size(), chr);
  return ret;
}

std::string Ginseng::pad_right(const std::string& str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.end(), length - str.size(), chr);
  return ret;
}

void Ginseng::greet()
{
  std::cout << "Welcome!" << std::endl;
}

void Ginseng::farewell()
{
  std::cout << "Bye." << std::endl;
}

void Ginseng::print_help()
{
  for (auto cmd : commands)
  {
    std::cout << pad_left(cmd.second.name, COL_WIDTH, ' ') 
              << " " 
              << pad_right(cmd.second.help.arguments, COL_WIDTH, ' ') 
              << " " 
              << cmd.second.help.description
              << "\n";
  }

  std::cout << pad_left("exit", COL_WIDTH, ' ') 
              << " " 
              << pad_right("", COL_WIDTH, ' ') 
              << " " 
              << "Terminates the execution"
              << std::endl;
}

void Ginseng::print_delimiter()
{
  std::cout << delimiter << " ";
}


std::vector<std::string> Ginseng::parse(std::string str)
{
  std::vector<std::string> strings;
  std::istringstream src(str);
  std::string s;

  while (std::getline(src, s, ' '))
  {
    strings.push_back(s);
  }

  return strings;
}

void Ginseng::handle_error(int err)
{
  switch (err)
  {
  case Exit::INVALID_ARGUMENTS :
    std::cout << "Invalid Arguments, type \"help\" for details." << std::endl;
    break;
  
  default:
    break;
  }
}


void Ginseng::start()
{
  clear_screen();
  greet_handler();

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
      int res = cmd.cb(args);
      if (res != Exit::SUCCESS) 
      {
        handle_error(res);
      }
    }
    else
    {
      std::cout << "Command not found." << std::endl;
    }
  }

  farewell_handler();
}


void Ginseng::add_command(std::string name, CmdCallback cb, Help help)
{
  commands[name] = Command(name, cb, help);
}