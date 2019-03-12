
#ifndef VISITOR_H
#define VISITOR_H

struct ASTnode;
struct Expr;
struct Type;
struct Field;
struct Format;
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
    std::string visit(ASTnode* );
    std::string visit(Expr *);//
    std::string visit(Type *);//
    std::string visit(Field *);//
    std::string visit(Format *);//
    std::string visit(Formalx *);//
    std::string visit(Formals *);//
    std::string visit(Exprx *);//
    std::string visit(Exprxx *);
    std::string visit(Block *);//
    std::string visit(Method *);//
    std::string visit(FieldMethod *);//
    std::string visit(Body *);
    std::string visit(Classe *);
    std::string visit(Classes *);
    std::string visit(Programm *);
    std::string visit(Dual *);
    std::string visit(Unary *);
    std::string visit(If *);
    std::string visit(While *);
    std::string visit(Let *);
    std::string visit(Assign *);
    std::string visit(Not *);
    std::string visit(And *);
    std::string visit(Equal *);
    std::string visit(Lower *);
    std::string visit(LowerEqual *);
    std::string visit(Plus *);
    std::string visit(Minus *);
    std::string visit(Times *);
    std::string visit(Div *);
    std::string visit(Pow *);
    std::string visit(Minus1 *);
    std::string visit(IsNull *);
    std::string visit(Args *);
    std::string visit(Function *);
    std::string visit(Dot *);
    std::string visit(New *);
    std::string visit(ObjID *);
    std::string visit(Literal *);
    std::string visit(IntLit *);
    std::string visit(StrLit *);
    std::string visit(BoolLit *);
    std::string visit(Lpar *);
    std::string visit(Rpar *);
    std::string visit(Parenthese *);
};


#endif