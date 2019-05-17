#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "CheckTypeScope.hh"
#include "ASTnode.hh"
#include "SymbolTable.hh"
#include "prototype.hh"

std::string self_classID;

std::unordered_map<std::string,std::unordered_map<std::string,std::string>> class_variables_table;

bool is_field_init = false;


std::string CheckTypeScope::visit(ASTnode* asTnode){return "NOT SUPPOSED TO HAPPEN ASTNODE";}

std::string CheckTypeScope::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN EXPRESSION";}

std::string CheckTypeScope::visit(Type *type)
{
    type->setType(type->getID());
    return type->getID();
}

std::string CheckTypeScope::visit(Field *field)
{
    //cout << ::prototype[self_classID].field[field->getID()].type << field->getExpr()->accept(this) << "\n";


    //We search parents if the arg type is a class
    std::set<std::string> expr_parents;
    std::string expr_type;
    if(field->getExpr()){
        ::is_field_init = true;
        expr_type = field->getExpr()->accept(this);
        ::is_field_init = false;
    }
    std::string field_type = ::prototype[self_classID].field[field->getID()].type;
    //cout<<field_type<<"\n";

    if(field_type != "string" && field_type != "unit" && field_type != "bool" && field_type != "int32" && prototype.find(field_type)==prototype.end()){
        yy::location l = field->getLocation();
        errors.add(l,"Field : used of an undefined type ");
    }

    if(field->getExpr() != 0 && ::prototype.find(expr_type) != ::prototype.end()){
        expr_parents = ::prototype[expr_type].parent;

        //In this case the expr_type is a class an field type an object, no need to do the other check
        if(field_type == "Object"){
            ::vtable.add_element(field->getID(),field->getType()->accept(this),field->getLocation());
            return "done";
        }
    }

    if( field->getExpr() != 0 && field_type != expr_type && (expr_parents.find(field_type)==expr_parents.end())){
        yy::location l = field->getLocation();
        errors.add(l,"Field : The return value type of the expr is not correct  ");
    }
    ::vtable.add_element(field->getID(),field->getType()->accept(this),field->getLocation());
    return "done";
}

std::string CheckTypeScope::visit(Formal *formal)
{
    std::cerr << " checkTypeScope in formal";
    return "should never happend";
}

std::string CheckTypeScope::visit(Formalx *formalx)
{
    std::cerr << " checkTypeScope in formalx";
    return "should never happend";
}

std::string CheckTypeScope::visit(Formals *formals)
{
    Formal* formal = formals->getFormal();
    Formalx* formalx = formals->getFormalx();
    std::string type;

    while(true){
        if(formal == nullptr){
            break;
        }
        type = formal->getType()->accept(this);
        if(type != "string" && type != "unit" && type != "bool" && type != "int32" && prototype.find(type)==prototype.end()){
            yy::location l = formal->getLocation();
            errors.add(l,"Formal : used of an undefined type ");
        }

        ::vtable.add_element(formal->getID(),formal->getType()->accept(this),formal->getLocation());
        formal = formalx->getFormal();
        formalx = formalx->getFormalx();
    }
    return "done";
}

std::string CheckTypeScope::visit(Exprx *exprx)
{
    std::cerr << " checkTypeScope in exprx";
    return "should never happend";
}

std::string CheckTypeScope::visit(Exprxx *exprxx)
{
    std::cerr << " checkTypeScope in exprxx";
    return "should never happend";
}

std::string CheckTypeScope::visit(Block *block)
{
    ::vtable.new_scope();
    Expr* expr = block->getExpr();
    Exprx* exprx = block->getExprx();
    std::string current_type;

    while(true){
        if(expr == nullptr){
            break;
        }
        current_type = expr->accept(this);
        expr = exprx->getExpr();
        exprx = exprx->getExprx();
    }
    ::vtable.exit_scope();

    block->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(Method *method)
{
    ::vtable.new_scope();
    method->getFormals()->accept(this);

    std::string ret_type = ::prototype[self_classID].method[method->getID()].return_type;
    std::set<std::string> parents;
    std::string block_ret_type = method->getBlock()->accept(this);


    //IF the block type is a class we search the parents
    if(::prototype.find(block_ret_type) != ::prototype.end()){
        parents = ::prototype[block_ret_type].parent;

        //In this case the expr_type is a class an field type an object, no need to do the other check
        if(ret_type == "Object"){
            vtable.exit_scope();
            return "done";
        }
    }

    //If the return types are different and the ret_type is not a ancestor of block_ret_type
    if((ret_type != block_ret_type) && (parents.find(ret_type) == parents.end()) ){
        yy::location l = method->getLocation();
        errors.add(l,"Method : The return value type of the block is not correct  ");
    }
    vtable.exit_scope();
    return "done";
}

std::string CheckTypeScope::visit(FieldMethod *fieldMethod){
    if(fieldMethod->getFieldMethod() == nullptr)
        return "done";
    if(fieldMethod->getMethod() == nullptr){
        fieldMethod->getFieldMethod()->accept(this);
        fieldMethod->getField()->accept(this);
        return  "done";
    }
    else{
        fieldMethod->getFieldMethod()->accept(this);
        fieldMethod->getMethod()->accept(this);
        return "done";
    }
}

std::string CheckTypeScope::visit(Body *body){
    body->getFieldMethod()->accept(this);
    return "done";
}

std::string CheckTypeScope::visit(Classe *classe)
{

    ::self_classID = classe->getTypeID();
    ::vtable.new_scope();
    //Check parents and add the variables inherited from them
    std::set<std::string> parents;
    if(::prototype.find(::self_classID) != ::prototype.end()){
        parents = ::prototype[::self_classID].parent;
    }

    //cout<<::self_classID<<"\n";
    for(auto it = parents.begin(); it != parents.end(); ++it){
        //cout<<*it<<"\n";
        if(class_variables_table.find(*it) != class_variables_table.end()){
            for(auto it_ = class_variables_table[*it].begin(); it_ != class_variables_table[*it].end(); ++it_ ){
                //If the variable to add has not been already added because of another ancestor
                //If we don't check if chained inheritance the variable will be added two times 
                if(!::vtable.check_variable(it_->first))
                    ::vtable.add_element(it_->first,it_->second,classe->getLocation());
            }
            
        }
    }

    classe->getBody()->accept(this);
    class_variables_table[::self_classID] = ::vtable.exit_class_scope();
    return "done";
}

std::string CheckTypeScope::visit(Classes *classes){
    if(classes->nextClass() == nullptr){
        classes->getClass()->accept(this);
        return "done";
    }
    else{
        classes->nextClass()->accept(this);
        classes->getClass()->accept(this);
        return "done";
    }
}

std::string CheckTypeScope::visit(Programm *programm)
{
    cout<<"cheking type scope"<<std::endl;
    return programm->getClasses()->accept(this);
}


std::string CheckTypeScope::visit(Dual *dual)
{
    std::cerr << " checkTypeScope in dual";
    return "should never happend";
}

std::string CheckTypeScope::visit(Unary *unary)
{
    std::cerr << " checkTypeScope in unary";
    return "should never happend";
}

std::string CheckTypeScope::visit(If *anIf){
    std::string if_type = anIf->getIf()->accept(this);
    std::string then_type = anIf->getThen()->accept(this);
    std::string else_type;
    if(anIf->getElse()!=0)
        else_type = anIf->getElse()->accept(this);
    else
        else_type = "";

    //cout<<if_type << then_type << else_type << "\n";


    yy::location l = anIf->getLocation();

    if(if_type != "bool"){
        errors.add(l,"The condition of the If is not boolean ");
        std::string current_type = "int32";
        anIf->setType(current_type);
        return current_type;
    }

    if(else_type != "" && ::prototype.find(then_type) != ::prototype.end() && ::prototype.find(else_type) != ::prototype.end()){
        std::set<std::string> then_parents = ::prototype[then_type].parent;
        std::set<std::string> else_parents = ::prototype[else_type].parent;

        //We search the common class between else and then 

        //Case 1.1 the class are same
        if(else_type == then_type){
            anIf->setType(then_type);
            return then_type;
        }

        //Case 1.2 the then class is one of the else parent
        if(else_parents.find(then_type) != else_parents.end()){
            anIf->setType(then_type);
            return then_type;
        }

        //Case 1.3 the else class is one of the then parent
        if(then_parents.find(else_type) != else_parents.end()){
            anIf->setType(else_type);
            return else_type;
        }

        //Case 2: they have common parents
        int count = 0;
        for (auto it= then_parents.end();it!=then_parents.begin();--it){
            if( count && else_parents.find(*it) != else_parents.end()){
                //cout<<*it<<"\n";
                std::string current_type = *it;
                anIf->setType(current_type);
                return current_type;
            }
            count++;
        }

        //Case 3: they have no common class
        std::string current_type = "Object";
        anIf->setType(current_type);
        return current_type;
    }

    if(then_type == "unit" || else_type == "unit" || else_type == ""){
        std::string current_type = "unit";
        anIf->setType(current_type);
        return current_type;
    }

    if( (then_type == else_type && else_type == "int32") || (then_type ==  else_type && else_type =="string")){
        std::string current_type = then_type;
        anIf->setType(current_type);
        return current_type;
    }

    errors.add(l,"IfElse : the branches types doesn't match ");
    std::string current_type = "int32";
    anIf->setType(current_type);
    return current_type;  
}

std::string CheckTypeScope::visit(While *aWhile)
{
    if(aWhile->getWhile()->accept(this) != "bool"){
        yy::location l = aWhile->getLocation();
        errors.add(l,"The condition of the while is not boolean ");
    }

    aWhile->getDo()->accept(this);
    return "unit";
}

std::string CheckTypeScope::visit(Let *let)
{
    //Check assign
    //search parents if the effective type is a class
    std::string type = let->getType()->accept(this);

    if(let->getAssign() != 0){
        std::string arg = let->getAssign()->accept(this);
        
        //We search parents if the arg type is a class
        std::set<std::string> arg_parents;
        if(::prototype.find(arg) != ::prototype.end())
            arg_parents = ::prototype[arg].parent;

        if( type != arg && (arg_parents.find(type) == arg_parents.end())){  //Note : what appens if the init expression in null ?
            yy::location l = let->getLocation();
            errors.add(l,"Let : The type of the initializer must conform to the declared type <type> ");
            std::string current_type = "int32";
            let->setType(current_type);
            return current_type;
        }
    }

    if(type != "string" && type != "unit" && type != "bool" && type != "int32" && prototype.find(let->getType()->accept(this))==prototype.end()){
        yy::location l = let->getLocation();
        errors.add(l,"Let : used of an undefined type ");
        std::string current_type = let->getIn()->accept(this);
        let->setType(current_type);
        return current_type;
    }

    ::vtable.new_scope();
    ::vtable.add_element(let->getObjID(),let->getType()->accept(this),let->getLocation());
    std::string returnvalue = let->getIn()->accept(this);
    ::vtable.exit_scope();
    std::string current_type = returnvalue;
    let->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(Assign *assign)
{
    yy::location l = assign->getLocation();
    std::string expr_type = assign->getExpr()->accept(this);

    if(::vtable.lookup(assign->getObjID(),l) == expr_type){
        std::string current_type = expr_type;
        assign->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = assign->getLocation();
        errors.add(l,"in assigment, the type of the the expression doesn't conform to the type of the identifier ");
        std::string current_type = ::vtable.lookup(assign->getObjID(),l);
        assign->setType(current_type);
        return current_type;
    }

}

std::string CheckTypeScope::visit(Not *aNot)
{
    std::string current_type = aNot->getExpr()->accept(this);
    aNot->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(And *anAnd)
{
    std::string ltype = anAnd->getLeft()->accept(this);
    std::string rtype = anAnd->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = "bool";
        anAnd->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = anAnd->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = "bool";
        anAnd->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Equal *equal)
{
    std::string ltype = equal->getLeft()->accept(this);
    std::string rtype = equal->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = "bool";
        equal->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = equal->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = "bool";
        equal->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Lower *lower)
{
    std::string ltype = lower->getLeft()->accept(this);
    std::string rtype = lower->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = "bool";
        lower->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = lower->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = "bool";
        lower->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(LowerEqual *lowerEqual)
{
    std::string ltype = lowerEqual->getLeft()->accept(this);
    std::string rtype = lowerEqual->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = "bool";
        lowerEqual->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = lowerEqual->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = "bool";
        lowerEqual->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Plus *plus)
{
    std::string ltype = plus->getLeft()->accept(this);
    std::string rtype = plus->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = ltype;
        plus->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = plus->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = ltype;
        plus->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Minus *minus)
{
    std::string ltype = minus->getLeft()->accept(this);
    std::string rtype = minus->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = ltype;
        minus->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = minus->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = ltype;
        minus->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Times *times)
{
    std::string ltype = times->getLeft()->accept(this);
    std::string rtype = times->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = ltype;
        times->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = times->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = ltype;
        times->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Div *div)
{
    std::string ltype = div->getLeft()->accept(this);
    std::string rtype = div->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = ltype;
        div->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = div->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = ltype;
        div->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Pow *pow)
{
    std::string ltype = pow->getLeft()->accept(this);
    std::string rtype = pow->getRight()->accept(this);
    if (ltype == rtype){
        std::string current_type = ltype;
        pow->setType(current_type);
        return current_type;
    }
    else{
        yy::location l = pow->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        std::string current_type = ltype;
        pow->setType(current_type);
        return current_type;
    }
}

std::string CheckTypeScope::visit(Minus1 *minus1)
{
    std::string current_type = minus1->getExpr()->accept(this);
    minus1->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(IsNull *isNull)
{
    if(::prototype.find(isNull->getExpr()->accept(this)) == ::prototype.end()){
        yy::location l = isNull->getLocation();
        errors.add(l,"isnull : the type of expr is not Object");
    }
    isNull->setType("bool");
    return "bool";
}

std::string CheckTypeScope::visit(Args *args){
    std::cerr << " checkTypeScope in Args";
    return "should never happend";
}

std::string CheckTypeScope::visit(Function *function)
{

    std::string id = function->getID();
    yy::location l = function->getLocation();


    //cout<<"enter in function : "<< l << "\n";

    //Case 1 : the method exists not in the self class
    if(::prototype[::self_classID].method.find(id) == ::prototype[::self_classID].method.end()){

        //Case 1.1 : the method exists in IO
        if(::prototype["IO"].method.find(id) != ::prototype[::self_classID].method.end()){
            Args* args = function->getArgs();
            Expr* expr = args->getExpr();
            std::list<std::string> method_args = ::prototype["IO"].method[id].arguments;
            if((expr == nullptr && method_args.empty() ) || (expr->accept(this) == method_args.front()) ){
                std::string current_type = ::prototype["IO"].method[id].return_type;
                function->setType(current_type);
                return current_type;
            }
            else{
                errors.add(l, "Dispatch : wrong arguments of a IO method" );
                std::string current_type = "int32";
                function->setType(current_type);
                return current_type;
            }
        }

        errors.add(l, "Dispatch : The class self has no method " + id );
        std::string current_type = "int32";
        function->setType(current_type);
        return current_type;
    }

    //Case 2 : the method is defined in the self class, we check the args
    if(is_field_init == true)
        errors.add(l, "cannot use self in field initializer" );

    std::list<std::string> method_args = ::prototype[::self_classID].method[id].arguments;

    Args* args = function->getArgs();
    Expr* expr = args->getExpr();
    Exprxx* exprxx = args->getExprxx();

    while(true){
        if(expr == nullptr){
            break;
        }

        //search parents if the effective type is a class
        std::string arg = expr->accept(this);
        std::set<std::string> arg_parents;
        if(::prototype.find(arg) != ::prototype.end())
            arg_parents = ::prototype[arg].parent;

        if(method_args.front() == arg || (arg_parents.find(method_args.front()) != arg_parents.end() ) ){
            method_args.pop_front();
            expr = exprxx->getExpr();
            exprxx = exprxx->getExprxx();
        }
        else{
            errors.add(l, "Dispatch : The arguments types doesn't match  " + id );
            std::string current_type = "int32";
            function->setType(current_type);
            return current_type;
        }
    }

    std::string current_type = ::prototype[::self_classID].method[id].return_type;
    function->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(Dot *dot)
{

    std::string expr_0_type = dot->getExpr()->accept(this); //must be a class type
    std::string id = dot->getID();
    yy::location l = dot->getLocation();

    //cout<<"enter in dot : "<< l << "\n";

    if(::prototype.find(expr_0_type) == ::prototype.end()){
        errors.add(l, "Dispatch : The type of the expr_0 is not a class");
        std::string current_type = "int32";
        dot->setType(current_type);
        return current_type;

        //The method caled is not defined in the input code, 
        //it comes from an imported class, so we cannot check thzt the class P has a method <id> and the arguments

    }
    if(::prototype[expr_0_type].method.find(id) == ::prototype[expr_0_type].method.end()){
        errors.add(l, "Dispatch : The class <expr_0> has no method " + id );
        std::string current_type = "int32";
        dot->setType(current_type);
        return current_type;
    }

    std::list<std::string> method_args = ::prototype[expr_0_type].method[id].arguments;

    Args* args = dot->getArgs();
    Expr* expr = args->getExpr();
    Exprxx* exprxx = args->getExprxx();
    int n_args = method_args.size();
    int count = 0;
    while(true){
        if(expr == nullptr){
            break;
        }

        //search parents if the effective type is a class
        std::string arg = expr->accept(this);
        std::set<std::string> arg_parents;
        if(::prototype.find(arg) != ::prototype.end())
            arg_parents = ::prototype[arg].parent;

        if(method_args.front() == arg || (arg_parents.find(method_args.front()) != arg_parents.end() ) ){
            method_args.pop_front();
            expr = exprxx->getExpr();
            exprxx = exprxx->getExprxx();
        }
        else{
            errors.add(l, "Dispatch : The arguments types doesn't match  " + id );
            std::string current_type = "int32";
            dot->setType(current_type);
            return current_type;
        }
        count ++;
    }

    if(count!=n_args){
        errors.add(l, "the number of arguments does not match " );
    }

    std::string current_type = ::prototype[expr_0_type].method[id].return_type;
    dot->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(New *aNew)
{
    std::string current_type = aNew->getTypeID();
    aNew->setType(current_type);
    return current_type;
}

std::string CheckTypeScope::visit(ObjID *objID)
{
    yy::location l = objID->getLocation();
    if(objID->getID()=="()"){
        objID->setType("unit");
        return "unit";
    }
    if(objID->getID() == "self"){
        if(is_field_init == true)
            errors.add(l, "cannot use self in field initializer" );

        objID->setType(::self_classID);
        return self_classID;
        
    }
    std::string type = ::vtable.lookup(objID->getID(),l);


    if(type == " "){
        objID->setType("int32");
        return "int32"; //If error we send int32 by default
    }
    else{
        //If we are in a field init and the variable is of "depth 2" (class field)
        if(is_field_init == true && ::vtable.depth_variable(objID->getID())<3 ){
            errors.add(l, "cannot use a class field in field initializer" );
        }

        objID->setType(type);
        return type;
    }
}

std::string CheckTypeScope::visit(Literal *literal)
{
    std::cerr << " checkTypeScope in Literal";
    return "should never happend";
}

std::string CheckTypeScope::visit(IntLit *intLit)
{
    intLit->setType("int32");
    return "int32";
}

std::string CheckTypeScope::visit(StrLit *strLit)
{
    strLit->setType("string");
    return "string";
}

std::string CheckTypeScope::visit(BoolLit *boolLit)
{
    boolLit->setType("bool");
    return  "bool";
}

std::string CheckTypeScope::visit(Lpar *lpar)
{
    std::cerr << " checkTypeScope in (";
    return "should never happend";
}

std::string CheckTypeScope::visit(Rpar *rpar)
{
    std::cerr << " checkTypeScope in (";
    return "should never happend";
}

std::string CheckTypeScope::visit(Parenthese *parenthese)
{
    std::string current_type = parenthese->getExpr()->accept(this);
    parenthese->setType(current_type);
    return current_type;
}




//
