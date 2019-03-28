#ifndef PROTOTYPE
#define PROTOTYPE

#include <unordered_map> 
#include <string>
#include <set>
 
class ClassPrototype {
    public:
        //field id | field type
        std::unordered_map<std::string,std::string> field;
        // method id | return type
        std::unordered_map<std::string,std::string> method;
        // parent class id
        std::set<std::string> parent;
        std::string toString();
};
// class id | class prototype
extern std::unordered_map<std::string,ClassPrototype> prototype; 

#endif