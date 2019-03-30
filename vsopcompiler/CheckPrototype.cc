/*

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

bool check(std::string file){
    return check_main(file) && check_parent(file);
}
//return true if no error, false otherwise
bool check_main(std::string filename){
    if ( ::prototype.find("Main") ==  ::prototype.end()){
        errors.add(yy::location(&filename,1,1), "no Main Class defined");
        return false;
    }
    if(::prototype["Main"].method.find("main") == ::prototype["Main"].method.end()){
        // no main function in Main class.
        yy::location l = prototype["Main"].location;
        l.begin.filename = &filename;
        errors.add(l,"no main method define in Main Classe");
        return false;
    }
    else{
        MethodPrototype main = ::prototype["Main"].method["main"];
        //invalid return type of main in Main
        bool out = true;
        if(main.return_type != "int32"){
            yy::location l = main.return_loc;
            l.begin.filename = &filename;
            errors.add(l,"invalid return type of method main in Main, should be int32");
            out = false;
        }
        // main must not have argument.
        if(!main.arguments.empty()){
            yy::location l = main.args_loc;
            l.begin.filename = &filename;
            errors.add(l,"main method of classe Main cannot have arguments");
            out = false;
        }
        return out;
    } 
}

bool check_parent(std::string file){
    bool out = true;
    //iterate over all classe in prototype 
    std::set<std::string> classes = std::set<std::string>();
    for (auto const& pair: prototype) {
        classes.insert(pair.first);
    }
    while(!classes.empty()){
        if(!_check_parent(classes,*classes.begin(),file)){
            out = false;
        }
    }
    return out;
}

bool _check_parent(std::set<std::string>& todo,std::string classID,std::string file){

    bool out = true;

    //check there is no field self
    if(::prototype[classID].field.find("self") != ::prototype[classID].field.end()){
        yy::location l = prototype[classID].field["self"].location;
        errors.add(l,"field cannot be named self");
        out = false;
    }

     //base case 1.
    if(::prototype[classID].direct_parent == ""){
        todo.erase(classID);
        return out;
    }
    //base case 2: break cycle
    if(todo.find(classID) == todo.end()){
        return false;
    }
    todo.erase(classID);

    //recursive case: check parent before dealing with child.
    std::string parentID = ::prototype[classID].direct_parent;
    out = out && _check_parent(todo, parentID,file );
    //end of recursion.

    //Add parents of parent and check cycle.
    for (auto const& ancestor :  ::prototype[parentID].parent){
        if(::prototype[classID].parent.find(ancestor) != ::prototype[classID].parent.end()){
            //presence of a cycle in inheritence.
            yy::location l = prototype[classID].location;
            std::string parents = "";
            for(auto const& parent : prototype[parentID].parent){
                parents += parent + " , ";
            }
            errors.add(l," class " + classID + " is involved in a cycle inheritence with classes: " + parents);
            out = false;
        }  
        else{
            ::prototype[classID].parent.insert(ancestor);
        }          
    }

    //Add parent field to child field and check there is no redefinition.
    for (auto const& parent_field: ::prototype[parentID].field){
        if(::prototype[classID].field.find(parent_field.first) != ::prototype[classID].field.end()
        && parent_field.first != "self"){
            //error : redefinition of a field
            yy::location base_field = parent_field.second.location;
            yy::location child_field = prototype[classID].field[parent_field.first].location;
            errors.add(child_field, "Illegal override of field " + parent_field.first 
                + " defined in parent class " + parentID + " at " + locToStr(base_field));
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
                yy::location parent_loc = parent.return_loc;
                yy::location child_loc = child.return_loc;
                errors.add(child_loc," overrided method " + parent_method.first
                    + " should have return type " + parent_method.second.return_type
                    + " defined at " + locToStr(parent_loc));
                out = false;
            }
            if(parent.arguments != child.arguments){
                //error: argument differs.
                yy::location parent_loc = parent.args_loc;
                yy::location child_loc = child.args_loc;  
                std::string args = "";
                for(auto const & arg : parent_method.second.arguments){
                    args += arg + ",";
                }
                errors.add(child_loc," overrided method " + parent_method.first
                    + " should have argument " + args
                    + " defined at " + locToStr(parent_loc));             
            }
        }  
        else{
            ::prototype[classID].method.insert(parent_method);
        }          
    }
    return out; 
}
