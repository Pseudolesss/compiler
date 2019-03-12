//AST node class header.
#ifndef ASTNODE_H
#define ASTNODE_H

#include <list>
#include <string>
#include "Visitor.hh"

using namespace std;

//faut pas oublier d'inclure visitor.cpp à cause du template !

struct ASTnode
{
  public:
    ASTnode();
    ASTnode(bool);
    virtual std::string accept(Visitor*);
    bool isEmpty();

  private:
    bool isempty;
};

struct Expr : ASTnode
{
  public:
    string getDataType();
    std::string accept(Visitor*);
  protected:
    string dataType;
};

struct Type : ASTnode
{
  public:
    Type();
    Type(string);
    string getID();
    std::string accept(Visitor*);

  private:
    string typeID;
};

struct Field : ASTnode
{
  public:
    Field();
    Field(string, Type*);
    Field(string, Type*, Expr*);
    string getID();
    Type* getType();
    Expr* getExpr();
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

  private:
    FieldMethod* fieldMethod;
};

struct Classe : ASTnode
{
  public:
    Classe(string, Body*);
    Classe(string, string, Body*);
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

  private:
    Expr* left;
    Expr* right;
};

struct Unary : Expr
{
  public:
    Unary(Expr*);
    Expr* getExpr();
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

  private:
    string ObjID;
    Expr* expr;
};

struct Not : Unary
{ 
  public:
  Not(Expr*);
  std::string accept(Visitor*);
};

struct And : Dual
{
  public:
    And(Expr*,Expr*);
    std::string accept(Visitor*);
};

struct Equal : Dual
{
  public:
  Equal(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Lower : Dual
{
  public:
  Lower(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct LowerEqual : Dual
{
  public:
  LowerEqual(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Plus : Dual
{
  public:
  Plus(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Minus : Dual
{
  public:
  Minus(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Times : Dual
{
  public:
  Times(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Div : Dual
{
  public:
  Div(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Pow : Dual
{
  public:
  Pow(Expr*,Expr*);
  std::string accept(Visitor*);
};

struct Minus1 : Unary
{
  public:
  Minus1(Expr*);
  std::string accept(Visitor*);
};

struct IsNull : Unary
{
  public:
  IsNull(Expr*);
  std::string accept(Visitor*);
};
struct Exprxx;
struct Exprxx
{
    public:
    Exprxx();
    Exprxx(Expr*,Exprxx*);
    Expr* getExpr();
    Exprxx* getExprxx();
    std::string accept(Visitor*);
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
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

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
    string getID();
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

  private:
    string typeID;
};

struct ObjID : Expr
{
  public:
    ObjID(string);
    string getID();
    std::string accept(Visitor*);

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
    std::string accept(Visitor*);

  private:
    int value;
};

struct StrLit : Literal
{
  public:
    StrLit(string);
    string getValue();
    std::string accept(Visitor*);

  private:
    string value;
};

struct BoolLit : Literal
{
  public:
    BoolLit(bool);
    bool getValue();
    std::string accept(Visitor*);

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
  std::string accept(Visitor*);
};

#endif