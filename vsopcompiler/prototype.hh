#ifndef PROTOTYPE
#define PROTOTYPE

#include <unordered_map> 
#include <string>
#include <set>
#include <list>

class MethodPrototype{
    public:
        std::list<std::string> arguments;
        std::string return_type;
        std::string toString() const;
};
class ClassPrototype {
    public:
        //field id | field type
        std::unordered_map<std::string,std::string> field;
        // method id | return type
        std::unordered_map<std::string,MethodPrototype> method;
        // parent class id
        std::set<std::string> parent;
        std::string direct_parent;
        std::string toString();
    
};
// class id | class prototype
extern std::unordered_map<std::string,ClassPrototype> prototype; 

#endif