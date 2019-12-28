#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

struct Command
{
  std::string name;
  std::function<void(std::vector<std::string>)> cb;
  std::string description;

  Command()
  {
  }

  Command(std::string t_name, std::function<void(std::vector<std::string>)> t_cb, std::string t_description) 
    : name(t_name), cb(t_cb), description(t_description)
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
  std::vector<std::string> parse(std::string str);

public:
  void add_command(std::string name, std::function<void(std::vector<std::string>)> cb, std::string description);
  void start();
  Ginseng(std::string t_delim="");
};