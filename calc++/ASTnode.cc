#include "ASTnode.hh"

using namespace std;

Expr::Expr(){
    this->isempty = false;
}
Type::Type(string s) : typeID(s){};
template <class T>
T Type::accept(struct Visitor<T> v)
{
    return v.viType(this);
};
string Type::getID()
{
    return this->typeID;
};


Field::Field(string object_id, struct Type a_type) : objID(object_id), type(a_type){
    expr = Expr();
};
Field::Field(string object_id, struct Type a_type, Expr an_expr) : objID(object_id), type(a_type){};
template <class T>
T Field::accept(struct Visitor<T> v)
{
    return v.viField(this);
};
string Field::getID()
{
    return this->objID;
};
Type Field::getType()
{
    return this->type;
};
Expr Field::getExpr()
{
    return this->expr;
};
