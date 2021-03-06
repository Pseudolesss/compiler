//
// Created by Julien L'hoest on 14/05/19.
//

#ifndef COMPILER_CODEGENERATOR_HH
#define COMPILER_CODEGENERATOR_HH

#include "LlvmVisitor.hh"

class CodeGenerator : LlvmVisitor
{
public:
    llvm::Value *visit(ASTnode *);
    llvm::Value *visit(Expr *);
    llvm::Value *visit(Dual *);
    llvm::Value *visit(Unary *);
    llvm::Value *visit(Literal *);
    llvm::Value *visit(Field *);
    llvm::Value *visit(Formal *);
    llvm::Value *visit(Formalx *);
    llvm::Value *visit(Formals *);
    llvm::Value *visit(Exprx *);
    llvm::Value *visit(Exprxx *);
    llvm::Value *visit(Block *);
    llvm::Value *visit(Method *);
    llvm::Value *visit(FieldMethod *);
    llvm::Value *visit(Body *);
    llvm::Value *visit(Classe *);
    llvm::Value *visit(Classes *);
    llvm::Value *visit(Programm *);
    llvm::Value *visit(If *);
    llvm::Value *visit(While *);
    llvm::Value *visit(Let *);
    llvm::Value *visit(Assign *);
    llvm::Value *visit(Not *);
    llvm::Value *visit(And *);
    llvm::Value *visit(Equal *);
    llvm::Value *visit(Lower *);
    llvm::Value *visit(LowerEqual *);
    llvm::Value *visit(Plus *);
    llvm::Value *visit(Minus *);
    llvm::Value *visit(Times *);
    llvm::Value *visit(Div *);
    llvm::Value *visit(Pow *);
    llvm::Value *visit(Minus1 *);
    llvm::Value *visit(IsNull *);
    llvm::Value *visit(Args *);
    llvm::Value *visit(Function *);
    llvm::Value *visit(Dot *);
    llvm::Value *visit(New *);
    llvm::Value *visit(ObjID *);
    llvm::Value *visit(IntLit *);
    llvm::Value *visit(StrLit *);
    llvm::Value *visit(BoolLit *);
    llvm::Value *visit(Lpar *);
    llvm::Value *visit(Rpar *);
    llvm::Value *visit(Parenthese *);

private:
    void fill_class_type();
    void fill_class_type_aux(std::string);
    void fill_method_proto();
    std::string classID;
    //default initialize of a variable in a function.
    void allocator(std::string classID, llvm::Function* f, std::string VarName);
    void create_malloc_function();
    void create_main();
    std::unordered_map<std::string,std::unordered_map<std::string,llvm::AllocaInst*>> class_variables_table;
};

#endif //COMPILER_CODEGENERATOR_HH
