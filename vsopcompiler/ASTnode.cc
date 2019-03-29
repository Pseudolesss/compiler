#include "ASTnode.hh"

using namespace std;
ASTnode::ASTnode(){}
ASTnode::ASTnode(yy::location l) : first_line(l.begin.line),last_line(l.end.line), first_column(l.begin.column), last_column(l.end.column)
{
    type = "";
    valueInh = "";
    valueSyn = "";
    std::cout<<first_line<<":"<<first_column<< ","<<last_line<<":"<<last_column<<std::endl;
}
std::string ASTnode::getType() { return type; }
std::string ASTnode::getValueInh() { return valueInh; }
std::string ASTnode::getValueSyn() { return valueSyn; }
void ASTnode::setType(std::string t) { type = t; }
void ASTnode::setValueInh(std::string v) { valueInh = v; }
void ASTnode::setValueSyn(std::string v) { valueSyn = v; }
std::string ASTnode::accept(Visitor *v) { return v->visit(this); }

string Expr::getDataType() { return dataType; }
std::string Expr::accept(Visitor *v) { return v->visit(this); }

Type::Type(string s) : typeID(s){};
string Type::getID() { return this->typeID; }
std::string Type::accept(Visitor *v) { return v->visit(this); }

Field::Field(string object_id, Type *a_type) : objID(object_id), type(a_type) { expr = nullptr; }
Field::Field(string object_id, Type *a_type, Expr *an_expr) : objID(object_id), type(a_type), expr(an_expr){};
string Field::getID() { return objID; }
Type *Field::getType() { return type; }
Expr *Field::getExpr() { return expr; }
std::string Field::accept(Visitor *v) { return v->visit(this); }

Formal::Formal(string s, Type *t) : objID(s), type(t){};
string Formal::getID() { return objID; }
Type *Formal::getType() { return type; }
std::string Formal::accept(Visitor *v) { return v->visit(this); }

Formalx::Formalx()
{
    formal = nullptr;
    formalx = nullptr;
}
Formalx::Formalx(Formal *f, Formalx *fx) : formal(f), formalx(fx){};
Formal *Formalx::getFormal() { return formal; }
Formalx *Formalx::getFormalx() { return formalx; }
std::string Formalx::accept(Visitor *v) { return v->visit(this); }

Formals::Formals()
{
    formal = nullptr;
    formalx = nullptr;
}
Formals::Formals(Formal *f, Formalx *fx) : formal(f), formalx(fx){};
Formal *Formals::getFormal() { return formal; }
Formalx *Formals::getFormalx() { return formalx; }
std::string Formals::accept(Visitor *v) { return v->visit(this); }

Exprx::Exprx()
{
    expr = nullptr;
    exprx = nullptr;
}
Exprx::Exprx(Expr *e, Exprx *ex) : expr(e), exprx(ex){};
Exprx *Exprx::getExprx() { return exprx; }
Expr *Exprx::getExpr() { return expr; }
std::string Exprx::accept(Visitor *v) { return v->visit(this); }

Block::Block(Expr *e, Exprx *ex) : expr(e), exprx(ex) { dataType = string("Block"); };
Expr *Block::getExpr() { return expr; }
Exprx *Block::getExprx() { return exprx; }
std::string Block::accept(Visitor *v) { return v->visit(this); }

Method::Method(string s, Formals *fs, Type *t, Block *b) : objID(s), formals(fs), type(t), block(b){};
string Method::getID() { return objID; }
Formals *Method::getFormals() { return formals; }
Type *Method::getType() { return type; }
Block *Method::getBlock() { return block; }
std::string Method::accept(Visitor *v) { return v->visit(this); }

FieldMethod::FieldMethod()
{
    field = nullptr;
    method = nullptr;
    fieldMethod = nullptr;
}
FieldMethod::FieldMethod(FieldMethod *fm, Field *f) : fieldMethod(fm), field(f) { method = nullptr; }
FieldMethod::FieldMethod(FieldMethod *fm, Method *m) : fieldMethod(fm), method(m) { field = nullptr; }
Field *FieldMethod::getField() { return field; }
Method *FieldMethod::getMethod() { return method; }
FieldMethod *FieldMethod::getFieldMethod() { return fieldMethod; }
std::string FieldMethod::accept(Visitor *v) { return v->visit(this); }

Body::Body(FieldMethod *fm) : fieldMethod(fm) {}
FieldMethod *Body::getFieldMethod() { return fieldMethod; }
std::string Body::accept(Visitor *v) { return v->visit(this); }

Classe::Classe(string s, Body *b) : typeID(s), body(b) {}
Classe::Classe(string typeID, string parent, Body *b) : typeID(typeID), parentID(parent), body(b) {}
std::string Classe::getTypeID() { return typeID; }
std::string Classe::getParentID() { return parentID; }
Body *Classe::getBody() { return body; }
std::string Classe::accept(Visitor *v) { return v->visit(this); }

Classes::Classes(Classes *cs, Classe *c) : next_class(cs), a_class(c) {}
Classes::Classes(Classe *c) : a_class(c) { next_class = nullptr; }
Classe *Classes::getClass() { return a_class; }
Classes *Classes::nextClass() { return next_class; }
std::string Classes::accept(Visitor *v) { return v->visit(this); }

Programm::Programm(Classes *cs, yy::location l) : ASTnode(l),classes(cs) { std::cout<<"in programm"<<std::endl;classe = nullptr; }
Programm::Programm(Classe *c, yy::location l) : ASTnode(l), classe(c) { std::cout<<"in programm"<<std::endl;classes = nullptr; }
Classes *Programm::getClasses() { return classes; }
Classe *Programm::getClasse() { return classe; }
std::string Programm::accept(Visitor *v) { return v->visit(this); }

Dual::Dual(Expr *left, Expr *right) : left(left), right(right) { dataType = string("Dual"); }
Expr *Dual::getLeft() { return left; }
Expr *Dual::getRight() { return right; }
std::string Dual::accept(Visitor *v) { return v->visit(this); }

Unary::Unary(Expr *e) : expr(e) { dataType = string("Unary"); }
Expr *Unary::getExpr() { return expr; }
std::string Unary::accept(Visitor *v) { return v->visit(this); }

If::If(Expr *e1, Expr *e2, Expr *e3) : _if(e1), _then(e2), _else(e3) { dataType = string("If"); }
If::If(Expr *e1, Expr *e2) : _if(e1), _then(e2)
{
    _else = nullptr;
    dataType = string("If");
}
Expr *If::getElse() { return _else; }
Expr *If::getIf() { return _if; }
Expr *If::getThen() { return _then; }
std::string If::accept(Visitor *v) { return v->visit(this); }

While::While(Expr *w, Expr *d) : _while(w), _do(d) { dataType = string("While"); }
Expr *While::getWhile() { return _while; }
Expr *While::getDo() { return _do; }
std::string While::accept(Visitor *v) { return v->visit(this); }

Let::Let(string s, Type *t, Expr *e) : ObjID(s), type(t), in(e)
{
    assign = nullptr;
    dataType = string("Let");
}
Let::Let(string s, Type *t, Expr *in, Expr *ass) : ObjID(s), type(t), assign(ass), in(in) { dataType = string("Let"); }
Type *Let::getType() { return type; }
string Let::getObjID() { return ObjID; }
Expr *Let::getAssign() { return assign; }
Expr *Let::getIn() { return in; }
std::string Let::accept(Visitor *v) { return v->visit(this); }

Assign::Assign(string s, Expr *e) : ObjID(s), expr(e) { dataType = string("Assign"); }
Expr *Assign::getExpr() { return expr; }
string Assign::getObjID() { return ObjID; }
std::string Assign::accept(Visitor *v) { return v->visit(this); }

Exprxx::Exprxx()
{
    expr = nullptr;
    exprxx = nullptr;
}
Exprxx::Exprxx(Expr *e, Exprxx *ex) : expr(e), exprxx(ex) {}
Exprxx *Exprxx::getExprxx() { return exprxx; }
Expr *Exprxx::getExpr() { return expr; }
std::string Exprxx::accept(Visitor *v) { return v->visit(this); }

Args::Args()
{
    expr = nullptr;
    exprxx = nullptr;
}
Args::Args(Expr *e, Exprxx *ex) : expr(e), exprxx(ex) {}
Expr *Args::getExpr() { return expr; }
Exprxx *Args::getExprxx() { return exprxx; }
std::string Args::accept(Visitor *v) { return v->visit(this); }

Function::Function(string name, Args *a) : args(a), ID(name) { dataType = string("Function"); }
std::string Function::getID() { return ID; }
Args *Function::getArgs() { return args; }
std::string Function::accept(Visitor *v) { return v->visit(this); }

Dot::Dot(Expr *e, string s, Args *a) : expr(e), ID(s), args(a) { dataType = string("Dot"); }
Expr *Dot::getExpr() { return expr; }
Args *Dot::getArgs() { return args; }
std::string Dot::getID() { return ID; }
std::string Dot::accept(Visitor *v) { return v->visit(this); }

New::New(string s) : typeID(s) { dataType = string("New"); }
string New::getTypeID() { return typeID; }
std::string New::accept(Visitor *v) { return v->visit(this); }

ObjID::ObjID(string s) : ObjId(s) { dataType = string("ObjID"); }
string ObjID::getID() { return ObjId; }
std::string ObjID::accept(Visitor *v) { return v->visit(this); }

IntLit::IntLit(int i) : value(i) {}
int IntLit::getValue() { return value; }
std::string IntLit::accept(Visitor *v) { return v->visit(this); }

StrLit::StrLit(string s) : value(s) {}
string StrLit::getValue() { return value; }
std::string StrLit::accept(Visitor *v) { return v->visit(this); }

BoolLit::BoolLit(bool v) : value(v) {}
bool BoolLit::getValue() { return value; }
std::string BoolLit::accept(Visitor *v) { return v->visit(this); }

And::And(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string And::accept(Visitor *v) { return v->visit(this); }

Not::Not(Expr *e) : Unary(e){};
std::string Not::accept(Visitor *v) { return v->visit(this); }

Equal::Equal(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Equal::accept(Visitor *v) { return v->visit(this); }

Lower::Lower(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Lower::accept(Visitor *v) { return v->visit(this); }

LowerEqual::LowerEqual(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string LowerEqual::accept(Visitor *v) { return v->visit(this); }

Plus::Plus(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Plus::accept(Visitor *v) { return v->visit(this); }

Minus::Minus(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Minus::accept(Visitor *v) { return v->visit(this); }

Times::Times(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Times::accept(Visitor *v) { return v->visit(this); }

Div::Div(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Div::accept(Visitor *v) { return v->visit(this); }

Pow::Pow(Expr *e1, Expr *e2) : Dual(e1, e2){};
std::string Pow::accept(Visitor *v) { return v->visit(this); }

Minus1::Minus1(Expr *e) : Unary(e){};
std::string Minus1::accept(Visitor *v) { return v->visit(this); }

IsNull::IsNull(Expr *e) : Unary(e){};
std::string IsNull::accept(Visitor *v) { return v->visit(this); }

Parenthese::Parenthese(Expr *e) : Unary(e){};
std::string Parenthese::accept(Visitor *v) { return v->visit(this); }
