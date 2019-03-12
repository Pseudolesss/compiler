#include <iostream>
#include "driver.hh"

int
main (int argc, char *argv[])
{
  int res = 0;
  driver drv;
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string("-parse")) {
      drv.setting = 1;
      continue;
    }
    if (argv[i] == std::string("-lex")) {
      drv.setting = 0;
      continue;
    }
    if (argv[i] == std::string("-p"))
      drv.trace_parsing = true;
    else if (argv[i] == std::string("-s"))
      drv.trace_scanning = true;
    else if (true)
        int res = drv.parse(argv[i]);
        if(drv.setting == 1)
        std::cout << drv.root->accept(new Visitor()) << '\n';
  }
  return res;
}
