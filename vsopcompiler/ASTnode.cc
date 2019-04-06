#include "ASTnode.hh"

using namespace std;
ASTnode::ASTnode(yy::location l) : location(l)
{
    type = "";
    valueInh = "";
    valueSyn = "";
}
yy::location ASTnode::getLocation(){return location;}
std::string ASTnode::getValueInh() { return valueInh; }
std::string ASTnode::getValueSyn() { return valueSyn; }
std::string ASTnode::accept(Visitor *v) { return v->visit(this); }
void ASTnode::setType(std::string t) { type = t; }
void ASTnode::setValueInh(std::string v) { valueInh = v; }
void ASTnode::setValueSyn(std::string v) { valueSyn = v; }

Expr::Expr(yy::location l) : ASTnode(l){}
void Expr::setType(std::string type){dataType = type;}
string Expr::getDataType() { return dataType; }
std::string Expr::accept(Visitor *v) { return v->visit(this); }

Type::Type(string s,yy::location l) : ASTnode(l),typeID(s){};
string Type::getID() { return this->typeID; }
std::string Type::accept(Visitor *v) { return v->visit(this); }

Field::Field(string object_id, Type *a_type,yy::location l) : ASTnode(l),objID(object_id), type(a_type) { expr = nullptr; }
Field::Field(string object_id, Type *a_type, Expr *an_expr,yy::location l) : ASTnode(l),objID(object_id), type(a_type), expr(an_expr){};
string Field::getID() { return objID; }
Type *Field::getType() { return type; }
Expr *Field::getExpr() { return expr; }
std::string Field::accept(Visitor *v) { return v->visit(this); }

Formal::Formal(string s, Type *t,yy::location l) : ASTnode(l),objID(s), type(t){};
string Formal::getID() { return objID; }
Type *Formal::getType() { return type; }
std::string Formal::accept(Visitor *v) { return v->visit(this); }

Formalx::Formalx(yy::location l) : ASTnode(l)
{
    formal = nullptr;
    formalx = nullptr;
}
Formalx::Formalx(Formal *f, Formalx *fx,yy::location l) : ASTnode(l),formal(f), formalx(fx){};
Formal *Formalx::getFormal() { return formal; }
Formalx *Formalx::getFormalx() { return formalx; }
std::string Formalx::accept(Visitor *v) { return v->visit(this); }

Formals::Formals(yy::location l) : ASTnode(l)
{
    formal = nullptr;
    formalx = nullptr;
}
Formals::Formals(Formal *f, Formalx *fx,yy::location l) : ASTnode(l),formal(f), formalx(fx){};
Formal *Formals::getFormal() { return formal; }
Formalx *Formals::getFormalx() { return formalx; }
std::string Formals::accept(Visitor *v) { return v->visit(this); }

Exprx::Exprx(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprx = nullptr;
}
Exprx::Exprx(Expr *e, Exprx *ex,yy::location l) : ASTnode(l),expr(e), exprx(ex){};
Exprx *Exprx::getExprx() { return exprx; }
Expr *Exprx::getExpr() { return expr; }
std::string Exprx::accept(Visitor *v) { return v->visit(this); }

Block::Block(Expr *e, Exprx *ex,yy::location l) : Expr(l),expr(e), exprx(ex) { dataType = string("Block"); };
Expr *Block::getExpr() { return expr; }
Exprx *Block::getExprx() { return exprx; }
std::string Block::accept(Visitor *v) { return v->visit(this); }

Method::Method(string s, Formals *fs, Type *t, Block *b,yy::location l) : ASTnode(l), objID(s), formals(fs), type(t), block(b){};
string Method::getID() { return objID; }
Formals *Method::getFormals() { return formals; }
Type *Method::getType() { return type; }
Block *Method::getBlock() { return block; }
std::string Method::accept(Visitor *v) { return v->visit(this); }

FieldMethod::FieldMethod(yy::location l) : ASTnode(l)
{
    field = nullptr;
    method = nullptr;
    fieldMethod = nullptr;
}
FieldMethod::FieldMethod(FieldMethod *fm, Field *f,yy::location l) : ASTnode(l),fieldMethod(fm), field(f) { method = nullptr; }
FieldMethod::FieldMethod(FieldMethod *fm, Method *m,yy::location l) : ASTnode(l),fieldMethod(fm), method(m) { field = nullptr; }
Field *FieldMethod::getField() { return field; }
Method *FieldMethod::getMethod() { return method; }
FieldMethod *FieldMethod::getFieldMethod() { return fieldMethod; }
std::string FieldMethod::accept(Visitor *v) { return v->visit(this); }

Body::Body(FieldMethod *fm,yy::location l) : ASTnode(l),fieldMethod(fm) {}
FieldMethod *Body::getFieldMethod() { return fieldMethod; }
std::string Body::accept(Visitor *v) { return v->visit(this); }

Classe::Classe(string s, Body *b,yy::location l) : ASTnode(l),typeID(s), body(b) {}
Classe::Classe(string typeID, string parent, Body *b,yy::location l) : ASTnode(l),typeID(typeID), parentID(parent), body(b) {}
std::string Classe::getTypeID() { return typeID; }
std::string Classe::getParentID() { return parentID; }
Body *Classe::getBody() { return body; }
std::string Classe::accept(Visitor *v) { return v->visit(this); }

Classes::Classes(Classes *cs, Classe *c,yy::location l) : ASTnode(l),next_class(cs), a_class(c) {}
Classes::Classes(Classe *c,yy::location l) : a_class(c),ASTnode(l) { next_class = nullptr; }
Classe *Classes::getClass() { return a_class; }
Classes *Classes::nextClass() { return next_class; }
std::string Classes::accept(Visitor *v) { return v->visit(this); }

Programm::Programm(Classes *cs, yy::location l) : ASTnode(l),classes(cs) { classe = nullptr; }
Programm::Programm(Classe *c, yy::location l) : ASTnode(l), classe(c) { classes = nullptr; }
Classes *Programm::getClasses() { return classes; }
Classe *Programm::getClasse() { return classe; }
std::string Programm::accept(Visitor *v) { return v->visit(this); }

Dual::Dual(Expr *left, Expr *right,yy::location l) : Expr(l),left(left), right(right) { dataType = string("Dual"); }
Expr *Dual::getLeft() { return left; }
Expr *Dual::getRight() { return right; }
std::string Dual::accept(Visitor *v) { return v->visit(this); }

Unary::Unary(Expr *e,yy::location l) : Expr(l),expr(e) { dataType = string("Unary"); }
Expr *Unary::getExpr() { return expr; }
std::string Unary::accept(Visitor *v) { return v->visit(this); }

If::If(Expr *e1, Expr *e2, Expr *e3,yy::location l) :Expr(l), _if(e1), _then(e2), _else(e3) { dataType = string("If"); }
If::If(Expr *e1, Expr *e2,yy::location l) : Expr(l), _if(e1), _then(e2)
{
    _else = nullptr;
    dataType = string("If");
}
Expr *If::getElse() { return _else; }
Expr *If::getIf() { return _if; }
Expr *If::getThen() { return _then; }
std::string If::accept(Visitor *v) { return v->visit(this); }

While::While(Expr *w, Expr *d,yy::location l) : Expr(l), _while(w), _do(d) { dataType = string("While"); }
Expr *While::getWhile() { return _while; }
Expr *While::getDo() { return _do; }
std::string While::accept(Visitor *v) { return v->visit(this); }

Let::Let(string s, Type *t, Expr *e,yy::location l) : Expr(l), ObjID(s), type(t), in(e)
{
    assign = nullptr;
    dataType = string("Let");
}
Let::Let(string s, Type *t, Expr *ass, Expr *in,yy::location l) : Expr(l),ObjID(s), type(t), assign(ass), in(in) { dataType = string("Let"); }
Type *Let::getType() { return type; }
string Let::getObjID() { return ObjID; }
Expr *Let::getAssign() { return assign; }
Expr *Let::getIn() { return in; }
std::string Let::accept(Visitor *v) { return v->visit(this); }

Assign::Assign(string s, Expr *e,yy::location l) : Expr(l), ObjID(s), expr(e) { dataType = string("Assign"); }
Expr *Assign::getExpr() { return expr; }
string Assign::getObjID() { return ObjID; }
std::string Assign::accept(Visitor *v) { return v->visit(this); }

Exprxx::Exprxx(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprxx = nullptr;
}
Exprxx::Exprxx(Expr *e, Exprxx *ex,yy::location l) : ASTnode(l),expr(e), exprxx(ex) {}
Exprxx *Exprxx::getExprxx() { return exprxx; }
Expr *Exprxx::getExpr() { return expr; }
std::string Exprxx::accept(Visitor *v) { return v->visit(this); }

Args::Args(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprxx = nullptr;
}
Args::Args(Expr *e, Exprxx *ex,yy::location l) : ASTnode(l), expr(e), exprxx(ex) {}
Expr *Args::getExpr() { return expr; }
Exprxx *Args::getExprxx() { return exprxx; }
std::string Args::accept(Visitor *v) { return v->visit(this); }

Function::Function(string name, Args *a,yy::location l) : Expr(l),args(a), ID(name) { dataType = string("Function"); }
std::string Function::getID() { return ID; }
Args *Function::getArgs() { return args; }
std::string Function::accept(Visitor *v) { return v->visit(this); }

Dot::Dot(Expr *e, string s, Args *a,yy::location l) : Expr(l),expr(e), ID(s), args(a) { dataType = string("Dot"); }
Expr *Dot::getExpr() { return expr; }
Args *Dot::getArgs() { return args; }
std::string Dot::getID() { return ID; }
std::string Dot::accept(Visitor *v) { return v->visit(this); }

New::New(string s,yy::location l) : Expr(l),typeID(s) { dataType = s; }
string New::getTypeID() { return typeID; }
std::string New::accept(Visitor *v) { return v->visit(this); }

ObjID::ObjID(string s,yy::location l) : Expr(l),ObjId(s) { dataType = string("Object"); }
string ObjID::getID() { return ObjId; }
std::string ObjID::accept(Visitor *v) { return v->visit(this); }

Literal::Literal(yy::location l) : Expr(l) {}

IntLit::IntLit(int i,yy::location l) : Literal(l),value(i) {}
int IntLit::getValue() { return value; }
std::string IntLit::accept(Visitor *v) { return v->visit(this); }

StrLit::StrLit(string s,yy::location l) : Literal(l),value(s) {}
string StrLit::getValue() { return value; }
std::string StrLit::accept(Visitor *v) { return v->visit(this); }

BoolLit::BoolLit(bool v,yy::location l) : Literal(l),value(v) {}
bool BoolLit::getValue() { return value; }
std::string BoolLit::accept(Visitor *v) { return v->visit(this); }

And::And(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string And::accept(Visitor *v) { return v->visit(this); }

Not::Not(Expr *e,yy::location l) : Unary(e,l){};
std::string Not::accept(Visitor *v) { return v->visit(this); }

Equal::Equal(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Equal::accept(Visitor *v) { return v->visit(this); }

Lower::Lower(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Lower::accept(Visitor *v) { return v->visit(this); }

LowerEqual::LowerEqual(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string LowerEqual::accept(Visitor *v) { return v->visit(this); }

Plus::Plus(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Plus::accept(Visitor *v) { return v->visit(this); }

Minus::Minus(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Minus::accept(Visitor *v) { return v->visit(this); }

Times::Times(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Times::accept(Visitor *v) { return v->visit(this); }

Div::Div(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Div::accept(Visitor *v) { return v->visit(this); }

Pow::Pow(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l ){};
std::string Pow::accept(Visitor *v) { return v->visit(this); }

Minus1::Minus1(Expr *e,yy::location l) : Unary(e,l ){};
std::string Minus1::accept(Visitor *v) { return v->visit(this); }

IsNull::IsNull(Expr *e,yy::location l) : Unary(e,l ){};
std::string IsNull::accept(Visitor *v) { return v->visit(this); }

Parenthese::Parenthese(Expr *e,yy::location l) : Unary(e,l ){};
std::string Parenthese::accept(Visitor *v) { return v->visit(this); }
