#include <string>
#include "ASTnode.hh"


std::string Visitor::visit(ASTnode* asTnode){return "NOT SUPPOSED TO HAPPEN ASTNODE";}

std::string Visitor::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN EXPRESSION";}

std::string Visitor::visit(Type *type){return type->getID();}

std::string Visitor::visit(Field *field){return "2-";}

std::string Visitor::visit(Format *format){return "3-";}

std::string Visitor::visit(Formalx *formalx){return "4-";}

std::string Visitor::visit(Formals *formals){return "5-";}

std::string Visitor::visit(Exprx *exprx){return exprx->getExpr()->accept(this) + ", " + exprx->getExprx()->accept(this);}

std::string Visitor::visit(Exprxx *exprxx){
    if(exprxx->getExpr() == nullptr){
        return "";
    }
    else{
        return "," + exprxx->getExprxx()->accept(this);
    }
}

std::string Visitor::visit(Block *block){return "[" +  block->getExpr()->accept(this) + block->getExprx()->accept(this)+ "]";}

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

std::string Visitor::visit(Dual *dual){return "SHOULD NOT HAPPEND";}

std::string Visitor::visit(Unary *unary){return "SHOULD NOT HAPPEND";}

std::string Visitor::visit(If *anIf){
    if(anIf->getElse() == nullptr){
        return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen() + ")";
    }
    return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen() + ", " + anIf->getElse()->accept(this) + ")";
}

std::string Visitor::visit(While *aWhile){return "While(" + aWhile->getWhile()->accept(this) + ", " + aWhile->getDo()->accept(this) + ")";}

std::string Visitor::visit(Let *let){
    if(let->getIn() == nullptr){
        return "Let(" + let->getObjID() + ", "let->getType()->accept(this) + ", " + let->getAssign()->accept() + ")";
    }
    return "Let(" + let->getObjID() + ", "let->getType()->accept(this)+ ", " + let->getIn()->accept(this) + ", " + let->getAssign()->accept() + ")";
}

std::string Visitor::visit(Assign *assign){return "Assign(" + assign->getObjID() + ", " + assign->getExpr()->accept(this) + ")";}

std::string Visitor::visit(Not *aNot){return "UnOp(not, " + aNot->getExpr->accept(this) + ")";}

std::string Visitor::visit(And *anAnd){return "BinOp(" + "and" + ", " + anAnd->getLeft()->accept(this) + ", " + anAnd->getRight()->accept(this) + ")";}

std::string Visitor::visit(Equal *equal){return "BinOp(" + "=" + ", " + equal->getLeft()->accept(this) + ", " + equal->getRight()->accept(this) + ")";}

std::string Visitor::visit(Lower *lower){return "BinOp(" + "<" + ", " + lower->getLeft()->accept(this) + ", " + lower->getRight()->accept(this) + ")";}

std::string Visitor::visit(LowerEqual *lowerEqual){return "BinOp(" + "<=" + ", " + lowerEqual->getLeft()->accept(this) + ", " + lowerEqual->getRight()->accept(this) + ")";}

std::string Visitor::visit(Plus *plus){return "BinOp(" + "+" + ", " + plus->getLeft()->accept(this) + ", " + plus->getRight()->accept(this) + ")";}

std::string Visitor::visit(Minus *minus){return "BinOp(" + "-" + ", " + minus->getLeft()->accept(this) + ", " + minus->getRight()->accept(this) + ")";}

std::string Visitor::visit(Times *times){return "BinOp(" + "*" + ", " + times->getLeft()->accept(this) + ", " + times->getRight()->accept(this) + ")";}

std::string Visitor::visit(Div *div){return "BinOp(" + "/" + ", " + div->getLeft()->accept(this) + ", " + div->getRight()->accept(this) + ")";}

std::string Visitor::visit(Pow *pow){return "BinOp(" + "^" + ", " + pow->getLeft()->accept(this) + ", " + pow->getRight()->accept(this) + ")";}

std::string Visitor::visit(Minus1 *minus1){return "UnOp(-, " + minus1->getExpr()->accept(this) + ")";}

std::string Visitor::visit(IsNull *isNull){return "UnOp(isnull," + isNull->getExpr()->accept(this) + ")";}

std::string Visitor::visit(Args *args){
    if(args.expr != nullptr){
        return args->getExpr()->accept(this);
    }
    else{
        return "";
    }
}

std::string Visitor::visit(Function *function){return "Call,self,[" +function->getArgs()->accept(this) + "]";}

std::string Visitor::visit(Dot *dot){return "Call(" + dot->getExpr()->accept(this) + "," + dot->getID() + ",[" + dot->getArgs()->accept(this) + "]";}

std::string Visitor::visit(New *aNew){return "New(" + aNew->getTypeID + ")";}

std::string Visitor::visit(ObjID *objID){return objID->getID();}

std::string Visitor::visit(Literal *literal){return "NOT SUPPOSED TO HAPPEN LITERAL";}

std::string Visitor::visit(IntLit *intLit){return std::to_string(intLit->getValue());}

std::string Visitor::visit(StrLit *strLit){return strLit->getValue();}

std::string Visitor::visit(BoolLit *boolLit){return ( boolLit->getValue() ? "true" : "false";}

std::string Visitor::visit(Lpar *lpar){return "(";}

std::string Visitor::visit(Rpar *rpar){return ")";}

std::string Visitor::visit(Parenthese *parenthese){return parenthese->getExpr()->accept(this);}



//