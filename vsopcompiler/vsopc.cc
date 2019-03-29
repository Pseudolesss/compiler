#include <iostream>
#include "driver.hh"
#include "FillPrototype.hh"
#include "prototype.hh"
int
main (int argc, char *argv[])
{
  driver drv;

  if(argc != 3){
      std::cerr << "Wrong number of arguments" << std::endl;
      return 1;
  }

  if(argv[1] == std::string("-lex")){
      drv.setting = 0;
      drv.parse(argv[2]);
      return 0;
  }
  else if(argv[1] == std::string("-parse")){
      drv.setting = 1;
      int ret = drv.parse(argv[2]);
      if(ret == 0){
        drv.root->accept(new FillPrototype());
        for(auto elem : prototype){
          std::cout<< elem.first<< ":" << elem.second.toString() << std::endl;
        }
      }
      return ret;
  }
  else{
      std::cerr << "Non valid option argument" << std::endl;
      return 1;
  }
}
