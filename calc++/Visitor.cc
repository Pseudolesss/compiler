#include <string>
#include "ASTnode.hh"
#include <boost/tokenizer.hpp>


std::string Visitor::visit(ASTnode* asTnode){return "NOT SUPPOSED TO HAPPEN ASTNODE";}

std::string Visitor::visit(Expr * expr){return "NOT SUPPOSED TO HAPPEN EXPRESSION";}

std::string Visitor::visit(Type *type){return type->getID();}

std::string Visitor::visit(Field *field){
    if(field->getExpr() == nullptr){
        return "Field(" + field->getID() + ", " + field->getType()->accept(this) + " )";
    }
    return "Field(" + field->getID() + ", " + field->getType()->accept(this) + ", " + field->getExpr()->accept(this) + " )";
}

std::string Visitor::visit(Formal *formal){return formal->getID() + " : " + formal->getType()->accept(this);}

std::string Visitor::visit(Formalx *formalx){
    if(formalx->getFormal() == nullptr)
        return "";
    else
        return ", " + formalx->getFormal()->accept(this) + formalx->getFormalx()->accept(this);
}

std::string Visitor::visit(Formals *formals){
    if(formals->getFormal() == nullptr)
        return "[]";
    else
        return "[" + formals->getFormal()->accept(this) + formals->getFormalx()->accept(this) + "]";
}

std::string Visitor::visit(Exprx *exprx){
    if(exprx->getExprx() == nullptr)
        return "";
    return ", " + exprx->getExpr()->accept(this) + exprx->getExprx()->accept(this);
}

std::string Visitor::visit(Exprxx *exprxx){
    if(exprxx->getExpr() == nullptr){
        return "";
    }
    else{
        return "," + exprxx->getExprxx()->accept(this);
    }
}

std::string Visitor::visit(Block *block){return "[" +  block->getExpr()->accept(this) + block->getExprx()->accept(this)+ "]";}

std::string Visitor::visit(Method *method){return "Method(" + method->getID()+ ", " + method->getFormals()->accept(this) + ", " + method->getType()->accept(this) + ", " + method->getBlock()->accept(this) + ")" ;}

std::string Visitor::visit(FieldMethod *fieldMethod){
    if(fieldMethod->getFieldMethod() == nullptr)
        return "";
    if(fieldMethod->getMethod() == nullptr)
        return fieldMethod->getField()->accept(this) + "$" + fieldMethod->getField()->accept(this);
    else
        return fieldMethod->getMethod()->accept(this) + "$" + fieldMethod->getField()->accept(this);
}

std::string Visitor::visit(Body *body){

    std::vector<std::string> m;
    std::vector<std::string> f;

    std::string text =  body->getFieldMethod()->accept(this);

    boost::char_separator<char> sep("$");
    boost::tokenizer<boost::char_separator<char>> tokens(text, sep);
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


    return ret;
}

std::string Visitor::visit(Classe *classe){
    if(classe->getParentID().empty())
        return "Class(" + classe->getTypeID() + ", Object, " + classe->getBody()->accept(this) + ")";
    else
        return "Class(" + classe->getTypeID() + ", " + classe->getParentID() + ", " + classe->getBody()->accept(this) + ")";
}

std::string Visitor::visit(Classes *classes){
    if(classes->nextClass() == nullptr)
        return classes->getClass()->accept(this);
    else
        return classes->nextClass()->accept(this) + ", " + classes->getClass()->accept(this);
}

std::string Visitor::visit(Programm *programm){return "[" + programm->getClasses()->accept(this) + "]";}

std::string Visitor::visit(Dual *dual){return "SHOULD NOT HAPPEND: DUAL";}

std::string Visitor::visit(Unary *unary){return "SHOULD NOT HAPPEND: UNARY";}

std::string Visitor::visit(If *anIf){
    if(anIf->getElse() == nullptr){
        return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen()->accept(this) + ")";
    }
    return "If(" + anIf->getIf()->accept(this) + ", " + anIf->getThen()->accept(this) + ", " + anIf->getElse()->accept(this) + ")";
}

std::string Visitor::visit(While *aWhile){return "While(" + aWhile->getWhile()->accept(this) + ", " + aWhile->getDo()->accept(this) + ")";}

std::string Visitor::visit(Let *let){
    if(let->getIn() == nullptr){
        return "Let(" + let->getObjID() + ", " + let->getType()->accept(this) + ", " + let->getAssign()->accept(this) + ")";
    }
    return "Let(" + let->getObjID() + ", " + let->getType()->accept(this)+ ", " + let->getIn()->accept(this) + ", " + let->getAssign()->accept(this) + ")";
}

std::string Visitor::visit(Assign *assign){return "Assign(" + assign->getObjID() + ", " + assign->getExpr()->accept(this) + ")";}

std::string Visitor::visit(Not *aNot){return "UnOp(not, " + aNot->getExpr()->accept(this) + ")";}

std::string Visitor::visit(And *anAnd){return "BinOp(and, " + anAnd->getLeft()->accept(this) + ", " + anAnd->getRight()->accept(this) + ")";}

std::string Visitor::visit(Equal *equal){return "BinOp(=, " + equal->getLeft()->accept(this) + ", " + equal->getRight()->accept(this) + ")";}

std::string Visitor::visit(Lower *lower){return "BinOp(<, " + lower->getLeft()->accept(this) + ", " + lower->getRight()->accept(this) + ")";}

std::string Visitor::visit(LowerEqual *lowerEqual){return "BinOp(<=, " + lowerEqual->getLeft()->accept(this) + ", " + lowerEqual->getRight()->accept(this) + ")";}

std::string Visitor::visit(Plus *plus){return "BinOp(+, " + plus->getLeft()->accept(this) + ", " + plus->getRight()->accept(this) + ")";}

std::string Visitor::visit(Minus *minus){return "BinOp(-, " + minus->getLeft()->accept(this) + ", " + minus->getRight()->accept(this) + ")";}

std::string Visitor::visit(Times *times){return "BinOp(*, " + times->getLeft()->accept(this) + ", " + times->getRight()->accept(this) + ")";}

std::string Visitor::visit(Div *div){return "BinOp(/, " + div->getLeft()->accept(this) + ", " + div->getRight()->accept(this) + ")";}

std::string Visitor::visit(Pow *pow){return "BinOp(^, " + pow->getLeft()->accept(this) + ", " + pow->getRight()->accept(this) + ")";}

std::string Visitor::visit(Minus1 *minus1){return "UnOp(-, " + minus1->getExpr()->accept(this) + ")";}

std::string Visitor::visit(IsNull *isNull){return "UnOp(isnull, " + isNull->getExpr()->accept(this) + ")";}

std::string Visitor::visit(Args *args){
    if(args->getExpr() != nullptr){
        return args->getExpr()->accept(this);
    }
    else{
        return "";
    }
}

std::string Visitor::visit(Function *function){return "Call,self,[" +function->getArgs()->accept(this) + "]";}

std::string Visitor::visit(Dot *dot){return "Call(" + dot->getExpr()->accept(this) + "," + dot->getID() + ",[" + dot->getArgs()->accept(this) + "]";}

std::string Visitor::visit(New *aNew){return "New(" + aNew->getTypeID() + ")";}

std::string Visitor::visit(ObjID *objID){return objID->getID();}

std::string Visitor::visit(Literal *literal){return "NOT SUPPOSED TO HAPPEN: LITERAL";}

std::string Visitor::visit(IntLit *intLit){return std::to_string(intLit->getValue());}

std::string Visitor::visit(StrLit *strLit){return strLit->getValue();}

std::string Visitor::visit(BoolLit *boolLit){return  boolLit->getValue() ? "true" : "false";}

std::string Visitor::visit(Lpar *lpar){return "(";}

std::string Visitor::visit(Rpar *rpar){return ")";}

std::string Visitor::visit(Parenthese *parenthese){return parenthese->getExpr()->accept(this);}



//