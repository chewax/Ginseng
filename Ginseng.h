#ifndef __Ginseng_H
#define __Ginseng_H

#include "Terminal.h"
#include <functional>
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

/**
 * @struct Help
 * Struct intended to hold a command's help information
 **/
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

/**
 * @struct Command
 * Struct to hold all the information related with a command.
 */
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

/**
 * @class Ginseng
 * This class handles all command specifict functions
 * Intended to provide a simplified interface to buidl and mantain a REPL tool
 **/
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

/**
 * Prints a formated message into current cursor position.
 * @param std::string text with format
 * @param multiple formatting arguments
 **/
template <typename... Args>
void Ginseng::printf(const std::string &msg, Args... args)
{
  term.printf(msg, args...);
}

/**
 * Helper function to clear screen
 **/
void Ginseng::println(const std::string &msg)
{
  term.println(msg);
}

/**
 * Helper function to clear screen
 * OS independent.
 **/
void Ginseng::clear_screen()
{
  term.cls();
}

/**
 * Helper function to pad string to the left
 * @param std::string string to be padded
 * @param size_t final lenght of the string
 * @param char char to pad with
 * @return std::string padded string.
 **/
std::string Ginseng::pad_left(const std::string &str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.begin(), length - str.size(), chr);
  return ret;
}

/**
 * Helper function to pad string to the right
 * @param std::string string to be padded
 * @param size_t final lenght of the string
 * @param char char to pad with
 * @return std::string padded string.
 **/
std::string Ginseng::pad_right(const std::string &str, const size_t length, const char chr) const
{
  std::string ret(str);
  ret.insert(ret.end(), length - str.size(), chr);
  return ret;
}

/**
 * Helper function to print help for both preshipped functions and user defined commands
 **/
void Ginseng::print_help()
{
  term.println("");
  term.printf("%s %s %s\n", pad_right("Commmand", COL_WIDTH, ' ').c_str(), pad_right("Args", COL_WIDTH, ' ').c_str(), "Description");
  term.println(pad_right("", COL_WIDTH * 3, '-').c_str());

  for (const auto &cmd : commands)
  {
    term.printf("%s %s %s\n", pad_right(cmd.second.name, COL_WIDTH, ' ').c_str(), pad_right(cmd.second.help.arguments, COL_WIDTH, ' ').c_str(),
                cmd.second.help.description.c_str());
  }

  term.printf("%s %s %s\n", pad_right("exit", COL_WIDTH, ' ').c_str(), pad_right("", COL_WIDTH, ' ').c_str(), "Terminates the execution");
  term.printf("%s %s %s\n", pad_right("cls", COL_WIDTH, ' ').c_str(), pad_right("", COL_WIDTH, ' ').c_str(), "Clears the screen");
  term.println(pad_right("", COL_WIDTH * 3, '-').c_str());
}

/**
 * Helper function to print the defined delimiter at current cursor position.
 */
void Ginseng::print_delimiter()
{
  term.printf("%s ", delimiter.c_str());
}

/**
 * Will split a string. Every token is considered to end at a " " (space).
 * @param std::string string to be tokenized
 * @return std::vector<std::string> token array
 */
std::vector<std::string> Ginseng::parse(std::string str)
{
  std::vector<std::string> tokens;
  std::istringstream src(str);
  std::string tok;

  while (std::getline(src, tok, ' '))
  {
    tokens.push_back(tok);
  }

  return tokens;
}

/**
 * Handle execution ERROR
 * @param int Error code. See enum Exit.
 */
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

/**
 *  Starts the execution of the REPL.
 *  Thought to be called independently to allow configurations prior to launch
 */
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

    if (args[0] == "help" || args[0] == "?" || args[0] == ":h")
    {
      print_help();
      continue;
    }

    if (args[0] == "cls")
    {
      term.cls();
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

/**
 * Adds a command to the command pool
 * @param std::string identifier to be used for invoking the command.
 * @param std::function<int(std::vector<std::string>)> handler to be called when the command is invoked
 * @param Help struct containing help for given command.
 **/
void Ginseng::add_command(std::string name, CmdCallback cb, Help help)
{
  commands[name] = Command(name, cb, help);
}

#endif
