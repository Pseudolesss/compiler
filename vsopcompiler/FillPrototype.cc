#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "prototype.hh"
#include "FillPrototype.hh"

    std::string FillPrototype::visit(ASTnode *asTnode) { return "TODO"; }

    std::string FillPrototype::visit(Expr *expr) { return "TODO"; }

    std::string FillPrototype::visit(Type *type) { return "TODO"; }

    std::string FillPrototype::visit(Field *field)
    {
        if (field->getExpr() == nullptr)
        {
            return "TODO";
        }
        return "TODO";
    }

    std::string FillPrototype::visit(Formal *formal) { return "TODO"; }

    std::string FillPrototype::visit(Formalx *formalx)
    {
        if (formalx->getFormal() == nullptr)
            return "TODO";
        else
            return "TODO";
    }

    std::string FillPrototype::visit(Formals *formals)
    {
        if (formals->getFormal() == nullptr)
            return "TODO";
        else
            return "TODO";
    }

    std::string FillPrototype::visit(Exprx *exprx)
    {
        if (exprx->getExprx() == nullptr)
            return "TODO";
        return "TODO";
    }

    std::string FillPrototype::visit(Exprxx *exprxx)
    {
        if (exprxx->getExpr() == nullptr)
        {
            return "TODO";
        }
        else
        {
            return "TODO";
        }
    }

    std::string FillPrototype::visit(Block *block)
    {

        if (block->getExprx()->getExpr() == nullptr)
            return "TODO";
        else
            return "TODO";
    }

    std::string FillPrototype::visit(Method *method) { return "TODO"; }

    std::string FillPrototype::visit(FieldMethod *fieldMethod)
    {
        if (fieldMethod->getFieldMethod() == nullptr)
            return "TODO";
        if (fieldMethod->getMethod() == nullptr)
            return "TODO";
        else
            return "TODO";
    }

    std::string FillPrototype::visit(Body *body)
    {
        return "TODO";
    }

    std::string FillPrototype::visit(Classe *classe)
    {
        if(::prototype.find(classe->getTypeID()) == ::prototype.end()){
            ::prototype[classe->getTypeID()] = ClassPrototype();
            ::prototype[classe->getTypeID()].parent.insert(classe->getParentID());
        }    
        else{
            //throw semantic exception, redefinition of a classes.
        }
        classeID = classe->getTypeID();
        classe->getBody()->accept(this);
        return "done";
       
    }

    std::string FillPrototype::visit(Classes *classes)
    {
        classes->getClass()->accept(this);
        if (classes->nextClass() != nullptr)
            classes->nextClass()->accept(this);
        return "done";
  
    }

    std::string FillPrototype::visit(Programm *programm) { 
        if( programm->getClasse() != nullptr){
            programm->getClasse()->accept(this);
        }
        else{
            programm->getClasses()->accept(this);
        }
        return"done";
    }

    std::string FillPrototype::visit(Dual *dual) { return "TODO"; }

    std::string FillPrototype::visit(Unary *unary) { return "TODO"; }

    std::string FillPrototype::visit(If *anIf)
    {
        if (anIf->getElse() == nullptr)
        {
            return "TODO";
        }
        return "TODO";
    }

    std::string FillPrototype::visit(While *aWhile) { return "TODO"; }

    std::string FillPrototype::visit(Let *let)
    {
        if (let->getAssign() == nullptr)
        {
            return "TODO";
        }
        return "TODO";
    }

    std::string FillPrototype::visit(Assign *assign) { return "TODO"; }

    std::string FillPrototype::visit(Not *aNot) { return "TODO"; }

    std::string FillPrototype::visit(And *anAnd) { return "TODO"; }

    std::string FillPrototype::visit(Equal *equal) { return "TODO"; }

    std::string FillPrototype::visit(Lower *lower) { return "TODO"; }

    std::string FillPrototype::visit(LowerEqual *lowerEqual) { return "TODO"; }

    std::string FillPrototype::visit(Plus *plus) { return "TODO"; }

    std::string FillPrototype::visit(Minus *minus) { return "TODO"; }

    std::string FillPrototype::visit(Times *times) { return "TODO"; }

    std::string FillPrototype::visit(Div *div) { return "TODO"; }

    std::string FillPrototype::visit(Pow *pow) { return "TODO"; }

    std::string FillPrototype::visit(Minus1 *minus1) { return "TODO"; }

    std::string FillPrototype::visit(IsNull *isNull) { return "TODO"; }

    std::string FillPrototype::visit(Args *args)
    {
        if (args->getExpr() != nullptr)
        {
            return "TODO";
        }
        else
        {
            return "TODO";
        }
    }

    std::string FillPrototype::visit(Function *function) { return "TODO"; }

    std::string FillPrototype::visit(Dot *dot) { return "TODO"; }

    std::string FillPrototype::visit(New *aNew) { return "TODO"; }

    std::string FillPrototype::visit(ObjID *objID) { return "TODO"; }

    std::string FillPrototype::visit(Literal *literal) { return "TODO"; }

    std::string FillPrototype::visit(IntLit *intLit) { return "TODO"; }

    std::string FillPrototype::visit(StrLit *strLit) { return "TODO"; }

    std::string FillPrototype::visit(BoolLit *boolLit) { return "TODO"; }

    std::string FillPrototype::visit(Lpar *lpar) { return "TODO"; }

    std::string FillPrototype::visit(Rpar *rpar) { return "TODO"; }

    std::string FillPrototype::visit(Parenthese *parenthese) { return "TODO"; }
