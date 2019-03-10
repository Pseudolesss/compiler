#include <string>
#include "Visitor.hh"

template <class T>
class Visitor{

    public:
    T visit(Type* type){
        return type->getType();
    }

    T visit(Field* field){
        if(field->getExpr()->isEmpty()){
            return "Field(" + field->getID() + ", " + this->visit(field->getType()) + ")";
        }
        else{
            return "Field(" + field->getID() + ", " + this->visit(field->getType()) +", " + this.viExpr(field->getExpr()) +  ")";
        }
    }

    T visit(Formal* formal){
        return formal->getID() + " : " + formal->getType().getType();
    }

    T visit(Formalx* formalx){
        if(formalx->getFormal()->isEmpty() || formalx->getFormalx()->isEmpty())
            return "";
        else
            return ", " + this->visit(formalx->getFormal()) + this->visit(formalx->getFormalx());
    }

    T visit(Formals* formals){
        if(formalx->getFormal().isEmpty() || formalx->getFormalx().isEmpty())
            return "[]";
        else
            return "[" + this->visit(formalx->getFormal()) + this->visit(formalx->getFormalx()) + "]";
    }

    template <typename E>
    T visit(Expr<E>* expr){
        return this->visit(expr->getData());
    }

    T visit(Exprx* exprx){
        if(exprx->getExpr()->isEmpty() || exprx->getExprx()->isEmpty())
            return "";
        else
            return ", " + this->visit(exprx->getExpr()) + this->visit(exprx->getExprx());
    }

    T visit(Block* block) {
        return "[" + this->visit(block->getExpr()) + this->visit(block->getExprx()) + "]";
    }

    T visit(Method* method){
        return "Method(" + this->visit(method->getType()) + ", " + this->visit(method->getFormals()) + ", " + this->visit(method->getType())+ ", " + this.visit(method->getBlock()) + ")";
    }
    // Cette méthode doit renvoyer des listes je pense
    T visit(FieldMethod* fieldMethod){
        if(fieldMethod->getMethod()->isEmpty())
            return "/TODO";//TODO
        else
            return "TODO";//TODO
    }

    T visit()


};

