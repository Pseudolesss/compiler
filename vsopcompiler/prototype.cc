#include "prototype.hh"
#include <unordered_map> 
#include <string>
#include <set>
#include <iostream>

std::unordered_map<std::string,ClassPrototype> prototype; 

std::string print_set(std::set<std::string> myset){
    std::string out = "";
    for (std::string elem : myset) {
        out += elem + ",";
    }   
};
std::string print_map(std::unordered_map<std::string,std::string> const &m)
{
    std::string out = "";
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        out +=  " { " + (*it).first + " : " + (*it).second + " }\n ";
    }
    out += "\n";
    return out;
};

std::string ClassPrototype::toString(){
    std::string out  = "";
    out +="\tField: " + print_map(field);
    out += "\tMethod: " + print_map(method);
    out += "\tParent: " + print_set(parent);
    return out;
}
