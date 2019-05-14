//
// Created by Julien L'hoest on 14/05/19.
//
#include "CodeGenerator.hh"
#include "ASTnode.hh"

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

llvm::Value* CodeGenerator::accept(ASTnode* astNode) { return nullptr; }


llvm::Value* CodeGenerator::accept(Field* field) {
    //TODO implementation
    std::cout << "Field" << '\n';
    field->getExpr()->accept(this);
    return nullptr;
}


llvm::Value* CodeGenerator::accept(Programm* programm) {//TODO implementation(actually just for demo purposes)

    std::cout << "Program" << '\n';
    programm->getClasses()->accept(this);
    return nullptr;

}


llvm::Value* CodeGenerator::accept(Classes* classes) { //TODO implementation(actually just for demo purposes)

    std::cout << "Classes" << '\n';
    while(classes->getClass() != nullptr) {
        classes->getClass()->accept(this);
        classes = classes->nextClass();
    }
    return nullptr;
}


llvm::Value* CodeGenerator::accept(Classe* classe) {

    std::cout << "Classe" << '\n';

    // Get fields and methods name and type for corresponding class
    std::unordered_map<std::string,FieldPrototype> fields = ::prototype[classe->getTypeID()].field;
    std::unordered_map<std::string,MethodPrototype> methods = ::prototype[classe->getTypeID()].method;
    std::vector<std::string> fieldsKeys = ::prototype[classe->getTypeID()].fieldKeys;
    std::vector<std::string> methodsKeys = ::prototype[classe->getTypeID()].methodKeys;


    std::vector<llvm::Type* > vt;

    // TODO L'iterable renvoie des paires, composer avec ces paires
    // We first declare the field in the type declaration

    for(std::string key : fieldsKeys){

        FieldPrototype fieldPrototype = fields[key];

        if(fieldPrototype.type == "int32")
            vt.push_back(llvm::Type::getInt32Ty(TheContext));
        else if(fieldPrototype.type == "bool")
            vt.push_back(llvm::Type::getInt1Ty(TheContext));
        else if(fieldPrototype.type == "string")
            vt.push_back(llvm::Type::getInt8PtrTy(TheContext));
        else // Get a class type already declared
            vt.push_back(ClassesType[fieldPrototype.type]);
    }

    // Then the function
    std::vector<llvm::Type*> argsType;
    llvm::Type* pt = llvm::Type::getDoubleTy(TheContext);

    // TODO L'iterable renvoie des paires, composer avec ces paires
    for(std::string key  : methodsKeys){

        MethodPrototype methodPrototype = methods[key];

        // First the return type
        if(methodPrototype.return_type == "int32")
            pt = llvm::Type::getInt32Ty(TheContext);
        else if(methodPrototype.return_type == "bool")
            pt = llvm::Type::getInt1Ty(TheContext);
        else if(methodPrototype.return_type == "string")
            pt = llvm::Type::getInt8PtrTy(TheContext);
        else // Get a class type already declared
            pt = ClassesType[methodPrototype.return_type];

        // Then the arguments' types
        for(std::string t : methodPrototype.arguments){
            if(t == "int32")
                argsType.push_back(llvm::Type::getInt32Ty(TheContext));
            else if(t == "bool")
                argsType.push_back(llvm::Type::getInt1Ty(TheContext));
            else if(t == "string")
                argsType.push_back(llvm::Type::getInt8PtrTy(TheContext));
            else // Get a class type already declared
                argsType.push_back(ClassesType[t]);
        }

        vt.push_back(llvm::FunctionType::get(pt, argsType, false )); // Boolean for variable nb of arg
    }



    llvm::ArrayRef<llvm::Type*> ar = llvm::makeArrayRef(vt);

    llvm::StructType* st = llvm::StructType::create(TheContext , ar, classe->getTypeID() );

    // Add new type in global table
    ClassesType[classe->getTypeID()] = st;

    // TODO continu implementation
    // TODO implementation of get field/method access with the GEP => getElementPointer llvm defined function

    //TODO what follow has to be removed, it is just to call the rest of the tree and see what happens

    classe->getBody()->accept(this);

    return nullptr;



}


llvm::Value* CodeGenerator::accept(Block* block) {

    std::cout << "Block" << '\n';

    if(block->getExpr() == nullptr)
        return llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext)); // Noop
    //TODO Check the meaning of a no op and if this could be an ok alternative

    block->getExpr()->accept(this);
    block->getExprx()->getExpr()->accept(this);

    Exprx* e = block->getExprx();
    Expr* exp = block->getExpr();
    while(e->getExpr() != nullptr){
        exp->accept(this);
        exp = e->getExpr();
        e = e->getExprx();
    }

    return exp->accept(this);

}


llvm::Value* CodeGenerator::accept(FieldMethod* fieldMethod) {

    //TODO implementation
    std::cout << "FieldMethod" << '\n';

    // Fields and methods are returned backwards

    std::vector<Method*> m;
    std::vector<Field*> f;

    FieldMethod* fm = fieldMethod;
    while(fm->getFieldMethod() != nullptr){
        if(fm->getField() != nullptr)
            f.emplace_back(fm->getField());
        else
            m.emplace_back(fm->getMethod());
        fm = fm->getFieldMethod();
    }

    while(!f.empty()){
        f.back()->accept(this);
        f.pop_back();
    }

    while(!m.empty()){
        m.back()->accept(this);
        m.pop_back();
    }


    return nullptr;

}


llvm::Value* CodeGenerator::accept(Method* method) {

    std::cout << "Method" << '\n';

    llvm::Type* PT;

    if(method->getType()->getID() == "string"){
        PT = llvm::Type::getInt8PtrTy(TheContext);
    }else if(method->getType()->getID() == "int32"){
        PT = llvm::Type::getInt32Ty(TheContext);
    }else if(method->getType()->getID() == "bool"){
        PT = llvm::Type::getInt1Ty(TheContext);
    }else {
        //TODO Le cas d'une classe prédéfini
        PT = llvm::Type::getInt1Ty(TheContext);
    }

    std::vector<Formal* > fs;
    if(method->getFormals()->getFormal() != nullptr){
        fs.push_back(method->getFormals()->getFormal());
        Formalx* f = method->getFormals()->getFormalx();

        while(f->getFormal() != nullptr){
            fs.push_back(f->getFormal());
            f = f->getFormalx();
        }
    }

    std::vector<llvm::Type*> ArgumentsType;
    for(int i = 0, s = fs.size(); i < s; i++){

        if(fs[i]->getType()->getID() == "string"){
            ArgumentsType.push_back(llvm::Type::getInt8PtrTy(TheContext));
        }else if(fs[i]->getType()->getID() == "int32"){
            ArgumentsType.push_back(llvm::Type::getInt32Ty(TheContext));
        }else if(fs[i]->getType()->getID() == "bool"){
            ArgumentsType.push_back(llvm::Type::getInt1Ty(TheContext));
        }else {
            //TODO Le cas d'une classe prédéfini
            ArgumentsType.push_back(llvm::Type::getInt1Ty(TheContext));
        }

    }



    llvm::FunctionType* FT = llvm::FunctionType::get(PT, ArgumentsType, false); // Boolean to indicate variable nb of arg or not (like printf(string, arg1, arg2, ...) )
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, method->getID(), TheModule.get());

    // Naming declaration arguments (not necessary)
    // F->args and fs have same size
    int j = 0;
    for (auto &Arg : F->args()){
        Arg.setName(fs[j]->getID());
        j++;
    }

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    // Return instruction handle by Block codegen()
    llvm::Value* Block = method->getBlock()->accept(this);

    // Type has already be checked, it is "safe" enough to just take the function type as condition, Block last expr is ok tho
    //TODO Check if good condition and type for Unit
    if(Block->getType() == llvm::Type::getVoidTy(TheContext))
        Builder.CreateRetVoid();
    else
        Builder.CreateRet(Block);




    return F;
}


llvm::Value* CodeGenerator::accept(Body* body) {

    //TODO implementation
    std::cout << "Body" << '\n';

    body->getFieldMethod()->accept(this);

    return nullptr;

}


llvm::Value* CodeGenerator::accept(If* anIf) {

    std::cout << "If" << '\n';

    // Value of the conditional (i8)
    llvm::Value* CondV = anIf->getIf()->accept(this);

    // Need the function to insert the block in the right place
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(TheContext, "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(TheContext, "ifcont");

    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then value.
    Builder.SetInsertPoint(ThenBB);

    // Value of Then
    llvm::Value* ThenV = anIf->getThen()->accept(this);

    // Branch to the merging block
    Builder.CreateBr(MergeBB);

    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder.GetInsertBlock();

    // Emit else block.
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    llvm::Value* ElseV;
    if(anIf->getElse() != nullptr)
        ElseV = anIf->getElse()->accept(this);
    else //TODO Check if this is correct or not => Interpretation of Unit == NoOP ?
        ElseV = llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext)); // Noop

    Builder.CreateBr(MergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder.GetInsertBlock();

    // Emit merge block.
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);

    llvm::PHINode* PN = Builder.CreatePHI(ThenV->getType(), 2, "iftmp");

    // Add an incoming value to the end of the PHI list
    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    return PN;

}


llvm::Value* CodeGenerator::accept(While* aWhile) {

    std::cout << "While" << '\n';

    // TODO Change PHInode* by AllocaInst*
    // TODO Check if still usefull to manipulate allocation (Probably not) use it again for For Loop implementation

    // Get current function
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    // Value of the conditional (i8)
    llvm::Value* CondV = aWhile->getWhile()->accept(this);

//    // Create an alloca for the variable in the current block.
//    llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, "whilecond");

//    // Store the value into alloca
//    Builder.CreateStore(CondV, Alloca);

    // Make the new basic block for the loop header, inserting after current
    // block.
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "loop", TheFunction);

    // Insert an explicit fall through from the current block to the LoopBB.
    Builder.CreateBr(LoopBB);

    // Start insertion in LoopBB.
    Builder.SetInsertPoint(LoopBB);

//    // Within the loop, the variable is defined equal to the PHI node.  If it
//    // shadows an existing variable, we have to restore it, so save it now.
//    llvm::AllocaInst *OldVal = NamedValues["whilecond"]; //TODO Is using names like this ok? Seems ok
//    NamedValues["whilecond"] = Alloca;


    // Emit the body of the loop.  This, like any other expr, can change the
    // current BB.
    llvm::Value* Body = aWhile->getDo()->accept(this);

    //TODO Oh dear we are in trouble => Is it really a correct flow ? (seems ok)
    llvm::Value* NextC = aWhile->getWhile()->accept(this);

    // Create the "after loop" block and insert it.
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "afterloop", TheFunction);

    // Insert the conditional branch into the end of LoopEndBB.
    Builder.CreateCondBr(NextC, LoopBB, AfterBB);

    // Any new code will be inserted in AfterBB.
    Builder.SetInsertPoint(AfterBB);

//    // Restore the unshadowed variable if any, else remove the current one.
//    if (OldVal)
//        NamedValues["whilecond"] = OldVal;
//    else
//        NamedValues.erase("whilecond");

    // For while body always returns Unit TODO check representation of unit
    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext));

}


llvm::Value* CodeGenerator::accept(Assign* assign) {

    std::cout << "Assign" << '\n';

    llvm::Value* Val = assign->getExpr()->accept(this);

    llvm::Value* Obj = NamedValues[assign->getObjID()];

    Builder.CreateStore(Val, Obj);

    return Val; // C style => x = (y = z) applied in vsop

}


llvm::Value* CodeGenerator::accept(Let* let) {

    std::cout << "Let" << '\n';

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "letbody", Builder.GetInsertBlock()->getParent()); // Get the parent (function) of the actual block
    Builder.SetInsertPoint(BB);

    // Default constructor or not
    llvm::Value* Val;
    if(let->getAssign() == nullptr){
        // TODO checl if initiator ok by constant
        if(let->getType()->getID() == "int32")
            CreateEntryBlockAlloca(llvm::Type::getInt32Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0, true)), let->getObjID());
        else if(let->getType()->getID() == "bool")
            CreateEntryBlockAlloca(llvm::Type::getInt1Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(1, 0)), let->getObjID());
        else if(let->getType()->getID() == "string")
            CreateEntryBlockAlloca(llvm::Type::getInt32Ty(TheContext), llvm::ConstantDataArray::getString(TheContext, llvm::StringRef("")), let->getObjID());
        else // TODO initiator for class type
            CreateEntryBlockAlloca(llvm::Type::getInt1Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(1, 0)), let->getObjID());

    }
    else{

        llvm::Value* Init = let->getAssign()->accept(this);
        //TODO
        CreateEntryBlockAlloca(Init->getType(), Init, let->getObjID());

    }

    llvm::Value* LetBody = let->getIn()->accept(this);

    // TODO do we need to create a new block to be the new insertion block?

    return LetBody;

}


llvm::Value* CodeGenerator::accept(Function* function) {

    std::cout << "Function" << '\n';

    std::vector<llvm::Value*> ArgsVal;
    std::vector<Expr*> ArgsExpr;

    // Look up the name in the global module table.
    llvm::Function* functionCalled = TheModule->getFunction(function->getID());

    // Need to isolate the arguments of the function and get their llvm expression
    // Check if the first one is empty
    if(function->getArgs()->getExpr() != nullptr) {
        ArgsExpr.push_back( function->getArgs()->getExpr());
        struct Exprxx* exprxx = function->getArgs()->getExprxx();
        while(exprxx != nullptr){
            ArgsExpr.push_back( exprxx->getExpr());
            exprxx = exprxx->getExprxx();
        }
    }

    for (int i = 0, s = ArgsExpr.size(); i < s; ++i) {
        ArgsVal.push_back(ArgsExpr[i]->accept(this));
    }

    return Builder.CreateCall(functionCalled, ArgsVal, "fctcall");
}

//TODO IMPLEMENT POWER
llvm::Value* CodeGenerator::accept(Pow* pow) { std::cout << "pow" << '\n'; return nullptr;}


//TODO IMPLEMENT DYNAMIC DISPATCH 
llvm::Value* CodeGenerator::accept(Dot* dot) { std::cout << "dot" << '\n'; return nullptr;}

//TODO IMPLEMENT NEW
llvm::Value* CodeGenerator::accept(New* anew) {std::cout << "new" << '\n'; return nullptr;}

llvm::Value* CodeGenerator::accept(ObjID* objId) { std::cout << "ObjID" << '\n';
//TODO The goal is to send back the value only, no need to load in a register CHANGE THAT IF NEEDED
    return Builder.CreateLoad(NamedValues[objId->getID()], objId->getID()); } // No check done, should not cause error access (still bad habit)


llvm::Value* CodeGenerator::accept(IntLit* intLit) {std::cout << "IntLit" << '\n'; return llvm::ConstantInt::get(TheContext, llvm::APInt(32, intLit->getValue(), true));}


llvm::Value* CodeGenerator::accept(StrLit* strLit) { std::cout << "StrLit" << '\n'; return llvm::ConstantDataArray::getString(TheContext, llvm::StringRef(strLit->getValue())); }


llvm::Value* CodeGenerator::accept(BoolLit* boolLit) {std::cout << "BoolLit" << '\n'; return Builder.getInt1(boolLit->getValue()); }


llvm::Value* CodeGenerator::accept(And* anAnd) {std::cout << "And" << '\n'; return Builder.CreateAnd(anAnd->getLeft()->accept(this), anAnd->getRight()->accept(this), "andop"); }


llvm::Value* CodeGenerator::accept(Not* aNot) {std::cout << "Not" << '\n'; return Builder.CreateNot(aNot->getExpr()->accept(this), "notop"); }


llvm::Value* CodeGenerator::accept(Equal* equal) {std::cout << "Equal" << '\n'; return Builder.CreateICmpEQ(equal->getLeft()->accept(this), equal->getRight()->accept(this), "equbool"); }


llvm::Value* CodeGenerator::accept(Lower* lower) {std::cout << "Lower" << '\n'; return Builder.CreateICmpSLT(lower->getLeft()->accept(this), lower->getRight()->accept(this), "lowtha"); }


llvm::Value* CodeGenerator::accept(LowerEqual* lowerEqual) {std::cout << "LowerEqual" << '\n'; return Builder.CreateICmpSLE(lowerEqual->getLeft()->accept(this), lowerEqual->getRight()->accept(this), "lowequ");}


llvm::Value* CodeGenerator::accept(Plus* plus) {std::cout << "Plus" << '\n'; return Builder.CreateFAdd(plus->getLeft()->accept(this), plus->getRight()->accept(this), "addop");}


llvm::Value* CodeGenerator::accept(Minus* minus) {std::cout << "Minus" << '\n'; return Builder.CreateFSub(minus->getLeft()->accept(this), minus->getRight()->accept(this), "subop");}


llvm::Value* CodeGenerator::accept(Times* times) {std::cout << "Times" << '\n'; return Builder.CreateFMul(times->getLeft()->accept(this), times->getRight()->accept(this), "mulop");}


llvm::Value* CodeGenerator::accept(Div* div) {std::cout << "Div" << '\n'; return Builder.CreateFDiv(div->getLeft()->accept(this), div->getRight()->accept(this), "divop"); }


llvm::Value* CodeGenerator::accept(Minus1* minus1) {std::cout << "Minus1" << '\n'; return Builder.CreateFSub( llvm::ConstantInt::get(TheContext, llvm::APInt(32,0)), minus1->getExpr()->accept(this), "minusop"); }


llvm::Value* CodeGenerator::accept(IsNull* isNull) {std::cout << "IsNull" << '\n'; return Builder.CreateIsNull(isNull->getExpr()->accept(this), "isnullbool"); }

llvm::Value* CodeGenerator::accept(Parenthese* parenthese) {std::cout << "()" << '\n'; return parenthese->getExpr()->accept(this);}