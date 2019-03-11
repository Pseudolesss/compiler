//AST node class header.
#ifndef ASTNODE_H
#define ASTNODE_H

#include <list>
#include <string>

using namespace std;

//faut pas oublier d'inclure visitor.cpp à cause du template !
template <class T>
class Visitor;

struct ASTnode
{
  public:
    ASTnode();
    ASTnode(bool);
    template <class T> T
    accept(struct Visitor<T>);
    bool isEmpty();

  private:
    bool isempty;
};

struct Expr : ASTnode
{
  public:
    string getDataType();
  protected:
    string dataType;
};

struct Type : ASTnode
{
  public:
    Type();
    Type(string);
    template <typename T>
    T accept(struct Visitor<T>);
    string getID();

  private:
    string typeID;
};

struct Field : ASTnode
{
  public:
    Field();
    Field(string, Type*);
    Field(string, Type*, Expr*);
    template <typename T>
    T accept(struct Visitor<T>);
    string getID();
    Type* getType();
    Expr* getExpr();

  private:
    string objID;
    Type* type;
    Expr* expr;
};

struct Formal : ASTnode
{
  public:
    Formal(string, Type*);
    string getID();
    Type* getType();

  private:
    string objID;
    Type* type;
};
struct Formalx;
struct Formalx : ASTnode
{
  public:
    Formalx();
    Formalx(Formal*, Formalx*);
    Formal* getFormal();
    Formalx* getFormalx();

  private:
    Formal* formal;
    Formalx *formalx;
};

struct Formals : ASTnode
{
  public:
    Formals();
    Formals(Formal*, Formalx*);
    Formal* getFormal();
    Formalx* getFormalx();

  private:
    Formal* formal;
    Formalx* formalx;
};
struct Exprx;
struct Exprx : ASTnode
{
  public:
    Exprx();
    Exprx(Expr*, Exprx*);
    Expr* getExpr();
    Exprx* getExprx();

  private:
    Expr* expr;
    Exprx* exprx;
};

struct Block : Expr
{
  public:
    Block(Expr*, Exprx*);
    Expr* getExpr();
    Exprx* getExprx();

  private:
    Expr* expr;
    Exprx* exprx;
};

struct Method : ASTnode
{
  public:
    Method(string, Formals*, Type*, Block*);
    string getID();
    Formals* getFormals();
    Type* getType();
    Block* getBlock();

  private:
    string objID;
    Formals* formals;
    Type* type;
    Block* block;
};
struct FieldMethod;
struct FieldMethod : ASTnode
{
  public:
    FieldMethod(FieldMethod*, Field*);
    FieldMethod(FieldMethod*, Method*);
    FieldMethod();
    Field* getField();
    Method* getMethod();
    FieldMethod* getFieldMethod();

  private:
    Field* field;
    Method* method;
    FieldMethod *fieldMethod;
};

struct Body : ASTnode
{
  public:
    Body(FieldMethod*);
    FieldMethod* getFieldMethod();

  private:
    FieldMethod* fieldMethod;
};

struct Classe : ASTnode
{
  public:
    Classe(string, Body*);
    Classe(string, string, Body*);

  private:
    string typeID;
    string childID;
    Body* body;
};
struct Classes;
struct Classes : ASTnode
{
  public:
    Classes(Classes*, Classe*);
    Classes(Classe*);
    Classe* getClass();
    Classes* nextClass();

  private:
    Classe* a_class;
    Classes* next_class;
};

struct Programm : ASTnode
{
  public:
    Programm(Classes*);
    Programm(Classe*);
    Classes* getClasses();
    Classe* getClasse();

  private:
    Classes* classes;
    Classe* classe;
};

struct Dual : Expr
{
  public:
    Dual(Expr*, Expr*);
    Expr* getLeft();
    Expr* getRight();

  private:
    Expr* left;
    Expr* right;
};

struct Unary : Expr
{
  public:
    Unary(Expr*);
    Expr* getExpr();

  private:
    Expr* expr;
};

struct If : Expr
{
  public:
    If(Expr*, Expr*, Expr*);
    If(Expr*, Expr*);
    Expr* getIf();
    Expr* getThen();
    Expr* getElse();

  private:
    Expr* _if;
    Expr* _then;
    Expr* _else;
};

struct While : Expr
{
  public:
    While(Expr*, Expr*);
    Expr* getWhile();
    Expr* getDo();

  private:
    Expr* _while;
    Expr* _do;
};

struct Let : Expr
{
  public:
    Let(string, Type*, Expr*);
    Let(string, Type*, Expr*, Expr*);
    string getObjID();
    Type* getType();
    Expr* getAssign();
    Expr* getIn();

  private:
    string ObjID;
    Type* type;
    Expr* assign;
    Expr* in;
};

struct Assign : Expr
{
  public:
    Assign(string, Expr*);
    string getObjID();
    Expr* getExpr();

  private:
    string ObjID;
    Expr* expr;
};

struct Not : Unary
{ 
  public:
  Not(Expr*);
};

struct And : Dual
{
  public:
    And(Expr*,Expr*);
};

struct Equal : Dual
{
  public:
  Equal(Expr*,Expr*);
};

struct Lower : Dual
{
  public:
  Lower(Expr*,Expr*);
};

struct LowerEqual : Dual
{
  public:
  LowerEqual(Expr*,Expr*);
};

struct Plus : Dual
{
  public:
  Plus(Expr*,Expr*);
};

struct Minus : Dual
{
  public:
  Minus(Expr*,Expr*);
};

struct Times : Dual
{
  public:
  Times(Expr*,Expr*);
};

struct Div : Dual
{
  public:
  Div(Expr*,Expr*);
};

struct Pow : Dual
{
  public:
  Pow(Expr*,Expr*);
};

struct Minus1 : Unary
{
  public:
  Minus1(Expr*);
};

struct IsNull : Unary
{
  public:
  IsNull(Expr*);
};
struct Exprxx;
struct Exprxx
{
    public:
    Exprxx();
    Exprxx(Expr*,Exprxx*);
    Expr* getExpr();
    Exprxx* getExprxx();
    private:
    Expr* expr;
    Exprxx* exprxx;
};

struct Args
{
  public:
    Args();
    Args(Expr*, Exprxx*);
    Expr* getExpr();
    Exprxx* getExprxx();

  private:
    Expr* expr;
    Exprxx* exprxx;
};

struct Function : Expr
{
  public:
    Function(string, Args*);
    string getID();
    Args* getArgs();

  private:
    string ID;
    Args* args;
};

struct Dot : Expr
{
  public:
    Dot(Expr*, string, Args*);
    Expr* getExpr();
    Args* getArgs();

  private:
    Expr* expr;
    string ID;
    Args* args;
};

struct New : Expr
{
  public:
    New(string);
    string getTypeID();

  private:
    string typeID;
};

struct ObjID : Expr
{
  public:
    ObjID(string);
    string getID();

  private:
    string ObjId;
};

struct Literal : Expr
{
};

struct IntLit : Literal
{
  public:
    IntLit(int);
    int getValue();

  private:
    int value;
};

struct StrLit : Literal
{
  public:
    StrLit(string);
    string getValue();

  private:
    string value;
};

struct BoolLit : Literal
{
  public:
    BoolLit(bool);
    bool getValue();

  private:
    bool value;
};

struct Lpar : Expr
{
};

struct Rpar : Expr
{
};

struct Parenthese : Unary
{
  public:
  Parenthese(Expr*);
};

#endif