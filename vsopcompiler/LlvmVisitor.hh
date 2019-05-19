//
// Created by Julien L'hoest on 14/05/19.
//

#ifndef LLVM_VISITOR_HH
#define LLVM_VISITOR_HH

#include <string>
#include <stack>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include "SymbolTable.hh"
#include "SymbolTableAlloc.hh"

struct ASTnode;
struct Expr;
struct Type;
struct Field;
struct Formal;
struct Formalx;
struct Formals;
struct Exprx;
struct Exprxx;
struct Block;
struct Method;
struct FieldMethod;
struct Body;
struct Classe;
struct Classes;
struct Programm;
struct Dual;
struct Unary;
struct If;
struct While;
struct Let;
struct Assign;
struct Not;
struct And;
struct Equal;
struct Lower;
struct LowerEqual;
struct Plus;
struct Minus;
struct Times;
struct Div;
struct Pow;
struct Minus1;
struct IsNull;
struct Exprxx;
struct Args;
struct Function;
struct Dot;
struct New;
struct ObjID;
struct Literal;
struct IntLit;
struct StrLit;
struct BoolLit;
struct Lpar;
struct Rpar;
struct Parenthese;

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule = llvm::make_unique<llvm::Module>("mymodule", TheContext);
static std::map<std::string, llvm::Type *> ClassesType;
static std::map<std::string, llvm::Value *> Def_field_value;

//map each class to a unique llvm value* in both direction. The value is an int.
static std::map<std::string,llvm::Value *> class_key = std::map<std::string,llvm::Value *>();
static std::map<llvm::Value *,std::string> key_class = std::map<llvm::Value *,std::string>();
int key;

static std::stack<llvm::Value*>  self_ptr;

// Because in vsop, every variable have a default initial value TODO Check if this is true for Class types (fields should be initialized by default)
// @args Variable's type, Variable's value, Variable's name.
static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Type *VarType, llvm::Value *VarValue, const std::string &VarName)
{
    // TODO If needed, add Block argument to allocate memory where they are declared instead of function entry
    llvm::IRBuilder<> TmpB(Builder.GetInsertBlock(), Builder.GetInsertBlock()->begin());
    return TmpB.CreateAlloca(VarType, 0, VarValue, VarName.c_str());
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, llvm::Type *VarType, const std::string &VarName)
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                     TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(VarType, 0,
                             VarName.c_str());
}

class LlvmVisitor
{
public:
    virtual llvm::Value *visit(ASTnode *) = 0;
    virtual llvm::Value *visit(Expr *) = 0;
    //virtual llvm::Value* visit(Type *)=0;
    virtual llvm::Value *visit(Field *) = 0;
    virtual llvm::Value *visit(Formal *) = 0;
    virtual llvm::Value *visit(Formalx *) = 0;
    virtual llvm::Value *visit(Formals *) = 0;
    virtual llvm::Value *visit(Exprx *) = 0;
    virtual llvm::Value *visit(Exprxx *) = 0;
    virtual llvm::Value *visit(Block *) = 0;
    virtual llvm::Value *visit(Method *) = 0;
    virtual llvm::Value *visit(FieldMethod *) = 0;
    virtual llvm::Value *visit(Body *) = 0;
    virtual llvm::Value *visit(Classe *) = 0;
    virtual llvm::Value *visit(Classes *) = 0;
    virtual llvm::Value *visit(Programm *) = 0;
    virtual llvm::Value *visit(Dual *) = 0;
    virtual llvm::Value *visit(Unary *) = 0;
    virtual llvm::Value *visit(If *) = 0;
    virtual llvm::Value *visit(While *) = 0;
    virtual llvm::Value *visit(Let *) = 0;
    virtual llvm::Value *visit(Assign *) = 0;
    virtual llvm::Value *visit(Not *) = 0;
    virtual llvm::Value *visit(And *) = 0;
    virtual llvm::Value *visit(Equal *) = 0;
    virtual llvm::Value *visit(Lower *) = 0;
    virtual llvm::Value *visit(LowerEqual *) = 0;
    virtual llvm::Value *visit(Plus *) = 0;
    virtual llvm::Value *visit(Minus *) = 0;
    virtual llvm::Value *visit(Times *) = 0;
    virtual llvm::Value *visit(Div *) = 0;
    virtual llvm::Value *visit(Pow *) = 0;
    virtual llvm::Value *visit(Minus1 *) = 0;
    virtual llvm::Value *visit(IsNull *) = 0;
    virtual llvm::Value *visit(Args *) = 0;
    virtual llvm::Value *visit(Function *) = 0;
    virtual llvm::Value *visit(Dot *) = 0;
    virtual llvm::Value *visit(New *) = 0;
    virtual llvm::Value *visit(ObjID *) = 0;
    virtual llvm::Value *visit(Literal *) = 0;
    virtual llvm::Value *visit(IntLit *) = 0;
    virtual llvm::Value *visit(StrLit *) = 0;
    virtual llvm::Value *visit(BoolLit *) = 0;
    virtual llvm::Value *visit(Lpar *) = 0;
    virtual llvm::Value *visit(Rpar *) = 0;
    virtual llvm::Value *visit(Parenthese *) = 0;
};

#endif //LlvmVisitor_HH
