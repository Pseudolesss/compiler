#ifndef SYMBOLTABLEALLOC_H
#define SYMBOLTABLEALLOC_H

#include "location.hh"
#include <list>
#include <string>
#include <unordered_map>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

class SymbolTableAlloc
{
public:
	//Create an empty table
	SymbolTableAlloc();
	~SymbolTableAlloc();

	//Add a binding between a name and some information
	void add_element(std::string, llvm::AllocaInst *);

	//Lookup a name and retrieve its information
	llvm::AllocaInst * lookup(std::string);

	//Enter a new scope
	void new_scope();

	//Exit a scope
	void exit_scope();

	//Exit a class scope
	std::unordered_map<std::string, llvm::AllocaInst *> exit_class_scope();

	//Check if a variable is in the table
	bool check_variable(std::string element_id);
	int depth_variable(std::string element_id);

private:
	//The symbol table (a stack of hashtable, one for each scope)
	std::list<std::unordered_map<std::string, llvm::AllocaInst *>> symboltablealloc;
};

extern SymbolTableAlloc allocvtable;

#endif