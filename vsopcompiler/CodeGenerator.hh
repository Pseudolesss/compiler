//
// Created by Julien L'hoest on 14/05/19.
//

#ifndef COMPILER_CODEGENERATOR_HH
#define COMPILER_CODEGENERATOR_HH


#include "Visitor.hh"
#include <string>

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


static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule = llvm::make_unique<llvm::Module>("TODOPutCorrectFileName", TheContext);
static std::map<std::string, llvm::AllocaInst *> NamedValues;
static std::map<std::string, llvm::StructType *> ClassesType;

// Because in vsop, every variable have a default initial value TODO Check if this is true for Class types (fields should be initialized by default)
// @args Variable's type, Variable's value, Variable's name.
static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Type* VarType, llvm::Value* VarValue , const std::string &VarName) {

    // TODO If needed, add Block argument to allocate memory where they are declared instead of function entry
    llvm::IRBuilder<> TmpB(Builder.GetInsertBlock(), Builder.GetInsertBlock()->begin());

    return TmpB.CreateAlloca(VarType, 0, VarValue, VarName.c_str());
}


class CodeGenerator : public Visitor
{
public:
    llvm::Value* codegen(ASTnode *);
    llvm::Value* codegen(Expr *);
    llvm::Value* codegen(Type *);
    llvm::Value* codegen(Field *);
    llvm::Value* codegen(Formal *);
    llvm::Value* codegen(Formalx *);
    llvm::Value* codegen(Formals *);
    llvm::Value* codegen(Exprx *);
    llvm::Value* codegen(Exprxx *);
    llvm::Value* codegen(Block *);
    llvm::Value* codegen(Method *);
    llvm::Value* codegen(FieldMethod *);
    llvm::Value* codegen(Body *);
    llvm::Value* codegen(Classe *);
    llvm::Value* codegen(Classes *);
    llvm::Value* codegen(Programm *);
    llvm::Value* codegen(Dual *);
    llvm::Value* codegen(Unary *);
    llvm::Value* codegen(If *);
    llvm::Value* codegen(While *);
    llvm::Value* codegen(Let *);
    llvm::Value* codegen(Assign *);
    llvm::Value* codegen(Not *);
    llvm::Value* codegen(And *);
    llvm::Value* codegen(Equal *);
    llvm::Value* codegen(Lower *);
    llvm::Value* codegen(LowerEqual *);
    llvm::Value* codegen(Plus *);
    llvm::Value* codegen(Minus *);
    llvm::Value* codegen(Times *);
    llvm::Value* codegen(Div *);
    llvm::Value* codegen(Pow *);
    llvm::Value* codegen(Minus1 *);
    llvm::Value* codegen(IsNull *);
    llvm::Value* codegen(Args *);
    llvm::Value* codegen(Function *);
    llvm::Value* codegen(Dot *);
    llvm::Value* codegen(New *);
    llvm::Value* codegen(ObjID *);
    llvm::Value* codegen(Literal *);
    llvm::Value* codegen(IntLit *);
    llvm::Value* codegen(StrLit *);
    llvm::Value* codegen(BoolLit *);
    llvm::Value* codegen(Lpar *);
    llvm::Value* codegen(Rpar *);
    llvm::Value* codegen(Parenthese *);
};


#endif //COMPILER_CODEGENERATOR_HH
