#include <iostream>
#include <unistd.h>
#include "driver.hh"
#include "FillPrototype.hh"
#include "CheckPrototype.hh"
#include "CheckTypeScope.hh"
#include "CheckPrinter.hh"
#include "prototype.hh"
#include "ParserPrinter.hh"
#include "SymbolTable.hh"

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
      drv.file = argv[2];
      int ret = drv.parse(argv[2]);
      return ret;
  }
  else if(argv[1] == std::string("-check")){
      drv.setting = 1;
      drv.parse("/home/IO.vsop");
      drv.root->accept(new FillPrototype());
      //Something to deallocate all the tree.
      
      int ret = drv.parse(argv[2]);
      if(ret == 0){
        drv.root->accept(new FillPrototype());
        check(drv.file);
        drv.root->accept(new CheckTypeScope());
        std::cout << drv.root->accept(new CheckPrinter()) << std::endl;
        ::errors.print(drv.file);
        
        /*std::cout<<"end of error printing" << std::endl;
        for(auto elem : prototype){
            std::cout<< elem.first<< ":" << elem.second.toString() << std::endl;
          }
        //to test the symbol table
        // SymbolTable s = SymbolTable();
        // s.add_element("one", "string");
        // std::cout << s.lookup("one") << '\n';
        // s.new_scope();
        // s.add_element("two", "int64");
        // std::cout << s.lookup("two") << '\n';
        // s.exit_scope();
        // std::cout << s.lookup("two") << '\n';
        // s.add_element("one", "int64");
        // s.new_scope();
        // std::cout << s.lookup("one") << '\n';*/
        
        //cout<<::errors.isempty()<<"\n";

        return !::errors.isempty();        
      }

    }  
    else if (argv[1] == std::string("-parse")){
      drv.setting = 1;
      int ret = drv.parse(argv[2]);
      std::cout << drv.root->accept(new ParserPrinter()) << std::endl;
      return ret;
    }
    else if (argv[1] == std::string("-llvm")){

      drv.setting = 1;

      TheModule = llvm::make_unique<llvm::Module>("TODOPutCorrectFileName", TheContext);


      drv.root->accept(new FillPrototype());
      check(drv.file);
      drv.root->accept(new CheckTypeScope());
      std::cout << drv.root->accept(new CheckPrinter()) << std::endl;
      ::errors.print(drv.file);

      drv.root->codegen();
      TheModule->print(llvm::outs(), nullptr);

    }
  else{
      std::cerr << "Non valid option argument" << std::endl;
      return 1;
  }
}
