/*
check semantic error on prototype, check:
    -no Main class with main method: no arg and return int32
    -class redefined
    -field/method illegaly redefined field cannot be override, method can be override if same signature.
    -a field cannot be named self
    -an overriden method return type or argument does not match parent return type.
    -cyclic class
    -make inheritence 

*/
#include "prototype.hh"

bool check(std::string);
bool check_main(std::string);
bool check_inheritence(std::string);
bool _check_parent(std::set<std::string>&, std::string );