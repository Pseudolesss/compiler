#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "prototype.hh"
#include "FillPrototype.hh"
#include "ErrorHandler.hh"

    std::string locToStr(yy::location l){
        return std::to_string(l.begin.line) + "." 
                + std::to_string(l.begin.column);
    }

    std::string FillPrototype::visit(Field *field)
    {
        if(::prototype[classID].field.find(field->getID()) == ::prototype[classID].field.end()){
            FieldPrototype f = FieldPrototype();
            f.type = field->getType()->getID();
            f.location = field->getLocation();
            ::prototype[classID].field[field->getID()] = f;            
        }
        else{
            //semantic error: redefinition of a a field.
            yy::location l = ::prototype[classID].field[field->getID()].location;
            ::errors.add(field->getLocation(),
                "redefinition of field " + field->getID() 
                + "already defined at: " + locToStr(l));
        }
        return "done";
    }

    std::string FillPrototype::visit(Method *method) { 
        if(::prototype[classID].method.find(method->getID()) == ::prototype[classID].method.end()){
            MethodPrototype m = MethodPrototype();
            m.return_type = method->getType()->getID();
            m.location = method->getLocation();            
            ::prototype[classID].method[method->getID()] = m;
            methodID = method->getID();
            method->getFormals()->accept(this);
        }
        else{
            //redefinition of a method.
            yy::location l = ::prototype[classID].method[method->getID()].location;
            ::errors.add(method->getLocation(),
                "redefinition of method " + method->getID() 
                + " already defined at: " + locToStr(l));
        }
        return "done"; 
    }

    std::string FillPrototype::visit(FieldMethod *fieldMethod)
    {
        if (fieldMethod->getField() != nullptr)
           fieldMethod->getField()->accept(this);
        if (fieldMethod->getMethod() != nullptr)
            fieldMethod->getMethod()->accept(this);
        if (fieldMethod->getFieldMethod() != nullptr)
            fieldMethod->getFieldMethod()->accept(this);
        return "done";
    }

    std::string FillPrototype::visit(Body *body)
    {
        body->getFieldMethod()->accept(this);
        return "done";
    }

    std::string FillPrototype::visit(Classe *classe)
    {
        if(::prototype.find(classe->getTypeID()) == ::prototype.end()){
            ::prototype[classe->getTypeID()] = ClassPrototype();
            ::prototype[classe->getTypeID()].parent.insert(classe->getParentID());
            ::prototype[classe->getTypeID()].direct_parent = classe->getParentID();
        }    
        else{
            //redefinition of a classe
            yy::location l = ::prototype[classe->getTypeID()].location;
            errors.add(classe->getLocation(), 
                "redefinition of classe " + classe->getTypeID()
                 + " already define at " + locToStr(l));
        }
        classID = classe->getTypeID();
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
       ::prototype["Object"] = ClassPrototype();
        if( programm->getClasse() != nullptr)
            programm->getClasse()->accept(this);
        if( programm->getClasses() != nullptr)
            programm->getClasses()->accept(this);
        return"done";
    }
    std::string FillPrototype::visit(Formals *formals){
        if(formals->getFormal() != nullptr){
            formals->getFormal()->accept(this);
            formals->getFormalx()->accept(this);            
        }
        return "done";
    }

    std::string FillPrototype::visit(Formal *formal) {
        ::prototype[classID].method[methodID].arguments.push_back(formal->getType()->getID());
        return "done";
    }

    std::string FillPrototype::visit(Formalx *formalx){
        if(formalx->getFormal() != nullptr){
            formalx->getFormal()->accept(this);
            formalx->getFormalx()->accept(this);             
        } 
        return "done";    
    }

    std::string FillPrototype::visit(ASTnode *asTnode) { std::cerr<< "fillprototype in astnode"; return "should never happend";}

    std::string FillPrototype::visit(Expr *expr) {std::cerr<< "fillprototype in expr"; return "should never happend"; }

    std::string FillPrototype::visit(Type *type) { std::cerr<< "fillprototype in type"; return "should never happend"; }

    std::string FillPrototype::visit(Exprx *exprx){std::cerr<<"fillprototype in exprx" ; return "should never happend";}

    std::string FillPrototype::visit(Exprxx *exprxx){std::cerr<<"fillprototype in exprxx" ; return "should never happend";}

    std::string FillPrototype::visit(Block *block){std::cerr<<"fillprototype in block" ; return "should never happend";}

    std::string FillPrototype::visit(Dual *dual) {std::cerr<<"fillprototype in dual" ; return "should never happend";  }

    std::string FillPrototype::visit(Unary *unary) {std::cerr<<"fillprototype in iunary" ; return "should never happend"; }

    std::string FillPrototype::visit(If *anIf){std::cerr<<"fillprototype in if" ; return "should never happend";}

    std::string FillPrototype::visit(While *aWhile) { std::cerr << "fillprototype in while" ; return "should never happend";}

    std::string FillPrototype::visit(Let *let) {std::cerr<< "fillprototype in let" ; return "should never happend";}
    
    std::string FillPrototype::visit(Assign *assign) { std::cerr<< "fillprototype in assign" ; return "should never happend"; }

    std::string FillPrototype::visit(Not *aNot) { std::cerr<< "fillprototype in not" ; return "should never happend"; }

    std::string FillPrototype::visit(And *anAnd) { std::cerr<< "fillprototype in and" ; return "should never happend"; }

    std::string FillPrototype::visit(Equal *equal) {std::cerr<< "fillprototype in equal" ; return "should never happend"; }

    std::string FillPrototype::visit(Lower *lower) { std::cerr<< "fillprototype in lower" ; return "should never happend"; }

    std::string FillPrototype::visit(LowerEqual *lowerEqual) { std::cerr<< "fillprototype in lower equal" ; return "should never happend"; }

    std::string FillPrototype::visit(Plus *plus) { std::cerr<< "fillprototype in plus" ; return "should never happend"; }

    std::string FillPrototype::visit(Minus *minus) {std::cerr<< "fillprototype in minus" ; return "should never happend";}

    std::string FillPrototype::visit(Times *times) { std::cerr<< "fillprototype in time" ; return "should never happend"; }

    std::string FillPrototype::visit(Div *div) { std::cerr<< "fillprototype in div" ; return "should never happend"; }

    std::string FillPrototype::visit(Pow *pow) {  std::cerr<< "fillprototype in pow" ; return "should never happend"; }

    std::string FillPrototype::visit(Minus1 *minus1) {  std::cerr<< "fillprototype in minus1" ; return "should never happend"; }

    std::string FillPrototype::visit(IsNull *isNull) {  std::cerr<< "fillprototype in is null" ; return "should never happend"; }

    std::string FillPrototype::visit(Args *args){std::cerr<< "fillprototype in args" ; return "should never happend";}

    std::string FillPrototype::visit(Function *function) { std::cerr<< "fillprototype in function" ; return "should never happend"; }

    std::string FillPrototype::visit(Dot *dot) { std::cerr<< "fillprototype in dot" ; return "should never happend"; }

    std::string FillPrototype::visit(New *aNew) { std::cerr<< " fillprototype in new" ; return "should never happend"; }

    std::string FillPrototype::visit(ObjID *objID) { std::cerr<< " fillprototype in objid" ; return "should never happend"; }

    std::string FillPrototype::visit(Literal *literal) { std::cerr<< " fillprototype in literal" ; return "should never happend"; }

    std::string FillPrototype::visit(IntLit *intLit) { std::cerr<< " fillprototype in int literal" ; return "should never happend"; }

    std::string FillPrototype::visit(StrLit *strLit) { std::cerr<< " fillprototype in str literal" ; return "should never happend"; }

    std::string FillPrototype::visit(BoolLit *boolLit) { std::cerr<< " fillprototype in bool literal" ; return "should never happend"; }

    std::string FillPrototype::visit(Lpar *lpar) { std::cerr<< " fillprototype in (" ; return "should never happend"; }

    std::string FillPrototype::visit(Rpar *rpar) { std::cerr<< " fillprototype in )" ; return "should never happend"; }

    std::string FillPrototype::visit(Parenthese *parenthese) { std::cerr<< " fillprototype in ()" ; return "should never happend"; }
