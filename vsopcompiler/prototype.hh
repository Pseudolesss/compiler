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
        ASTnode* node;
       
};
class FieldPrototype{
    public:
        std::string type;
        ASTnode* node;
};
class ClassPrototype {
    public:
        //field id | field type
        std::unordered_map<std::string,FieldPrototype> field;
        // method id | return type
        std::unordered_map<std::string,MethodPrototype> method;
        // parent class id
        std::set<std::string> parent;
        std::string direct_parent;
        std::string toString();
        ASTnode* node;
    
};
// class id | class prototype
extern std::unordered_map<std::string,ClassPrototype> prototype; 
extern ErrorHandler errors;

#endif