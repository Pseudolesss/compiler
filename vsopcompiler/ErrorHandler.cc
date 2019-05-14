#include "ErrorHandler.hh"
#include "prototype.hh"
#include <iostream>
#include <utility> 
//return true if left < right

bool LocationComparator::operator() (const yy::location& left,const yy::location& right){
        if(left.begin.line < right.begin.line)
            return true;
        if(left.begin.line > right.begin.line)
            return false;
        if(left.begin.column < right.begin.column)
            return true;
        return false;
}

ErrorHandler::ErrorHandler(){
    errors = std::multimap<yy::location,std::string,LocationComparator>();
}
void ErrorHandler::add(yy::location l, std::string e){
    errors.insert(std::make_pair(l,": semantic error: " + e));
}

void ErrorHandler::print(std::string filepath){ 
    for (auto const& pair: errors) {
        std::cerr << filepath << pair.second << std::endl; //Modified temporarly
    }
}

bool ErrorHandler::isempty(){
    return errors.empty();
}

ErrorHandler errors = ErrorHandler();

