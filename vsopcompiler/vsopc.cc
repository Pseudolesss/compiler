#include <iostream>
#include <unistd.h>
#include <stdlib.h>     

#include "driver.hh"
#include "FillPrototype.hh"
#include "CheckPrototype.hh"
#include "CheckTypeScope.hh"
#include "CheckPrinter.hh"
#include "prototype.hh"
#include "ParserPrinter.hh"
#include "SymbolTable.hh"
#include "CodeGenerator.hh"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

int main(int argc, char *argv[])
{

  driver drv;

  if (argc < 2)
  {
    std::cerr << "Wrong number of arguments" << std::endl;
    return 1;
  }

  if (argv[1] == std::string("-lex"))
  {
    drv.setting = 0;
    drv.file = argv[2];
    int ret = drv.parse(argv[2]);
    return ret;
  }
  else if (argv[1] == std::string("-check"))
  {
    drv.setting = 1;
    drv.parse("/home/IO.vsop");
    drv.root->accept(new FillPrototype());
    //Something to deallocate all the tree.

    int ret = drv.parse(argv[2]);
    if (ret == 0)
    {
      drv.root->accept(new FillPrototype());
      check(drv.file);
      drv.root->accept(new CheckTypeScope());
      std::cout << drv.root->accept(new CheckPrinter()) << std::endl;
      ::errors.print(drv.file);
      return !::errors.isempty();
    }
  }
  else if (argv[1] == std::string("-parse"))
  {
    drv.setting = 1;
    int ret = drv.parse(argv[2]);
    std::cout << drv.root->accept(new ParserPrinter()) << std::endl;
    return ret;
  }
  else if (argv[1] == std::string("-llvm") || argc == 2)
  {

    drv.setting = 1;
    std::cout << "parsing io.vsop" << std::endl;;

    drv.parse("IO.vsop");
    drv.root->accept(new FillPrototype());
    std::cout << "parsing file"<<  std::endl;
    int ret;
    if(argc == 2){
      ret = drv.parse(argv[1]);
    }else{
      ret = drv.parse(argv[2]);
    }
    drv.root->accept(new FillPrototype());
    std::cout << "fill proto done" << std::endl;
    check(drv.file);
    std::cout << "check done" << std::endl;
    drv.root->accept(new CheckTypeScope());
    std::cout << "type scope done" << std::endl;
    std::cout << drv.root->accept(new CheckPrinter()) << std::endl;
    ::errors.print(drv.file);
    std::cout << "printer done" << std::endl;
    drv.root->accept(new CodeGenerator());
    std::cout << "llvm code generated" << std::endl;
    TheModule->print(llvm::outs(), nullptr);

    if (argc == 3)
    {
      TheModule->print(llvm::outs(), nullptr);
    }
    else
    {
      // Initialize the target registry etc.
      llvm::InitializeAllTargetInfos();
      llvm::InitializeAllTargets();
      llvm::InitializeAllTargetMCs();
      llvm::InitializeAllAsmParsers();
      llvm::InitializeAllAsmPrinters();

      auto TargetTriple = llvm::sys::getDefaultTargetTriple();
      TheModule->setTargetTriple(TargetTriple);

      std::string Error;
      auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

      // Print an error and exit if we couldn't find the requested target.
      // This generally occurs if we've forgotten to initialise the
      // TargetRegistry or we have a bogus target triple.
      if (!Target)
      {
        llvm::errs() << Error;
        return 1;
      }

      auto CPU = "generic";
      auto Features = "";

      llvm::TargetOptions opt;
      auto RM = llvm::Optional<llvm::Reloc::Model>();
      auto TheTargetMachine =
          Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

      TheModule->setDataLayout(TheTargetMachine->createDataLayout());

      auto Filename = std::string(argv[1]);
      unsigned pos = Filename.find_last_of(".");
      Filename = Filename.substr(0,pos) + ".o";
      std::error_code EC;
      llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

      if (EC)
      {
        llvm::errs() << "Could not open file: " << EC.message();
        return 1;
      }

      llvm::legacy::PassManager pass;
      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

      if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
      {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
      }

      pass.run(*TheModule);
      dest.flush();

      llvm::outs() << "Wrote " << Filename << "\n";
      //deliver executable.
      std::string gcc_out = Filename.substr(0,pos);
      std::cout<<"writing executable"<<std::endl;
      std::string command = "g++ " + Filename + " -o main" ;//+ gcc_out;
      ret = system (command.c_str());
      return ret;
    }
  }
  else
  {
    std::cerr << "unkow command " << std::endl;
    return 1;
  }
}
