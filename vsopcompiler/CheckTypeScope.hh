#ifndef CHECKTYPESCOPE
#define CHECKTYPESCOPE

#include "Visitor.hh"
#include <string>

class CheckTypeScope : public Visitor
{
public:
  std::string visit(ASTnode *);
  std::string visit(Expr *);
  std::string visit(Type *);
  std::string visit(Field *);
  std::string visit(Formal *);
  std::string visit(Formalx *);
  std::string visit(Formals *);
  std::string visit(Exprx *);
  std::string visit(Exprxx *);
  std::string visit(Block *);
  std::string visit(Method *);
  std::string visit(FieldMethod *);
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

private:
  std::string classID;
  std::string methodID;
};

extern std::string self_classID;


#endif