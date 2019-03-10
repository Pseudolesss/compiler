#include "ASTnode.hh"
//inclure Visitor.cpp pour le compilateur.
template <class T>
class Visitor
{
  public:
    <T> viASTnode(ASTnode* );
    <T> viExpr(Expr *);
    <T> viType(Type *);
    <T> viField(Field *);
    <T> viFormat(Format *);
    <T> viFormalx(Formalx *);
    <T> viFormals(Formals *);
    <T> viExprx(Exprx *);
    <T> viExpr(Expr *);
    <T> viMethod(Method *);
    <T> viFieldMethod(FieldMethod *);
    <T> viBody(Body *);
    <T> viClasse(Classe *);
    <T> viClasses(Classes *);
    <T> viProgramm(Programm *);
    <T> viDual(Dual *);
    <T> viUnary(Unary *);
    <T> viIf(If *);
    <T> viWhile(While *);
    <T> viLet(Let *);
    <T> viAssign(Assign *);
    <T> viNot(Not *);
    <T> viAnd(And *);
    <T> viEqual(Equal *);
    <T> viLower(Lower *);
    <T> viLowerEqual(LowerEqual *);
    <T> viPlus(Plus *);
    <T> viMinus(Minus *);
    <T> viTimes(Times *);
    <T> viDiv(Div *);
    <T> viPow(Pow *);
    <T> viMinus1(Minus1 *);
    <T> viIsNull(IsNull *);
    <T> viExprxx(Exprxx);
    <T> viArgs(Args *);
    <T> viFunction(Function *);
    <T> viDot(Dot *);
    <T> viNew(New *);
    <T> viObjID(ObjID *);
    <T> viLiteral(Literal *);
    <T> viIntList(IntList *);
    <T> viStrLit(StrLit *);
    <T> viBoolLit(BoolLit *);
    <T> viLpar(Lpar *);
    <T> viRpar(Rpar *);
    <T> viParenthese(Parenthese *);
}