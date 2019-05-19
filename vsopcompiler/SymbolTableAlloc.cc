#include "SymbolTableAlloc.hh"
#include "prototype.hh"

#include <stack>
#include <string>
#include <unordered_map>


SymbolTableAlloc allocvtable = SymbolTableAlloc();

SymbolTableAlloc::SymbolTableAlloc(){
	new_scope();
}

SymbolTableAlloc::~SymbolTableAlloc(){}

void SymbolTableAlloc::add_element(std::string element_id, llvm::AllocaInst * element_type){
		symboltablealloc.front()[element_id] = element_type;
}

llvm::AllocaInst * SymbolTableAlloc::lookup(std::string element_id){
	for (std::list<std::unordered_map<std::string,llvm::AllocaInst *>>::iterator it=symboltablealloc.begin(); it != symboltablealloc.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return (*it)[element_id];
    	}
	}
	return nullptr;
}

bool SymbolTableAlloc::check_variable(std::string element_id){

	for (std::list<std::unordered_map<std::string,llvm::AllocaInst *>>::iterator it=symboltablealloc.begin(); it != symboltablealloc.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return true;
    	}
	}
	return false;
}

int SymbolTableAlloc::depth_variable(std::string element_id){
	int count = symboltablealloc.size();
	for (std::list<std::unordered_map<std::string,llvm::AllocaInst *>>::iterator it=symboltablealloc.begin(); it != symboltablealloc.end(); ++it){
    	if ((*it).find(element_id) != (*it).end()){
        	return count;
    	}
    	count--;
	}
	return count;
}

void SymbolTableAlloc::new_scope(){
	std::unordered_map<std::string,llvm::AllocaInst *> hashtable ;
	symboltablealloc.push_front(hashtable);
}

void SymbolTableAlloc::exit_scope(){
	symboltablealloc.pop_front();
}

std::unordered_map<std::string,llvm::AllocaInst *> SymbolTableAlloc::exit_class_scope(){
	std::unordered_map<std::string,llvm::AllocaInst *> map = symboltablealloc.front();
	symboltablealloc.pop_front();
	return map;
}
