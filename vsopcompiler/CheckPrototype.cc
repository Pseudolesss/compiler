/*
ALL DONE, EXCEPT ERROR MESSAGE

check semantic error on prototype, check:
    -no Main class with main method: no arg and return int32
    -class redefined => already done
    -field/method illegaly redefined, implement inheritence.
    -a field cannot be named self
    -an overriden method return type or argument does not match parent return type.
    -cyclic class
    -make inheritence 

*/
#include "CheckPrototype.hh"
#include "prototype.hh"
#include <set>
//return true if no error, false otherwise
bool check_main(){
    if ( ::prototype.find("Main") ==  ::prototype.end()){
        return false;
    }
    if(::prototype["Main"].method.find("main") == ::prototype["Main"].method.end()){
        // no main function in Main class.
        MethodPrototype main = ::prototype["Main"].method["main"];
        if(main.return_type != "int32"){
            // invalid return type of main method in main classe.
            return false;
        }
        if(!main.arguments.empty){
            // argument in main method.
            return false;
        }
    }
    return true;
}
//TODO: add parent field, method to child. Check method/field override, cyclic inheritence. check no field named self.
bool check_parent(){
    bool out = true;
    //iterate over all classe in prototype 
    std::set<std::string> classes = std::set<std::string>();
    for (auto const& pair: prototype) {
        classes.insert(pair.first);
    }
    while(!classes.empty()){
        if(!_check_parent(classes,*classes.begin())){
            out = false;
        }
    }
    return out;
}

bool _check_parent(std::set<std::string>& todo,std::string classID){

    bool out = true;
    //base case.
    if(::prototype[classID].direct_parent == ""){
        if(::prototype[classID].field.find("self") != ::prototype[classID].field.end()){
            // error, a field cannot be named self.
        }
        todo.erase(classID);
        return true;
    }

    //base case 2: break cycle
    if(todo.find(classID) == todo.end()){
        return false;
    }
    todo.erase(classID);

    //recursive case: check parent before dealing with child.
    std::string parentID = ::prototype[classID].direct_parent;
    out = out && _check_parent(todo, parentID );
    //end of recursion.

    //Add parents of parent and check cycle.
    for (auto const& ancestor :  ::prototype[parentID].parent){
        if(::prototype[classID].parent.find(ancestor) != ::prototype[classID].parent.end()){
            //error : cycle
            out = false;
        }  
        else{
            ::prototype[classID].parent.insert(ancestor);
        }          
    }

    //Add parent field to child field and check there is no redefinition.
    for (auto const& parent_field: ::prototype[parentID].field){
        if(::prototype[classID].field.find(parent_field.first) != ::prototype[classID].field.end()){
            //error : redefinition of a field
            out = false;
        }  
        else{
            ::prototype[classID].field.insert(parent_field);
        }          
    }

    //Add parent method to child method if not override. check override are well done
    for (auto const& parent_method: ::prototype[parentID].method){
        if(::prototype[classID].method.find(parent_method.first) != ::prototype[classID].method.end()){
            //check the override.
            MethodPrototype parent = parent_method.second;
            MethodPrototype child = ::prototype[classID].method[parent_method.first];
            if(parent.return_type != child.return_type){
                //error: return type are different in override method.
                out = false;
            }
            if(parent.arguments != child.arguments){
                //error: argument differs.
            }
        }  
        else{
            ::prototype[classID].method.insert(parent_method);
        }          
    }
    return out; 
}
