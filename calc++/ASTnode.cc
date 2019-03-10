#include "ASTnode.hh"

using namespace std;

ASTnode::ASTnode(){isempty = false;}
ASTnode::ASTnode(bool b) : isempty(b){};
bool ASTnode::isEmpty(){return isempty;}
template <class T>
T ASTnode::accept(struct Visitor<T> v) { return v.visit(this); }


Expr::Expr() : ASTnode(true){}


Type::Type(string s) : typeID(s){};
string Type::getID() { return this->typeID; }


Field::Field(string object_id, Type* a_type) : objID(object_id), type(a_type) { expr = nullptr; }
Field::Field(string object_id, Type* a_type, Expr* an_expr) : objID(object_id), type(a_type){};
string Field::getID() { return objID; }
Type* Field::getType() { return type; }
Expr* Field::getExpr() { return expr; }


Formal::Formal(string s, Type* t) : objID(s), type(t){};
string Formal::getID() { return objID; }
Type* Formal::getType() { return type; }


Formalx::Formalx() {formal = nullptr; formalx = nullptr;}
Formalx::Formalx(Formal* f, Formalx* fx) : formal(f), formalx(fx){};
Formal* Formalx::getFormal(){return formal;}
Formalx* Formalx::getFormalx(){return formalx;}


Formals::Formals(){formal = nullptr; formalx = nullptr;}
Formals::Formals(Formal* f, Formalx* fx) : formal(f), formalx(fx){};
Formal* Formals::getFormal(){return formal;}
Formalx* Formals::getFormalx(){return formalx;}


Exprx::Exprx(){expr = nullptr; exprx = nullptr;}
Exprx::Exprx(Expr* e, Exprx* ex) : expr(e), exprx(ex){};
Exprx* Exprx::getExprx(){return exprx;}
Expr* Exprx::getExpr(){return expr;}


Block::Block(Expr* e, Exprx* ex) : expr(e), exprx(ex){};
Expr* Block::getExpr() {return expr;}
Exprx* Block::getExprx() {return exprx;}


Method::Method(string s, Formals* fs, Type* t, Block* b) : objID(s), formals(fs), type(t),block(b){};
string Method::getID(){return objID;}
Formals* Method::getFormals(){return formals;}
Type* Method::getType(){return type;}
Block* Method::getBlock(){return block;}


FieldMethod::FieldMethod(){field = nullptr; method = nullptr; fieldMethod = nullptr;}
FieldMethod::FieldMethod(FieldMethod* fm, Field* f) : fieldMethod(fm) ,field(f){method = nullptr;}
FieldMethod::FieldMethod(FieldMethod* fm, Method* m) : fieldMethod(fm), method(m){field = nullptr;}
Field* FieldMethod::getField(){return field;}
Method* FieldMethod::getMethod(){return method;}
FieldMethod* FieldMethod::getFieldMethod(){return fieldMethod;}


Body::Body(FieldMethod* fm) : fieldMethod(fm){}
FieldMethod* Body::getFieldMethod(){return fieldMethod;}


Classe::Classe(string s, Body* b) : typeID(s), body(b){}
Classe::Classe(string parent, string child,Body* b) :  typeID(parent), childID(child), body(b){}


Classes::Classes(Classes cs, Classe c) : next_class(&cs), a_class(c){}
Classes::Classes(Classe c) : a_class(c){ next_class = nullptr;}
Classe Classes::getClass(){return a_class;}
Classes* Classes::nextClass(){return next_class;}


Programm::Programm(Classes* cs) : classes(cs){classe = nullptr;}
Programm::Programm(Classe* c) : classe(c){classes = nullptr;}
Classes* Programm::getClasses(){return classes;}
Classe* Programm::getClasse(){return classe;}


Dual::Dual(Expr* left, Expr* right) : left(left), right(right) {}
Expr* Dual::getLeft(){return left;};
Expr* Dual::getRight(){return right;}


Unary::Unary(Expr* e) : expr(e) {}
Expr* Unary::getExpr(){return expr;}


If::If(Expr* e1, Expr* e2, Expr* e3) : _if(e1), _then(e2),_else(e3){}
If::If(Expr* e1, Expr* e2) : _if(e1), _then(e2){_else = nullptr;}
Expr* If::getElse(){return _else;}
Expr* If::getIf(){return _if;}
Expr* If::getThen(){return _then;}


While::While(Expr* w,Expr*  d) : _while(w),_do(d){}
Expr* While::getWhile(){return _while;}
Expr* While::getDo(){return _do;}


Let::Let(string s, Type* t, Expr* e) : ObjID(s), type(t), in(e){assign = nullptr;}
Let::Let(string s, Type* t, Expr* in, Expr* ass) : ObjID(s), type(t), assign(ass), in(in) {}
Type* Let::getType(){return type;}
string Let::getObjID(){return ObjID;}
Expr* Let::getAssign(){return assign;}
Expr* Let::getIn(){return in;}


Assign::Assign(string s, Expr* e) : ObjID(s), expr(e) {}
Expr* Assign::getExpr(){return expr;}
string Assign::getObjID(){return ObjID;}


Exprxx::Exprxx(){expr = nullptr; exprxx= nullptr;}
Exprxx::Exprxx(Expr* e ,Exprxx* ex) : expr(e), exprxx(ex) {}
Exprxx* Exprxx::getExprxx(){return exprxx;}
Expr* Exprxx::getExpr(){return expr;}


Args::Args(){expr = nullptr; exprxx = nullptr;}
Args::Args(Expr* e, Exprxx* ex) : expr(e), exprxx(ex) {}
Expr* Args::getExpr(){return expr;}
Exprxx* Args::getExprxx(){return exprxx;}


Function::Function(string name,Args* a) : args(a), ID(name) {}
Args* Function::getArgs(){return args;}


Dot::Dot(Expr* e, string s, Args* a) : expr(e),ID(s),args(a) {}
Expr* Dot::getExpr(){return expr;}
Args* Dot::getArgs(){return args;}


New::New(string s) : typeID(s) {}
string New::getTypeID(){return typeID;}


ObjID::ObjID(string s) : ObjId(s) {}
string ObjID::getID() {return ObjId;}

IntLit::IntLit(int i) : value(i){}
int IntLit::getValue() {return value;}


StrLit::StrLit(string s) : value(s) {}
string StrLit::getValue(){return value;}


BoolLit::BoolLit(bool v) : value(v){}
bool BoolLit::getValue(){return value;}









































