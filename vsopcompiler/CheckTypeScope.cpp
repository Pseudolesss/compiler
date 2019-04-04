#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "SymbolTable.hh"

std::string self_classID;


std::string Visitor::visit(ASTnode* asTnode){return "NOT SUPPOSED TO HAPPEN ASTNODE";}

std::string Visitor::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN EXPRESSION";}

std::string Visitor::visit(Type *type){return type->getID();}

std::string Visitor::visit(Field *field)
{
    if(::prototype[self_classID].field[field.getID()].type != field->getExpr()->accept(this)){
        yy::location l = field->getLocation();
        errors.add(l,"Field : The return value type of the expr is not correct  ");
    }
    return "done";
}

std::string Visitor::visit(Formal *formal)
{
    std::cerr << " checkTypeScope in formal";
    return "should never happend";
}

std::string Visitor::visit(Formalx *formalx)
{
    std::cerr << " checkTypeScope in formalx";
    return "should never happend";
}

std::string Visitor::visit(Formals *formals)
{
    std::cerr << " checkTypeScope in formals";
    return "should never happend";
}

std::string Visitor::visit(Exprx *exprx)
{
    std::cerr << " checkTypeScope in exprx";
    return "should never happend";
}

std::string Visitor::visit(Exprxx *exprxx)
{
    std::cerr << " checkTypeScope in exprxx";
    return "should never happend";
}

std::string Visitor::visit(Block *block)
{
    ::vtable.new_scope();
    Expr* expr = block->getExpr();
    Exprxx* exprxx = block->getExprxx();
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
    return current_type;
}

std::string Visitor::visit(Method *method)
{
    if(::prototype[self_classID].method[method.getID()].return_type != method->getBlock()->accept(this)){
        yy::location l = method->getLocation();
        errors.add(l,"Method : The return value type of the block is not correct  ");
    }
    return "done";
}

std::string Visitor::visit(FieldMethod *fieldMethod){
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

std::string Visitor::visit(Body *body){
    body->getFieldMethod()->accept(this);
    return "done";
}

std::string Visitor::visit(Classe *classe){
    ::self_classID = classe.getTypeID();
    classe->getBody()->accept(this);
    return "done";
}

std::string Visitor::visit(Classes *classes){
    if(classes->nextClass() == nullptr)
        classes->getClass()->accept(this);
        return "done";
    else{
        classes->nextClass()->accept(this);
        classes->getClass()->accept(this);
        return "done";
    }
}

std::string Visitor::visit(Programm *programm)
{
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
    std::string else_type = anIf->getElse()->accept(this);
    yy::location l = anIf->getLocation();

    if(if_type != "bool"){
        errors.add(l,"The condition of the If is not boolean ");
        return "int64";
    }

    if(::prototype.find(then_type) != ::prototype.end() && ::prototype.find(else_type) != ::prototype.end()){
        std::set<std::string> then_parents = ::prototype[then_type].parent;
        std::set<std::string> else_parents = ::prototype[else_type].parent;
        for (std::set<std::string>::iterator it=then_parents.begin(); it!=then_parents.end(); ++it){
            if(else_parents.find(*it) != else_parents.end()){
                return *it;
            }
        }
        return "Object";
    }

    if(then_type == "unit" || else_type == "unit"){
        return "unit";
    }

    if(then_type == else_type == "int64" || then_type ==  else_type == "string"){
        return then_type;
    }

    errors.add(l,"IfElse : the branches types doesn't match ");
    return "int64";  
}

std::string CheckTypeScope::visit(While *aWhile)
{
    if(aWhile->getWhile()->accept(this) != "bool"){
        yy::location l = aWhile->getLocation();
        errors.add(l,"The condition of the while is not boolean ");
    }
    return aWhile->getDo()->accept(this);
}

std::string CheckTypeScope::visit(Let *let)
{
    std::string init_type = let->getAssign()->accept(this);
    if(init_type != nullptr && let->getType() != init_type){
        yy::location l = let->getLocation();
        errors.add(l,"Let : The type of the initializer must conform to the declared type <type> ");
        return "int64";
    }
    ::vtable.new_scope();
    ::vtable.add_element(let->getObjID(),let->getType());
    std::string returnvalue = let->getIn()->accept(this);
    ::vtable.exit_scope();
    return returnvalue;
}

std::string CheckTypeScope::visit(Assign *assign)
{
    std::string expr_type = assign->getExpr()->accept(this);
    if(::vtable.lookup(assign->getObjID()) == expr_type){
        return expr_type;
    }
    else{
        yy::location l = assign->getLocation();
        errors.add(l,"in assigment, the type of the the expression doesn't conform to the type of the identifier ");
        return ::vtable.lookup(assign->getObjID());
    }

}

std::string CheckTypeScope::visit(Not *aNot)
{
    return aNot->getExpr()->accept(this);
}

std::string CheckTypeScope::visit(And *anAnd)
{
    return comparetwotypes(anAnd);
}

std::string CheckTypeScope::visit(Equal *equal)
{
    return comparetwotypes(equal);
}

std::string CheckTypeScope::visit(Lower *lower)
{
    return comparetwotypes(lower);
}

std::string CheckTypeScope::visit(LowerEqual *lowerEqual)
{
    return comparetwotypes(lowerEqual);
}

std::string CheckTypeScope::visit(Plus *plus)
{
    return comparetwotypes(plus);
}

std::string CheckTypeScope::visit(Minus *minus)
{
    return comparetwotypes(minus);
}

std::string CheckTypeScope::visit(Times *times)
{
    return comparetwotypes(times);
}

std::string CheckTypeScope::visit(Div *div)
{
    return comparetwotypes(div);
}

std::string CheckTypeScope::visit(Pow *pow)
{
    return comparetwotypes(pow);
}

std::string CheckTypeScope::visit(Minus1 *minus1)
{
    return minus1->getExpr()->accept(this);
}

std::string CheckTypeScope::visit(IsNull *isNull)
{
    return "bool";
}

std::string CheckTypeScope::visit(Args *args){
    std::cerr << " checkTypeScope in Args";
    return "should never happend";
}

std::string CheckTypeScope::visit(Function *function)
{
    
    std::string id = dot->getID();
    yy::location l = dot->getLocation();

    if(::prototype[::self_classID].method.find(id) == ::prototype[::self_classID].end()){
        errors.add(l, "Dispatch : The class self has no method " + id );
        return "int64";
    }

    std::list<std::string> method_args = ::prototype[::self_classID].method[id].arguments;

    Args* args = dot->getArgs();
    Expr* expr = args->getExpr();
    Exprxx* exprxx = args->getExprxx();

    while(){
        if(expr == nullptr){
            break;
        }
        if(expr->accept(this) == method_args.front()){
            method_args.pop_front();
            expr = exprxx->getExpr();
            exprxx = exprxx->getExprxx();
        }
        else{
            errors.add(l, "Dispatch : The arguments types doesn't match  " + id );
            return "int64";
        }
    }

    return ::prototype[::self_classID].method[id].return_type;
}

std::string CheckTypeScope::visit(Dot *dot)
{

    std::string expr_0_type = dot->getExpr()->accept(this); //must be a class type
    std::string id = dot->getID();
    yy::location l = dot->getLocation();

    if(::prototype.find(expr_0_type) == ::prototype.end()){
        errors.add(l, "Dispatch : The type of the expr_0 is not a class");
        return "int64";
    }
    if(::prototype[expr_0_type].method.find(id) == ::prototype[expr_0_type].end()){
        errors.add(l, "Dispatch : The class <expr_0> has no method " + id );
        return "int64";
    }

    std::list<std::string> method_args = ::prototype[expr_0_type].method[id].arguments;

    Args* args = dot->getArgs();
    Expr* expr = args->getExpr();
    Exprxx* exprxx = args->getExprxx();

    while(){
        if(expr == nullptr){
            break;
        }
        if(expr->accept(this) == method_args.front()){
            method_args.pop_front();
            expr = exprxx->getExprxx();
            exprxx = exprxx->getExprxx();
        }
        else{
            errors.add(l, "Dispatch : The arguments types doesn't match  " + id );
            return "int64";
        }
    }

    return ::prototype[expr_0_type].method[id].return_type;
}

std::string CheckTypeScope::visit(New *aNew)
{
    return aNew->getTypeID();
}

std::string CheckTypeScope::visit(ObjID *objID)
{
    if(objID->getID()=="()")
        return "unit";
    std::string type = ::vtable.lookup(objID->getID());
    if(type == " ")
        return "int64"; //If error we send int64 by default
    else
        return type;
}

std::string CheckTypeScope::visit(Literal *literal)
{
    std::cerr << " checkTypeScope in Literal";
    return "should never happend";
}

std::string CheckTypeScope::visit(IntLit *intLit)
{
    return "int64";
}

std::string CheckTypeScope::visit(StrLit *strLit)
{
    return "string";
}

std::string CheckTypeScope::visit(BoolLit *boolLit)
{
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
    return parenthese->getExpr()->accept(this);
}

static std::string comparetwotypes(Expr *parent_expr){
    std::string ltype = parent_expr->getLeft()->accept(this);
    std::string rtype = parent_expr->getRight()->accept(this);
    if (ltype == rtype){
        return ltype;
    }
    else{
        yy::location l = parent_expr->getLocation();
        errors.add(l,"the two arguments have different types : "+ ltype + " and " + rtype);
        return "int64";
    }
}



//