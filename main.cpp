
#include "Ginseng.h"

int main()
{
  Ginseng repl(
      "$test>",
      [&repl]() {
        repl.println("WELCOME TO MY AWESOME RELP!");
        repl.println("Type \"help\" to start");
        repl.println("");
        repl.println("HAVE FUN!");
      },
      [&repl]() {
        repl.println("BYE!");
      });

  // Create Help struct for hello command
  Help hello_h("Says hello back at you", "[name]");

  // Add command to the REPL
  repl.add_command(
      "hello",
      [&repl](std::vector<std::string> args) -> int {
        if (args.size() < 2)
          return Exit::INVALID_ARGUMENTS;
        repl.printf("HELLO %s\n", args[1].c_str());
        return Exit::SUCCESS;
      },
      hello_h);

  repl.start();
  return 0;
}