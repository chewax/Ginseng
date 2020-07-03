#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>

class Terminal
{
public:
  template <typename... Args>
  void printf(const std::string &msg, Args... args);
  void println(const std::string &msg);
  void cls(); // clear screen;
  std::string prompt(const std::string delimiter);

  Terminal();
  ~Terminal();

private:
  void init_screen();
  void destroy_screen();
  void refresh_line(const std::string &new_value, const std::string delimiter);
  void end_command(const std::string &command);
  void cr(); // carriage return;
  std::string delimiter = "> ";

  std::vector<std::string> history;
  std::vector<std::string>::iterator it;

  WINDOW *_window;
};

Terminal::Terminal()
{
  init_screen();
}

Terminal::~Terminal()
{
  destroy_screen();
}

// Clears screen.
void Terminal::cls()
{
  wmove(_window, 0, 0);
  wclrtobot(_window);
}

// Refresh line with parameter string.
void Terminal::refresh_line(const std::string &new_value, const std::string delimiter)
{
  int x;
  int y;
  getyx(_window, y, x);
  wmove(_window, y, 0);
  wclrtoeol(_window);
  wprintw(_window, "%s %s", delimiter.c_str(), new_value.c_str());
  wrefresh(_window);
}

// Adds a Carriage Return
void Terminal::cr()
{
  wprintw(_window, "\n");
  wrefresh(_window);
}

// Refresh line with parameter string.
void Terminal::end_command(const std::string &command)
{
  if (command == "" || command == "...")
    return;
  history.push_back(command);
  it = history.end();
}

// Safely prints into command feedback window row.
template <typename... Args>
void Terminal::printf(const std::string &msg, Args... args)
{
  wprintw(_window, msg.c_str(), args...);
  wrefresh(_window);
}

void Terminal::println(const std::string &msg)
{
  wprintw(_window, msg.c_str());
  cr();
}

std::string Terminal::prompt(const std::string delimiter = ">")
{
  cr();
  refresh_line("", delimiter);
  int c = wgetch(_window);
  std::string command;
  bool eoc = false;

  while (!eoc) //!= CR
  {
    switch (c)
    {

    case KEY_UP:
      // Handle key up press
      // Retrieves previous from history
      {
        if (it > history.begin())
          it--;
        ;
        command = (*it);
        break;
      }

    case KEY_DOWN:
      // Handles key down press
      // Retrieves following command from history
      {
        if (it < history.end())
          it++;
        if (it == history.end())
          command = "";
        else
          command = (*it);
        break;
      }

    case 127:
      // Handles backspace press
      // Removes last charater from buffered command
      {
        command.pop_back();
        break;
      }

    case 10:
      // Return key
      {
        eoc = command.size() > 0;
        break;
      }

    default:
      // Handles non special characters
      // Appends character to buffered command
      {
        if (c != -1)
          command += c; // Input in current loop (-1 == ERR)
        break;
      }
    }
    refresh_line(command, delimiter);
    c = wgetch(_window);
  }

  end_command(command);

  if (command == "" || command == "...")
    return "";
  return command;
}

void Terminal::init_screen()
{
  // NCURSES init
  initscr();
  cbreak(); // break with ctrl + c
  noecho(); // dont echo inputs

  int height, width, start_y, start_x;
  getmaxyx(stdscr, height, width);

  start_y = 0;
  start_x = 0;

  _window = newwin(height, width, start_y, start_x);

  keypad(_window, true);
  nodelay(_window, true);
  scrollok(_window, TRUE);

  history.push_back("...");
  it = history.end();

  wclrtobot(_window);
}

void Terminal::destroy_screen()
{
  // deallocates memory and ends ncurses
  endwin();
}
