#include "SymbolTable.hh"
#include "prototype.hh"

#include <stack>
#include <string>
#include <unordered_map>

SymbolTable::SymbolTable(){
	new_scope();
}

SymbolTable::~SymbolTable(){}

void SymbolTable::add_element(std::string element_id, std::string element_type){
	symboltable.top()[element_id] = element_type;
}

std::string SymbolTable::lookup(std::string element_id){

	std::stack<std::unordered_map<std::string,std::string>> _symboltable = symboltable;

	while(!_symboltable.empty()){
		if (_symboltable.top().find(element_id) != _symboltable.top().end()){
        	return _symboltable.top()[element_id];
    	}
    	else{
    		_symboltable.pop();
    	}
	}
	//The location must be the position of the variable, here we use main to test
	yy::location l = prototype["Main"].location;
	errors.add(l,"the variable " + element_id + " is not defined in this scope");
	return " ";
}

void SymbolTable::new_scope(){
	std::unordered_map<std::string,std::string> hashtable ;
	symboltable.push(hashtable);
}

void SymbolTable::exit_scope(){
	symboltable.pop();
}

