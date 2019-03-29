#include "prototype.hh"
#include <unordered_map> 
#include <string>
#include <set>
#include <iostream>

std::unordered_map<std::string,ClassPrototype> prototype; 

std::string const print_set(std::set<std::string> myset){
    std::string out = "{";
    for (std::string elem : myset) {
        out += elem + ",";
    }   
    return out + "}\n";
};

std::string MethodPrototype::toString() const{
    std::string out = "arguments: {";
    for (auto const& elem : arguments){
        out += elem + ",";
    }
    out += "} return type: " + return_type + "\n\t\t" ;
    return out;
};


std::string print_map(std::unordered_map<std::string,std::string> const &m)
{
    std::string out = "{";
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        out +=   (*it).first + " : " + (*it).second + " , ";
    }
    return out + "}\n";
};

std::string print_map(std::unordered_map<std::string,MethodPrototype> const &m)
{
    std::string out = "{";
    for (auto const& pair: m) {
        out +=   pair.first + " : " + pair.second.toString() + " , ";
    }
    return out + "}\n";
};

std::string ClassPrototype::toString(){
    std::string out  = "";
    //std::cout<<"print field"<<std::endl;
    out +="\t Field: " + print_map(field);
    //std::cout<<"print methods"<<std::endl;
    out +="\t Method: " + print_map(method) ;
    
    //std::cout<<"print parent"<<std::endl;
    out += "\t Parent: " + print_set(parent);
    return out;
}
