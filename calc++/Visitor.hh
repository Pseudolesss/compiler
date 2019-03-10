#include "ASTnode.hh"
//inclure visit.cpp pour le compilateur.
template <class T>
class Visitor
{
  public:
    T visit(ASTnode* );
    T visit(Expr *);//
    T visit(Type *);//
    T visit(Field *);//
    T visit(Format *);//
    T visit(Formalx *);//
    T visit(Formals *);//
    T visit(Exprx *);//
    T visit(Exprxx *);
    T visit(Block *);//
    T visit(Method *);//
    T visit(FieldMethod *);//
    T visit(Body *);
    T visit(Classe *);
    T visit(Classes *);
    T visit(Programm *);
    T visit(Dual *);
    T visit(Unary *);
    T visit(If *);
    T visit(While *);
    T visit(Let *);
    T visit(Assign *);
    T visit(Not *);
    T visit(And *);
    T visit(Equal *);
    T visit(Lower *);
    T visit(LowerEqual *);
    T visit(Plus *);
    T visit(Minus *);
    T visit(Times *);
    T visit(Div *);
    T visit(Pow *);
    T visit1(Minus1 *);
    T visit(IsNull *);
    T visit(Exprxx*);
    T visit(Args *);
    T visit(Function *);
    T visit(Dot *);
    T visit(New *);
    T visit(ObjID *);
    T visit(Literal *);
    T visit(IntList *);
    T visit(StrLit *);
    T visit(BoolLit *);
    T visit(Lpar *);
    T visit(Rpar *);
    T visit(Parenthese *);
}