#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "CheckPrinter.hh"


std::string CheckPrinter::visit(ASTnode* asTnode){return "NOT SUPPOSED TO HAPPEN ASTNODE";}

std::string CheckPrinter::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN EXPRESSION";}

std::string CheckPrinter::visit(Type *type){return type->getID();}

std::string CheckPrinter::visit(Field *field){
    if(field->getExpr() == nullptr){
        return "Field(" + field->getID() + ", " + field->getType()->accept(this) + " )";
    }
    return "Field(" + field->getID() + ", " + field->getType()->accept(this) + ", " + field->getExpr()->accept(this) + " )";
}

std::string CheckPrinter::visit(Formal *formal){return formal->getID() + " : " + formal->getType()->accept(this);}

std::string CheckPrinter::visit(Formalx *formalx){
    if(formalx->getFormal() == nullptr)
        return "";
    else
        return ", " + formalx->getFormal()->accept(this) + formalx->getFormalx()->accept(this);
}

std::string CheckPrinter::visit(Formals *formals){
    if(formals->getFormal() == nullptr)
        return "[]";
    else
        return "[" + formals->getFormal()->accept(this) + formals->getFormalx()->accept(this) + "]";
}

std::string CheckPrinter::visit(Exprx *exprx){
    if(exprx->getExprx() == nullptr)
        return "";
    return ", " + exprx->getExpr()->accept(this) + exprx->getExprx()->accept(this);
}

std::string CheckPrinter::visit(Exprxx *exprxx){
    if(exprxx->getExpr() == nullptr){
        return "";
    }
    else{
        return "," + exprxx->getExprxx()->accept(this);
    }
}

std::string CheckPrinter::visit(Block *block){

    if(block->getExprx()->getExpr() == nullptr)
        return  block->getExpr()->accept(this) + block->getExprx()->accept(this);
    else
        return "[" +  block->getExpr()->accept(this) + block->getExprx()->accept(this)+ "]" + " : " + block->getDataType();
}

std::string CheckPrinter::visit(Method *method){return "Method(" + method->getID()+ ", " + method->getFormals()->accept(this) + ", " + method->getType()->accept(this) + ", " + method->getBlock()->accept(this) + ")" ;}

std::string CheckPrinter::visit(FieldMethod *fieldMethod){
    if(fieldMethod->getFieldMethod() == nullptr)
        return "";
    if(fieldMethod->getMethod() == nullptr)
        return fieldMethod->getFieldMethod()->accept(this) + fieldMethod->getField()->accept(this) + "$";
    else
        return fieldMethod->getFieldMethod()->accept(this) + fieldMethod->getMethod()->accept(this) + "$";
}

std::string CheckPrinter::visit(Body *body){

    std::vector<std::string> m;
    std::vector<std::string> f;
    std::vector<std::string> tokens;

    std::string text =  body->getFieldMethod()->accept(this);

    while(!text.empty()){
        tokens.push_back(text.substr(0, text.find('$')  ));
        text = text.erase(0, text.find('$') + 1);

    }

    for (const auto& t : tokens) {
        if(t[0] == 'M')
            m.push_back(t);
        else if(t[0] == 'F')
            f.push_back(t);
        else
            m.push_back("ERROR: " + t);
    }

    std::string ret;

    // if no element
    if(f.empty())
        ret += "[], ";
    else
        for(int i = 0; i < f.size(); i++){
            // if one element only
            if(f.size()==1){
                ret += "[" + f[i] + "], ";
                continue;
            }
            // initial elem for size > 1
            if(i == 0){
                ret += "[" + f[i];
                continue;
            }
            // last elem for size > 1
            if(i == f.size()-1){
                ret += ", " + f[i] + "] ,";
                continue;
            }
            // intermediate for size > 1
            ret += ", " + f[i];

        }

    // if no element
    if(m.empty())
        ret += "[]";
    else
        for(int i = 0; i < m.size(); i++){
            // if one element only
            if(m.size()==1){
                ret += "[" + m[i] + "]";
                continue;
            }
            // initial elem for size > 1
            if(i == 0){
                ret += "[" + m[i];
                continue;
            }
            // last elem for size > 1
            if(i == m.size()-1){
                ret += ", " + m[i] + "]";
                continue;
            }
            // intermediate for size > 1
            ret += ", " + m[i];

        }


    return ret ;
}

std::string CheckPrinter::visit(Classe *classe){
    if(classe->getParentID().empty())
        return "Class(" + classe->getTypeID() + ", Object, " + classe->getBody()->accept(this) + ")";
    else
        return "Class(" + classe->getTypeID() + ", " + classe->getParentID() + ", " + classe->getBody()->accept(this) + ")";
}

std::string CheckPrinter::visit(Classes *classes){
    if(classes->nextClass() == nullptr)
        return classes->getClass()->accept(this);
    else
        return classes->nextClass()->accept(this) + ", " + classes->getClass()->accept(this);
}

std::string CheckPrinter::visit(Programm *programm){return "[" + programm->getClasses()->accept(this) + "]";}

std::string CheckPrinter::visit(Dual *dual){return "SHOULD NOT HAPPEND: DUAL";}

std::string CheckPrinter::visit(Unary *unary){return "SHOULD NOT HAPPEND: UNARY";}

std::string CheckPrinter::visit(If *anIf){
    if(anIf->getElse() == nullptr){
        return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen()->accept(this) + ")" + " : " + anIf->getDataType();
    }
    return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen()->accept(this) + ", " + anIf->getElse()->accept(this) + ")";
}

std::string CheckPrinter::visit(While *aWhile){return "While(" + aWhile->getWhile()->accept(this) + ", " + aWhile->getDo()->accept(this) + ")" + " : " + aWhile->getDataType();}

std::string CheckPrinter::visit(Let *let){
    if(let->getAssign() == nullptr){
        return "Let(" + let->getObjID() + ", " + let->getType()->accept(this) + ", " + let->getIn()->accept(this) + ")";
    }
    return "Let(" + let->getObjID() + ", " + let->getType()->accept(this)+ ", " + let->getIn()->accept(this) + ", " + let->getAssign()->accept(this) + ")" +  " : " + let->getDataType();
}

std::string CheckPrinter::visit(Assign *assign){return "Assign(" + assign->getObjID() + ", " + assign->getExpr()->accept(this) + ")" +  " : " + assign->getDataType();}

std::string CheckPrinter::visit(Not *aNot){return "UnOp(not, " + aNot->getExpr()->accept(this) + ")" + " : " + aNot->getDataType();}

std::string CheckPrinter::visit(And *anAnd){return "BinOp(and, " + anAnd->getLeft()->accept(this) + ", " + anAnd->getRight()->accept(this) + ")" + + " : " + anAnd->getDataType();}

std::string CheckPrinter::visit(Equal *equal){return "BinOp(=, " + equal->getLeft()->accept(this) + ", " + equal->getRight()->accept(this) + ")" + + " : " + equal->getDataType();}

std::string CheckPrinter::visit(Lower *lower){return "BinOp(<, " + lower->getLeft()->accept(this) + ", " + lower->getRight()->accept(this) + ")" + " : " + lower->getDataType();}

std::string CheckPrinter::visit(LowerEqual *lowerEqual){return "BinOp(<=, " + lowerEqual->getLeft()->accept(this) + ", " + lowerEqual->getRight()->accept(this) + ")" + " : " + lowerEqual->getDataType();;}

std::string CheckPrinter::visit(Plus *plus){return "BinOp(+, " + plus->getLeft()->accept(this) + ", " + plus->getRight()->accept(this) + ")" + + " : " + plus->getDataType();;}

std::string CheckPrinter::visit(Minus *minus){return "BinOp(-, " + minus->getLeft()->accept(this) + ", " + minus->getRight()->accept(this) + ")" + + " : " + minus->getDataType();;}

std::string CheckPrinter::visit(Times *times){return "BinOp(*, " + times->getLeft()->accept(this) + ", " + times->getRight()->accept(this) + ")" + + " : " + times->getDataType();;}

std::string CheckPrinter::visit(Div *div){return "BinOp(/, " + div->getLeft()->accept(this) + ", " + div->getRight()->accept(this) + ")"+ " : " + div->getDataType();;}

std::string CheckPrinter::visit(Pow *pow){return "BinOp(^, " + pow->getLeft()->accept(this) + ", " + pow->getRight()->accept(this) + ")" + " : " + pow->getDataType();}

std::string CheckPrinter::visit(Minus1 *minus1){return "UnOp(-, " + minus1->getExpr()->accept(this) + ")" + " : " + minus1->getDataType();}

std::string CheckPrinter::visit(IsNull *isNull){return "UnOp(isnull, " + isNull->getExpr()->accept(this) + ")" + " : " + isNull->getDataType();}

std::string CheckPrinter::visit(Args *args){
    if(args->getExpr() != nullptr){
        return args->getExpr()->accept(this);
    }
    else{
        return "";
    }
}

std::string CheckPrinter::visit(Function *function){return "Call(self, " + function->getID() +  ", [" +function->getArgs()->accept(this) + "])" + " : " + function->getDataType();}

std::string CheckPrinter::visit(Dot *dot){return "Call(" + dot->getExpr()->accept(this) + "," + dot->getID() + ",[" + dot->getArgs()->accept(this) + "])" + " : " + dot->getDataType();}

std::string CheckPrinter::visit(New *aNew){return "New(" + aNew->getTypeID() + ")" + " : " + aNew->getDataType();}

std::string CheckPrinter::visit(ObjID *objID){return objID->getID() + " : " + objID->getDataType();}

std::string CheckPrinter::visit(Literal *literal){return "NOT SUPPOSED TO HAPPEN: LITERAL";}

std::string CheckPrinter::visit(IntLit *intLit){return std::to_string(intLit->getValue()) + " : " + intLit->getDataType();}

std::string CheckPrinter::visit(StrLit *strLit){return strLit->getValue() + " : " + strLit->getDataType();;}

std::string CheckPrinter::visit(BoolLit *boolLit){return  std::string(boolLit->getValue() ? "true" : "false") + " : " + boolLit->getDataType();}

std::string CheckPrinter::visit(Lpar *lpar){return "("; + " : " + lpar->getDataType();}

std::string CheckPrinter::visit(Rpar *rpar){return std::string(")") + " : " + rpar->getDataType();}

std::string CheckPrinter::visit(Parenthese *parenthese){return parenthese->getExpr()->accept(this) + " : " + parenthese->getDataType();}



//