
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

    virtual std::string visit(ASTnode* );
    virtual std::string visit(Expr *);//
    virtual std::string visit(Type *);//
    virtual std::string visit(Field *);//
    virtual std::string visit(Formal *);//
    virtual std::string visit(Formalx *);//
    virtual std::string visit(Formals *);//
    virtual std::string visit(Exprx *);//
    virtual std::string visit(Exprxx *);
    virtual std::string visit(Block *);//
    virtual std::string visit(Method *);//
    virtual std::string visit(FieldMethod *);//
    virtual std::string visit(Body *);
    virtual std::string visit(Classe *);
    virtual std::string visit(Classes *);
    virtual std::string visit(Programm *);
    virtual std::string visit(Dual *);
    virtual std::string visit(Unary *);
    virtual std::string visit(If *);
    virtual std::string visit(While *);
    virtual std::string visit(Let *);
    virtual std::string visit(Assign *);
    virtual std::string visit(Not *);
    virtual std::string visit(And *);
    virtual std::string visit(Equal *);
    virtual std::string visit(Lower *);
    virtual std::string visit(LowerEqual *);
    virtual std::string visit(Plus *);
    virtual std::string visit(Minus *);
    virtual std::string visit(Times *);
    virtual std::string visit(Div *);
    virtual std::string visit(Pow *);
    virtual std::string visit(Minus1 *);
    virtual std::string visit(IsNull *);
    virtual std::string visit(Args *);
    virtual std::string visit(Function *);
    virtual std::string visit(Dot *);
    virtual std::string visit(New *);
    virtual std::string visit(ObjID *);
    virtual std::string visit(Literal *);
    virtual std::string visit(IntLit *);
    virtual std::string visit(StrLit *);
    virtual std::string visit(BoolLit *);
    virtual std::string visit(Lpar *);
    virtual std::string visit(Rpar *);
    virtual std::string visit(Parenthese *);
};


#endif