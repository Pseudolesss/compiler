//
// Created by Julien L'hoest on 14/05/19.
//

llvm::Value* CodeGenerator::codegen(ASTnode* astNode) { return nullptr; }


llvm::Value* CodeGenerator::codegen(Field* field) {
    //TODO implementation
    std::cout << "Field" << '\n';

    getExpr()->codegen();

    return nullptr;


}


llvm::Value* CodeGenerator::codegen(Programm* programm) {//TODO implementation(actually just for demo purposes)

    std::cout << "Program" << '\n';
    getClasses()->codegen();
    return nullptr;

}


llvm::Value* CodeGenerator::codegen(Classes* classes) { //TODO implementation(actually just for demo purposes)

    std::cout << "Classes" << '\n';
    Classes* classes = this;
    while(classes->getClass() != nullptr) {
        classes->getClass()->codegen();
        classes = classes->nextClass();
    }

    return nullptr;
}


llvm::Value* CodeGenerator::codegen(Classe* classe) {

    std::cout << "Classe" << '\n';

    // Get fields and methods name and type for corresponding class
    std::unordered_map<std::string,FieldPrototype> fields = ::prototype[getTypeID()].field;
    std::unordered_map<std::string,MethodPrototype> methods = ::prototype[getTypeID()].method;
    std::vector<std::string> fieldsKeys = ::prototype[getTypeID()].fieldKeys;
    std::vector<std::string> methodsKeys = ::prototype[getTypeID()].methodKeys;


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

    llvm::StructType* st = llvm::StructType::create(TheContext , ar, getTypeID() );

    // Add new type in global table
    ClassesType[getTypeID()] = st;

    // TODO continu implementation
    // TODO implementation of get field/method access with the GEP => getElementPointer llvm defined function

    //TODO what follow has to be removed, it is just to call the rest of the tree and see what happens

    getBody()->codegen();

    return nullptr;



}


llvm::Value* CodeGenerator::codegen(Block* block) {

    std::cout << "Block" << '\n';

    if(getExpr() == nullptr)
        return llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext)); // Noop
    //TODO Check the meaning of a no op and if this could be an ok alternative

    getExpr()->codegen();
    getExprx()->getExpr()->codegen();

    Exprx* e = getExprx();
    Expr* exp = getExpr();
    while(e->getExpr() != nullptr){
        exp->codegen();
        exp = e->getExpr();
        e = e->getExprx();
    }

    return exp->codegen();

}


llvm::Value* CodeGenerator::codegen(FieldMethod* fieldMethod) {

    //TODO implementation
    std::cout << "FieldMethod" << '\n';

    // Fields and methods are returned backwards

    std::vector<Method*> m;
    std::vector<Field*> f;

    FieldMethod* fm = this;
    while(fm->getFieldMethod() != nullptr){
        if(fm->getField() != nullptr)
            f.emplace_back(fm->getField());
        else
            m.emplace_back(fm->getMethod());
        fm = fm->getFieldMethod();
    }

    while(!f.empty()){
        f.back()->codegen();
        f.pop_back();
    }

    while(!m.empty()){
        m.back()->codegen();
        m.pop_back();
    }


    return nullptr;

}


llvm::Value* CodeGenerator::codegen(Method* method) {

    std::cout << "Method" << '\n';

    llvm::Type* PT;

    if(getType()->getID() == "string"){
        PT = llvm::Type::getInt8PtrTy(TheContext);
    }else if(getType()->getID() == "int32"){
        PT = llvm::Type::getInt32Ty(TheContext);
    }else if(getType()->getID() == "bool"){
        PT = llvm::Type::getInt1Ty(TheContext);
    }else {
        //TODO Le cas d'une classe prédéfini
        PT = llvm::Type::getInt1Ty(TheContext);
    }

    std::vector<Formal* > fs;
    if(getFormals()->getFormal() != nullptr){
        fs.push_back(getFormals()->getFormal());
        Formalx* f = getFormals()->getFormalx();

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
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, getID(), TheModule.get());

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
    llvm::Value* Block = getBlock()->codegen();

    // Type has already be checked, it is "safe" enough to just take the function type as condition, Block last expr is ok tho
    //TODO Check if good condition and type for Unit
    if(Block->getType() == llvm::Type::getVoidTy(TheContext))
        Builder.CreateRetVoid();
    else
        Builder.CreateRet(Block);




    return F;
}


llvm::Value* CodeGenerator::codegen(Body* body) {

    //TODO implementation
    std::cout << "Body" << '\n';

    getFieldMethod()->codegen();

    return nullptr;

}


llvm::Value* CodeGenerator::codegen(If* anIf) {

    std::cout << "If" << '\n';

    // Value of the conditional (i8)
    llvm::Value* CondV = getIf()->codegen();

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
    llvm::Value* ThenV = getThen()->codegen();

    // Branch to the merging block
    Builder.CreateBr(MergeBB);

    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder.GetInsertBlock();

    // Emit else block.
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    llvm::Value* ElseV;
    if(getElse() != nullptr)
        ElseV = getElse()->codegen();
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


llvm::Value* CodeGenerator::codegen(While* aWhile) {

    std::cout << "While" << '\n';

    // TODO Change PHInode* by AllocaInst*
    // TODO Check if still usefull to manipulate allocation (Probably not) use it again for For Loop implementation

    // Get current function
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();

    // Value of the conditional (i8)
    llvm::Value* CondV = getWhile()->codegen();

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
    llvm::Value* Body = getDo()->codegen();

    //TODO Oh dear we are in trouble => Is it really a correct flow ? (seems ok)
    llvm::Value* NextC = getWhile()->codegen();

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


llvm::Value* CodeGenerator::codegen(Assign* assign) {

    std::cout << "Assign" << '\n';

    llvm::Value* Val = getExpr()->codegen();

    llvm::Value* Obj = NamedValues[getObjID()];

    Builder.CreateStore(Val, Obj);

    return Val; // C style => x = (y = z) applied in vsop

}


llvm::Value* CodeGenerator::codegen(Let* let) {

    std::cout << "Let" << '\n';

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "letbody", Builder.GetInsertBlock()->getParent()); // Get the parent (function) of the actual block
    Builder.SetInsertPoint(BB);

    // Default constructor or not
    llvm::Value* Val;
    if(getAssign() == nullptr){
        // TODO checl if initiator ok by constant
        if(getType()->getID() == "int32")
            CreateEntryBlockAlloca(llvm::Type::getInt32Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0, true)), getObjID());
        else if(getType()->getID() == "bool")
            CreateEntryBlockAlloca(llvm::Type::getInt1Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(1, 0)), getObjID());
        else if(getType()->getID() == "string")
            CreateEntryBlockAlloca(llvm::Type::getInt32Ty(TheContext), llvm::ConstantDataArray::getString(TheContext, llvm::StringRef("")), getObjID());
        else // TODO initiator for class type
            CreateEntryBlockAlloca(llvm::Type::getInt1Ty(TheContext), llvm::ConstantInt::get(TheContext, llvm::APInt(1, 0)), getObjID());

    }
    else{

        llvm::Value* Init = getAssign()->codegen();

        CreateEntryBlockAlloca(Init->getType(), Init, getObjID());

    }

    llvm::Value* LetBody = getIn()->codegen();

    // TODO do we need to create a new block to be the new insertion block?

    return LetBody;

}


llvm::Value* CodeGenerator::codegen(Function* function) {

    std::cout << "Function" << '\n';

    std::vector<llvm::Value*> ArgsVal;
    std::vector<Args*> ArgsVec;

    // Look up the name in the global module table.
    llvm::Function* functionCalled = TheModule->getFunction(getID());

    // Need to isolate the arguments of the function and get their llvm expression
    // Check if the first one is empty
    if(function->getArgs()->getExpr() != nullptr) {
        ArgsVec.push_back( (Args*) function->getArgs()->getExpr());
        while (ArgsVec.back()->getExpr() != nullptr)
            ArgsVec.push_back( (Args*) ArgsVec.back()->getExpr());
    }

    for (int i = 0, s = ArgsVec.size(); i < s; ++i) {
        ArgsVal.push_back(ArgsVec[i]->codegen());
    }

    return Builder.CreateCall(functionCalled, ArgsVal, "fctcall");
}


llvm::Value* CodeGenerator::codegen(ObjID* objId) { std::cout << "ObjID" << '\n';
//TODO The goal is to send back the value only, no need to load in a register CHANGE THAT IF NEEDED
    return Builder.CreateLoad(NamedValues[getID()], getID()); } // No check done, should not cause error access (still bad habit)


llvm::Value* CodeGenerator::codegen(IntLit* intLit) {std::cout << "IntLit" << '\n'; return llvm::ConstantInt::get(TheContext, llvm::APInt(32, intLit->getValue(), true));}


llvm::Value* CodeGenerator::codegen(StrLit* strLit) { std::cout << "StrLit" << '\n'; return llvm::ConstantDataArray::getString(TheContext, llvm::StringRef(strLit->getValue())); }


llvm::Value* CodeGenerator::codegen(BoolLit* boolLit) {std::cout << "BoolLit" << '\n'; return Builder.getInt1(boolLit->getValue()); }


llvm::Value* CodeGenerator::codegen(And* anAnd) {std::cout << "And" << '\n'; return Builder.CreateAnd(anAnd->getLeft()->codegen(), anAnd->getRight()->codegen(), "andop"); }


llvm::Value* CodeGenerator::codegen(Not* aNot) {std::cout << "Not" << '\n'; return Builder.CreateNot(aNot->getExpr()->codegen(), "notop"); }


llvm::Value* CodeGenerator::codegen(Equal* equal) {std::cout << "Equal" << '\n'; return Builder.CreateICmpEQ(equal->getLeft()->codegen(), equal->getRight()->codegen(), "equbool"); }


llvm::Value* CodeGenerator::codegen(Lower* lower) {std::cout << "Lower" << '\n'; return Builder.CreateICmpSLT(lower->getLeft()->codegen(), lower->getRight()->codegen(), "lowtha"); }


llvm::Value* CodeGenerator::codegen(LowerEqual* lowerEqual) {std::cout << "LowerEqual" << '\n'; return Builder.CreateICmpSLE(lowerEqual->getLeft()->codegen(), lowerEqual->getRight()->codegen(), "lowequ");}


llvm::Value* CodeGenerator::codegen(Plus* plus) {std::cout << "Plus" << '\n'; return Builder.CreateFAdd(plus->getLeft()->codegen(), plus->getRight()->codegen(), "addop");}


llvm::Value* CodeGenerator::codegen(Minus* minus) {std::cout << "Minus" << '\n'; return Builder.CreateFSub(minus->getLeft()->codegen(), minus->getRight()->codegen(), "subop");}


llvm::Value* CodeGenerator::codegen(Times* times) {std::cout << "Times" << '\n'; return Builder.CreateFMul(times->getLeft()->codegen(), times->getRight()->codegen(), "mulop");}


llvm::Value* CodeGenerator::codegen(Div* div) {std::cout << "Div" << '\n'; return Builder.CreateFDiv(div->getLeft()->codegen(), div->getRight()->codegen(), "divop"); }


llvm::Value* CodeGenerator::codegen(Minus1* minus1) {std::cout << "Minus1" << '\n'; return Builder.CreateFSub( llvm::ConstantInt::get(TheContext, llvm::APInt(32,0)), minus1->getExpr()->codegen(), "minusop"); }


llvm::Value* CodeGenerator::codegen(IsNull* isNull) {std::cout << "IsNull" << '\n'; return Builder.CreateIsNull(isNull->getExpr()->codegen(), "isnullbool"); }
