#ifndef PROTOTYPE
#define PROTOTYPE

#include <unordered_map> 
#include <string>
#include <set>
#include <list>
#include "ErrorHandler.hh"
#include "ASTnode.hh"
class MethodPrototype{
    public:
        std::list<std::string> arguments;
        std::string return_type;
        std::string toString() const;
        yy::location location;
        yy::location args_loc;
        yy::location return_loc; 
       
};
class FieldPrototype{
    public:
        std::string type;
        yy::location location;
};
class ClassPrototype {
    public:
        //field id | field type
        std::unordered_map<std::string,FieldPrototype> field;
        // method id | return type
        std::unordered_map<std::string,MethodPrototype> method;
        // To track relative order of "keys" field/method insertion.
        std::vector<std::string> fieldKeys, methodKeys;
        // parent class id
        std::set<std::string> parent;
        std::string direct_parent;
        std::string toString();
        yy::location location;
    
};

std::string locToStr(yy::location);
// class id | class prototype
extern std::unordered_map<std::string,ClassPrototype> prototype; 
extern ErrorHandler errors;

#endif