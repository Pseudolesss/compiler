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


class CodeGenerator 
{
public:
    llvm::Value* accept(ASTnode *);
    llvm::Value* accept(Expr *);
    llvm::Value* accept(Type *);
    llvm::Value* accept(Field *);
    llvm::Value* accept(Formal *);
    llvm::Value* accept(Formalx *);
    llvm::Value* accept(Formals *);
    llvm::Value* accept(Exprx *);
    llvm::Value* accept(Exprxx *);
    llvm::Value* accept(Block *);
    llvm::Value* accept(Method *);
    llvm::Value* accept(FieldMethod *);
    llvm::Value* accept(Body *);
    llvm::Value* accept(Classe *);
    llvm::Value* accept(Classes *);
    llvm::Value* accept(Programm *);
    llvm::Value* accept(Dual *);
    llvm::Value* accept(Unary *);
    llvm::Value* accept(If *);
    llvm::Value* accept(While *);
    llvm::Value* accept(Let *);
    llvm::Value* accept(Assign *);
    llvm::Value* accept(Not *);
    llvm::Value* accept(And *);
    llvm::Value* accept(Equal *);
    llvm::Value* accept(Lower *);
    llvm::Value* accept(LowerEqual *);
    llvm::Value* accept(Plus *);
    llvm::Value* accept(Minus *);
    llvm::Value* accept(Times *);
    llvm::Value* accept(Div *);
    llvm::Value* accept(Pow *);
    llvm::Value* accept(Minus1 *);
    llvm::Value* accept(IsNull *);
    llvm::Value* accept(Args *);
    llvm::Value* accept(Function *);
    llvm::Value* accept(Dot *);
    llvm::Value* accept(New *);
    llvm::Value* accept(ObjID *);
    llvm::Value* accept(Literal *);
    llvm::Value* accept(IntLit *);
    llvm::Value* accept(StrLit *);
    llvm::Value* accept(BoolLit *);
    llvm::Value* accept(Lpar *);
    llvm::Value* accept(Rpar *);
    llvm::Value* accept(Parenthese *);
};


#endif //COMPILER_CODEGENERATOR_HH
