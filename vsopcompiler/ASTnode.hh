//AST node class header.
#ifndef ASTNODE_H
#define ASTNODE_H

#include <list>
#include <string>
#include <vector>
#include "Visitor.hh"
#include "location.hh"

#include "prototype.hh"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace std;

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::AllocaInst *> NamedValues;
static std::map<std::string, llvm::StructType *> ClassesType;

// Because in vsop, every variable have a default initial value TODO Check if this is true for Class types (fields should be initialized by default)
// @args Variable's type, Variable's value, Variable's name.
static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Type* VarType, llvm::Value* VarValue , const std::string &VarName) {

    // TODO If needed, add Block argument to allocate memory where they are declared instead of function entry
    llvm::IRBuilder<> TmpB(Builder.GetInsertBlock(), Builder.GetInsertBlock()->begin());

    return TmpB.CreateAlloca(VarType, 0, VarValue, VarName.c_str());
}


struct ASTnode
{
  public:
    ASTnode(yy::location);
    virtual std::string accept(Visitor*);
    std::string getType();
    std::string getValueInh();
    std::string getValueSyn();
    void setType(std::string);
    void setValueInh(std::string);
    void setValueSyn(std::string);
    yy::location getLocation();
    virtual llvm::Value* codegen();

  private:
    std::string type;
    std::string valueInh;
    std::string valueSyn;
    yy::location location;
};

struct Expr : ASTnode
{
  public:
    Expr(yy::location l);
    std::string getDataType();
    std::string accept(Visitor*);
    void setType(std::string);
  protected:
    string dataType;
};

struct Type : ASTnode
{
  public:
    Type(yy::location);
    Type(string,yy::location);
    std::string getID();
    std::string accept(Visitor*);

  private:
    string typeID;
};

struct Field : ASTnode
{
  public:
    Field(string, Type*,yy::location);
    Field(string, Type*, Expr*,yy::location);
    string getID();
    Type* getType();
    Expr* getExpr();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string objID;
    Type* type;
    Expr* expr;
};

struct Formal : ASTnode
{
  public:
    Formal(string, Type*,yy::location);
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
    Formalx(yy::location);
    Formalx(Formal*, Formalx*,yy::location);
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
    Formals(yy::location);
    Formals(Formal*, Formalx*,yy::location);
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
    Exprx(yy::location);
    Exprx(Expr*, Exprx*,yy::location);
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
    Block(Expr*, Exprx*,yy::location);
    Expr* getExpr();
    Exprx* getExprx();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Expr* expr;
    Exprx* exprx;
};

struct Method : ASTnode
{
  public:
    Method(string, Formals*, Type*, Block*,yy::location);
    string getID();
    Formals* getFormals();
    Type* getType();
    Block* getBlock();
    std::string accept(Visitor*);
    llvm::Value* codegen();

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
    FieldMethod(FieldMethod*, Field*,yy::location);
    FieldMethod(FieldMethod*, Method*,yy::location);
    FieldMethod(yy::location);
    Field* getField();
    Method* getMethod();
    FieldMethod* getFieldMethod();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Field* field;
    Method* method;
    FieldMethod *fieldMethod;
};

struct Body : ASTnode
{
  public:
    Body(FieldMethod*,yy::location);
    FieldMethod* getFieldMethod();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    FieldMethod* fieldMethod;
};

struct Classe : ASTnode
{
  public:
    Classe(string, Body*,yy::location);
    Classe(string, string, Body*,yy::location);
    std::string getTypeID();
    std::string getParentID();
    Body* getBody();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string typeID;
    string parentID;
    Body* body;
};
struct Classes;
struct Classes : ASTnode
{
  public:
    Classes(Classes*, Classe*,yy::location);
    Classes(Classe*,yy::location);
    Classe* getClass();
    Classes* nextClass();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Classe* a_class;
    Classes* next_class;
};

struct Programm : ASTnode
{
  public:
    Programm(Classes*,yy::location);
    Programm(Classe*,yy::location);
    Classes* getClasses();
    Classe* getClasse();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Classes* classes;
    Classe* classe;
};

struct Dual : Expr
{
  public:
    Dual(Expr*, Expr*,yy::location);
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
    Unary(Expr*,yy::location);
    Expr* getExpr();
    std::string accept(Visitor*);

  private:
    Expr* expr;
};

struct If : Expr
{
  public:
    If(Expr*, Expr*, Expr*,yy::location);
    If(Expr*, Expr*,yy::location);
    Expr* getIf();
    Expr* getThen();
    Expr* getElse();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Expr* _if;
    Expr* _then;
    Expr* _else;
};

struct While : Expr
{
  public:
    While(Expr*, Expr*,yy::location);
    Expr* getWhile();
    Expr* getDo();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    Expr* _while;
    Expr* _do;
};

struct Let : Expr
{
  public:
    Let(string, Type*, Expr*,yy::location);
    Let(string, Type*, Expr*, Expr*,yy::location);
    std::string getObjID();
    Type* getType();
    Expr* getAssign();
    Expr* getIn();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string ObjID;
    Type* type;
    Expr* assign;
    Expr* in;
};

struct Assign : Expr
{
  public:
    Assign(string, Expr*,yy::location);
    string getObjID();
    Expr* getExpr();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string ObjID;
    Expr* expr;
};

struct Not : Unary
{ 
  public:
  Not(Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct And : Dual
{
  public:
    And(Expr*,Expr*,yy::location);
    std::string accept(Visitor*);
    llvm::Value* codegen();
};

struct Equal : Dual
{
  public:
  Equal(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct Lower : Dual
{
  public:
  Lower(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct LowerEqual : Dual
{
  public:
  LowerEqual(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();

};

struct Plus : Dual
{
  public:
  Plus(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct Minus : Dual
{
  public:
  Minus(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct Times : Dual
{
  public:
  Times(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct Div : Dual
{
  public:
  Div(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct Pow : Dual
{
  public:
  Pow(Expr*,Expr*,yy::location);
  std::string accept(Visitor*);
};

struct Minus1 : Unary
{
  public:
  Minus1(Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();
};

struct IsNull : Unary
{
  public:
  IsNull(Expr*,yy::location);
  std::string accept(Visitor*);
  llvm::Value* codegen();

};
struct Exprxx;
struct Exprxx : ASTnode
{
    public:
    Exprxx(yy::location);
    Exprxx(Expr*,Exprxx*,yy::location);
    Expr* getExpr();
    Exprxx* getExprxx();
    std::string accept(Visitor*);
    private:
    Expr* expr;
    Exprxx* exprxx;
};

struct Args : ASTnode
{
  public:
    Args(yy::location);
    Args(Expr*, Exprxx*,yy::location);
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
    Function(string, Args*,yy::location);
    string getID();
    Args* getArgs();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string ID;
    Args* args;
};

struct Dot : Expr
{
  public:
    Dot(Expr*, string, Args*,yy::location);
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
    New(string,yy::location);
    string getTypeID();
    std::string accept(Visitor*);

  private:
    string typeID;
};

struct ObjID : Expr
{
  public:
    ObjID(string,yy::location);
    std::string getID();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    std::string ObjId;
};

struct Literal : Expr
{
  public:
    Literal(yy::location);
};

struct IntLit : Literal
{
  public:
    IntLit(int,yy::location);
    int getValue();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    int value;
};

struct StrLit : Literal
{
  public:
    StrLit(string,yy::location);
    string getValue();
    std::string accept(Visitor*);
    llvm::Value* codegen();

  private:
    string value;
};

struct BoolLit : Literal
{
  public:
    BoolLit(bool,yy::location);
    bool getValue();
    std::string accept(Visitor*);
    llvm::Value* codegen();

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
  Parenthese(Expr*,yy::location);
  std::string accept(Visitor*);
};

#endif