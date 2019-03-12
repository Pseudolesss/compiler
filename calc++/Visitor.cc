#include <string>
#include "ASTnode.hh"


std::string Visitor::visit(ASTnode* asTnode){return "1-";}

std::string Visitor::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN";}

std::string Visitor::visit(Type *type){return type->getID();}

std::string Visitor::visit(Field *field){return "2-";}

std::string Visitor::visit(Format *format){return "3-";}

std::string Visitor::visit(Formalx *formalx){return "4-";}

std::string Visitor::visit(Formals *formals){return "5-";}

std::string Visitor::visit(Exprx *exprx){return "6-";}

std::string Visitor::visit(Exprxx *exprxx){return "7-";}

std::string Visitor::visit(Block *block){return "8-";}

std::string Visitor::visit(Method *method){return "9-";}

std::string Visitor::visit(FieldMethod *fieldMethod){return "10-";}

std::string Visitor::visit(Body *body){return "11-";}

std::string Visitor::visit(Classe *classe){return "12-";}

std::string Visitor::visit(Classes *classes){
    if(classes->nextClass() == nullptr)
        return classes->getClass()->accept(this);
    else
        return classes->getClass()->accept(this) + ", " + classes->nextClass()->accept(this);
}

std::string Visitor::visit(Programm *programm){return "[" + programm->getClasses()->accept(this) + "]";}

std::string Visitor::visit(Dual *dual){return "13-";}

std::string Visitor::visit(Unary *unary){return "14-";}

std::string Visitor::visit(If *anIf){return "15-";}

std::string Visitor::visit(While *aWhile){return "16-";}

std::string Visitor::visit(Let *let){return "17-";}

std::string Visitor::visit(Assign *assign){return "18-";}

std::string Visitor::visit(Not *aNot){return "19-";}

std::string Visitor::visit(And *anAnd){return "20-";}

std::string Visitor::visit(Equal *equal){return "21-";}

std::string Visitor::visit(Lower *lower){return "22-";}

std::string Visitor::visit(LowerEqual *lowerEqual){return "23-";}

std::string Visitor::visit(Plus *plus){return "24-";}

std::string Visitor::visit(Minus *minus){return "25-";}

std::string Visitor::visit(Times *times){return "26-";}

std::string Visitor::visit(Div *div){return "27-";}

std::string Visitor::visit(Pow *pow){return "28-";}

std::string Visitor::visit(Minus1 *minus1){return "29-";}

std::string Visitor::visit(IsNull *isNull){return "30-";}

std::string Visitor::visit(Args *args){return "31-";}

std::string Visitor::visit(Function *function){return "32-";}

std::string Visitor::visit(Dot *dot){return "33-";}

std::string Visitor::visit(New *aNew){return "34-";}

std::string Visitor::visit(ObjID *objID){return "35-";}

std::string Visitor::visit(Literal *literal){return "36-";}

std::string Visitor::visit(IntLit *intLit){return "37-";}

std::string Visitor::visit(StrLit *strLit){return "38-";}

std::string Visitor::visit(BoolLit *boolLit){return "39-";}

std::string Visitor::visit(Lpar *lpar){return "40-";}

std::string Visitor::visit(Rpar *rpar){return "41-";}

std::string Visitor::visit(Parenthese *parenthese){return "42-";}



//