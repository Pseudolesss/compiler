
#ifndef VISITOR_H
#define VISITOR_H

#include "SymbolTable.hh"

struct ASTnode;
struct Expr;
struct Type;
struct Field;
struct Formal;
struct Formalx;
struct Formals;
struct Exprx;
struct Exprxx;
struct Block;
struct Method;
struct FieldMethod;
struct Body;
struct Classe;
struct Classes;
struct Programm;
struct Dual;
struct Unary;
struct If;
struct While;
struct Let;
struct Assign;
struct Not;
struct And;
struct Equal;
struct Lower;
struct LowerEqual;
struct Plus;
struct Minus;
struct Times;
struct Div;
struct Pow;
struct Minus1;
struct IsNull;
struct Exprxx;
struct Args;
struct Function;
struct Dot;
struct New;
struct ObjID;
struct Literal;
struct IntLit;
struct StrLit;
struct BoolLit;
struct Lpar;
struct Rpar;
struct Parenthese;



class Visitor
{
public:

    SymbolTable st;

    virtual std::string visit(ASTnode* )=0;
    virtual std::string visit(Expr *)=0;
    virtual std::string visit(Type *)=0;
    virtual std::string visit(Field *)=0;
    virtual std::string visit(Formal *)=0;
    virtual std::string visit(Formalx *)=0;
    virtual std::string visit(Formals *)=0;
    virtual std::string visit(Exprx *)=0;
    virtual std::string visit(Exprxx *)=0;
    virtual std::string visit(Block *)=0;
    virtual std::string visit(Method *)=0;
    virtual std::string visit(FieldMethod *)=0;
    virtual std::string visit(Body *)=0;
    virtual std::string visit(Classe *)=0;
    virtual std::string visit(Classes *)=0;
    virtual std::string visit(Programm *)=0;
    virtual std::string visit(Dual *)=0;
    virtual std::string visit(Unary *)=0;
    virtual std::string visit(If *)=0;
    virtual std::string visit(While *)=0;
    virtual std::string visit(Let *)=0;
    virtual std::string visit(Assign *)=0;
    virtual std::string visit(Not *)=0;
    virtual std::string visit(And *)=0;
    virtual std::string visit(Equal *)=0;
    virtual std::string visit(Lower *)=0;
    virtual std::string visit(LowerEqual *)=0;
    virtual std::string visit(Plus *)=0;
    virtual std::string visit(Minus *)=0;
    virtual std::string visit(Times *)=0;
    virtual std::string visit(Div *)=0;
    virtual std::string visit(Pow *)=0;
    virtual std::string visit(Minus1 *)=0;
    virtual std::string visit(IsNull *)=0;
    virtual std::string visit(Args *)=0;
    virtual std::string visit(Function *)=0;
    virtual std::string visit(Dot *)=0;
    virtual std::string visit(New *)=0;
    virtual std::string visit(ObjID *)=0;
    virtual std::string visit(Literal *)=0;
    virtual std::string visit(IntLit *)=0;
    virtual std::string visit(StrLit *)=0;
    virtual std::string visit(BoolLit *)=0;
    virtual std::string visit(Lpar *)=0;
    virtual std::string visit(Rpar *)=0;
    virtual std::string visit(Parenthese *)=0;
};


#endif