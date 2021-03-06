#include "SymbolTable.hh"
#include "prototype.hh"

#include <stack>
#include <string>
#include <unordered_map>

SymbolTable vtable = SymbolTable();

SymbolTable::SymbolTable(){
	new_scope();
}

SymbolTable::~SymbolTable(){}

void SymbolTable::add_element(std::string element_id, std::string element_type, yy::location l){

	//cout<< element_id << " " << element_type << " " << l << "\n";

	bool already_defined = false;
	//We can have same variables but in different scopes

	std::list<std::unordered_map<std::string,std::string>>::iterator it=symboltable.begin();
	if ((*it).find(element_id) != (*it).end()){
		//The location must be the position of the variable, here we use main to test
		errors.add(l,"the variable " + element_id + " is already defined");
		already_defined = true;
    }

    if(!already_defined)
		symboltable.front()[element_id] = element_type;
}

std::string SymbolTable::lookup(std::string element_id, yy::location l){


	for (std::list<std::unordered_map<std::string,std::string>>::iterator it=symboltable.begin(); it != symboltable.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return (*it)[element_id];
    	}
	}
	//The location must be the position of the variable, here we use main to test
	errors.add(l,"the variable " + element_id + " is not defined in this scope");
	return " ";
}

bool SymbolTable::check_variable(std::string element_id){

	for (std::list<std::unordered_map<std::string,std::string>>::iterator it=symboltable.begin(); it != symboltable.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return true;
    	}
	}
	return false;
}

int SymbolTable::depth_variable(std::string element_id){
	int count = symboltable.size();
	for (std::list<std::unordered_map<std::string,std::string>>::iterator it=symboltable.begin(); it != symboltable.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return count;
    	}
    	count--;
	}
	return count;
}

void SymbolTable::new_scope(){
	std::unordered_map<std::string,std::string> hashtable ;
	symboltable.push_front(hashtable);
}

void SymbolTable::exit_scope(){
	symboltable.pop_front();
}

std::unordered_map<std::string,std::string> SymbolTable::exit_class_scope(){
	std::unordered_map<std::string,std::string> map = symboltable.front();
	symboltable.pop_front();
	return map;
}

	// for( auto it = map.begin() ; it != map.end() ; ++it ){
	// 	list.push_front(it->first,it->second);
	// }
