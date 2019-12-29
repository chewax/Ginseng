#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

using CmdCallback = std::function<int(std::vector<std::string>)>;

enum Exit
{
  SUCCESS=0,
  INVALID_ARGUMENTS=1,
  ERROR=9
};

struct Help
{
  std::string description;
  std::string arguments;

  Help()
  {
  }

  Help(std::string t_desc, std::string t_args): description(t_desc), arguments(t_args)
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

  Command(std::string t_name, CmdCallback t_cb, Help t_help) 
    : name(t_name), cb(t_cb), help(t_help)
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
  std::string pad_left(const std::string& str, const size_t length, const char chr) const;
  std::string pad_right(const std::string& str, const size_t length, const char chr) const;
  std::vector<std::string> parse(std::string str);

public:
  static void greet();
  static void farewell();
  void add_command(std::string name, CmdCallback cb, Help help);
  void start();
  Ginseng(std::string delim=">", std::function<void()> greet=Ginseng::greet, std::function<void()> farewell=Ginseng::farewell);
};