#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "SymbolTable.hh"


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
    //TODO : While (we must return as type the class of the first common ancestor of the two branches if the two branches have class type, and other situations)
    return "";
}

std::string CheckTypeScope::visit(While *aWhile)
{
    if(aWhile->getWhile()->accept(this) != "bool"){
        yy::location l = parent_expr->getLocation();
        errors.add(l,"The condition of the while is not boolean ");
    }
    return aWhile->getDo()->accept(this);
}

std::string CheckTypeScope::visit(Let *let){
    //TODO : Let
    return "";
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
    if(args->getExpr() != nullptr){
        return args->getExpr()->accept(this);
    }
    else{
        return " ";
    }
}

std::string CheckTypeScope::visit(Function *function)
{
    //TODO : Dispatch, the return value will be the value returned by the method invocation
    return " ";
}

std::string CheckTypeScope::visit(Dot *dot)
{
    //TODO : Dispatch, the return value will be the value returned by the method invocation
    return " ";
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