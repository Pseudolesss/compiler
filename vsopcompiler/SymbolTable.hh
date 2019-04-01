#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stack>
#include <string>
#include <unordered_map>

class SymbolTable
{
public:
	//Create an empty table
	SymbolTable();
	~SymbolTable();

	//Add a binding between a name and some information
	void add_element(std::string, std::string);

	//Lookup a name and retrieve its information
	std::string lookup(std::string);

	//Enter a new scope
	void new_scope();

	//Exit a scope 
	void exit_scope();

private:

	//The symbol table (a stack of hashtable, one for each scope)
	std::stack<std::unordered_map<std::string,std::string>> symboltable;
	
};



#endif