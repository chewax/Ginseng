// MIT License

// Copyright (c) [2020] [Daniel Waksman]

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __Ginseng_H
#define __Ginseng_H

#include "Terminal.h"
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define COL_WIDTH 15

using CmdCallback = std::function<int(std::vector<std::string>)>;

enum Exit
{
  SUCCESS = 0,
  INVALID_ARGUMENTS = 1,
  ERROR = 9
};

struct Help
{
  std::string description;
  std::string arguments;

  Help()
  {
  }

  Help(std::string t_desc, std::string t_args) : description(t_desc), arguments(t_args)
  {
  }
};

struct Command
{
  std::string name;
  CmdCallback cb;
  Help help;

  Command()
  {
  }

  Command(std::string t_name, CmdCallback t_cb, Help t_help) : name(t_name), cb(t_cb), help(t_help)
  {
  }
};

class Ginseng
{
private:
  std::string delimiter;
  std::map<std::string, Command> commands;
  void clear_screen();
  Command parse_command(std::string cmd_str);
  void print_help();
  void print_delimiter();
  void handle_error(int err);
  std::function<void()> greet_handler;
  std::function<void()> farewell_handler;
  std::string pad_left(const std::string &str, const size_t length, const char chr) const;
  std::string pad_right(const std::string &str, const size_t length, const char chr) const;
  std::vector<std::string> parse(std::string str);
  Terminal term;

public:
  void greet();
  void farewell();
  void add_command(std::string name, CmdCallback cb, Help help);
  void start();
  Ginseng(std::string delim, std::function<void()> greet, std::function<void()> farewell);

  template <typename... Args>
  void printf(const std::string &msg, Args... args);
  void println(const std::string &msg);
};

Ginseng::Ginseng(std::string delim, std::function<void()> greet, std::function<void()> farewell)
    : delimiter(delim), greet_handler(greet), farewell_handler(farewell)
{
}

// Safely prints into command feedback window row.
template <typename... Args>
void Ginseng::printf(const std::string &msg, Args... args)
{
  term.printf(msg, args...);
}

void Ginseng::println(const std::string &msg)
{
  term.println(msg);
}

void Ginseng::clear_screen()
{
  term.cls();
}

std::string Ginseng::pad_left(const std::string &str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.begin(), length - str.size(), chr);
  return ret;
}

std::string Ginseng::pad_right(const std::string &str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.end(), length - str.size(), chr);
  return ret;
}

void Ginseng::greet()
{
  term.println("Welcome!");
}

void Ginseng::farewell()
{
  term.println("Bye.");
}

void Ginseng::print_help()
{
  for (const auto &cmd : commands)
  {
    term.printf("%s %s %s\n", pad_left(cmd.second.name, COL_WIDTH, ' ').c_str(), pad_right(cmd.second.help.arguments, COL_WIDTH, ' ').c_str(),
                cmd.second.help.description.c_str());
  }

  term.printf("%s %s %s\n", pad_left("exit", COL_WIDTH, ' ').c_str(), pad_right("", COL_WIDTH, ' ').c_str(), "Terminates the execution");
}

void Ginseng::print_delimiter()
{
  term.printf("%s ", delimiter.c_str());
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
  case Exit::INVALID_ARGUMENTS:
    term.println("Invalid Arguments, type \"help\" for details.");
    break;

  default:
    break;
  }
}

void Ginseng::start()
{
  greet_handler();

  while (true)
  {

    std::string cmd_str = term.prompt(delimiter);
    term.println("");
    std::vector<std::string> args = parse(cmd_str);

    if (args[0] == std::string("exit") || args[0] == std::string("quit") || args[0] == std::string(":q"))
      break;

    if (args[0] == "help")
    {
      print_help();
      continue;
    }

    // If command is found insid the commands list.
    auto it = commands.find(args[0]);
    if (it != commands.end())
    {
      Command cmd = it->second;
      int res = cmd.cb(args);
      if (res != Exit::SUCCESS)
      {
        handle_error(res);
      }
    }
    else
    {
      term.println("Command not found.");
    }
  }

  farewell_handler();
}

void Ginseng::add_command(std::string name, CmdCallback cb, Help help)
{
  commands[name] = Command(name, cb, help);
}

#endif
