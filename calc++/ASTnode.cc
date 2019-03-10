#include "ASTnode.hh"

using namespace std;

ASTnode::ASTnode(){isempty = false;}
ASTnode::ASTnode(bool b) : isempty(b){};
bool ASTnode::isEmpty(){return isempty;}
template <class T>
T ASTnode::accept(struct Visitor<T> v) { return v.visit(this); }


Expr::Expr() : ASTnode(true){}


Type::Type(): ASTnode(true){};
Type::Type(string s) : typeID(s){};
string Type::getID() { return this->typeID; }


Field::Field() : ASTnode(true){}
Field::Field(string object_id, struct Type a_type) : objID(object_id), type(a_type) { expr = Expr(); }
Field::Field(string object_id, struct Type a_type, Expr an_expr) : objID(object_id), type(a_type){};
string Field::getID() { return objID; }
Type Field::getType() { return type; }
Expr Field::getExpr() { return expr; }


Formal::Formal() : ASTnode(true){}
Formal::Formal(string s, Type t) : objID(s), type(t){};
string Formal::getID() { return objID; }
Type Formal::getType() { return type; }


Formalx::Formalx() : ASTnode(true){}
Formalx::Formalx(Formal f, Formalx fx) : formal(f), formalx(&fx){};
Formal Formalx::getFormal(){return formal;}
Formalx Formalx::getFormalx(){return *formalx;}


Formals::Formals() : ASTnode(true){}
Formals::Formals(Formal f, Formalx fx) : formal(f), formalx(fx){};
Formal Formals::getFormal(){return formal;}
Formalx Formals::getFormalx(){return formalx;}


Exprx::Exprx() : ASTnode(true){}
Exprx::Exprx(Expr e, Exprx ex) : expr(e), exprx(&ex){};
Exprx Exprx::getExprx(){return *exprx;}
Expr Exprx::getExpr(){return expr;}


Block::Block() : Expr(){}
Block::Block(Expr e, Exprx ex) : expr(e), exprx(ex){};
Expr Block::getExpr() {return expr;}
Exprx Block::getExprx() {return exprx;}


Method::Method() : ASTnode(true){}
Method::Method(string s, Formals fs, Type t, Block b) : objID(s), formals(fs), type(t),block(b){};
string Method::getID(){return objID;}
Formals Method::getFormals(){return formals;}
Type Method::getType(){return type;}
Block Method::getBlock(){return block;}


FieldMethod::FieldMethod() : ASTnode(true){}
FieldMethod::FieldMethod(FieldMethod fm, Field f) : fieldMethod(&fm) ,field(f){}
FieldMethod::FieldMethod(FieldMethod fm, Method m) : fieldMethod(&fm), method(m){}
Field FieldMethod::getField(){return field;}
Method FieldMethod::getMethod(){return method;}
FieldMethod FieldMethod::getFieldMethod(){return *fieldMethod;}


Body::Body() : ASTnode(true) {}
Body::Body(FieldMethod fm) : fieldMethod(fm){}
FieldMethod Body::getFieldMethod(){return fieldMethod;}


Classe::Classe(string s, Body b) : typeID(s), body(b){}
Classe::Classe(string parent, string child,Body b) :  typeID(parent), childID(child), body(b){}
Classe::Classe() : ASTnode(true){}


Classes::Classes() : ASTnode(true){}
Classes::Classes(Classes cs, Classe c) : next_class(&cs), a_class(c){}
Classes::Classes(Classe c) : a_class(c){ next_class = nullptr;}
Classe Classes::getClass(){return a_class;}
Classes* Classes::nextClass(){return next_class;}


Programm::Programm(Classes cs) : classes(cs){}
Programm::Programm(Classe c) : classe(c){}
Programm::getClasses(){return classes;}
Programm::getClasse(){return getClasse;}






