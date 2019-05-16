//
// Created by Julien L'hoest on 14/05/19.
//
#include "CodeGenerator.hh"
#include "ASTnode.hh"

llvm::Value* CodeGenerator::visit(ASTnode* astNode) { return nullptr; }


llvm::Value* CodeGenerator::visit(Field* field) {
    //TODO implementation
    std::cout << "Field" << '\n';
    field->getExpr()->accept(this);
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Programm* programm) {

    fill_class_type();
    fill_method_proto();
    programm->getClasses()->accept(this);
    return nullptr;

}


llvm::Value* CodeGenerator::visit(Classes* classes) { //TODO implementation(actually just for demo purposes)

    std::cout << "Classes" << '\n';
    while(classes->getClass() != nullptr) {
        classes->getClass()->accept(this);
        classes = classes->nextClass();
    }
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Classe* classe) {

    std::cout << "Classe" << '\n';

    // Get fields and methods name and type for corresponding class
    std::unordered_map<std::string,FieldPrototype> fields = ::prototype[classe->getTypeID()].field;
    std::unordered_map<std::string,MethodPrototype> methods = ::prototype[classe->getTypeID()].method;
    //std::vector<std::string> fieldsKeys = ::prototype[classe->getTypeID()].fieldKeys;
    //std::vector<std::string> methodsKeys = ::prototype[classe->getTypeID()].methodKeys;


    std::vector<llvm::Type* > vt;

    // TODO L'iterable renvoie des paires, composer avec ces paires
    // We first declare the field in the type declaration
    for(auto it = fields.begin(); it != fields.end();++it){
        std::cout<<fields.size()<<endl;
        std::cout<<"hello in loop"<<endl;
        FieldPrototype fieldPrototype = it->second;
        std::cout<<"filed prorotype set"<<endl;
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
    for(auto it = methods.begin(); it != methods.end();++it){
        std::cout<<"hello in second loop"<<endl;

        MethodPrototype methodPrototype = it->second;

        // First the return type
        if(methodPrototype.return_type == "int32")
            pt = llvm::Type::getInt32Ty(TheContext);
        else if(methodPrototype.return_type == "bool")
            pt = llvm::Type::getInt1Ty(TheContext);
        else if(methodPrototype.return_type == "string")
            pt = llvm::Type::getInt8PtrTy(TheContext);
        else 
            cout<<"Get a class type already declared"<<endl;
            pt = ClassesType[methodPrototype.return_type];

        // Then the arguments' types
        std::cout<<"arguments"<<endl;
        for(std::string t : methodPrototype.arguments){
            if(t == "int32")
                argsType.push_back(llvm::Type::getInt32Ty(TheContext));
            else if(t == "bool")
                argsType.push_back(llvm::Type::getInt1Ty(TheContext));
            else if(t == "string")
                argsType.push_back(llvm::Type::getInt8PtrTy(TheContext));
            else // 
                cout<<"Get a class type already declared"<<endl;
                argsType.push_back(ClassesType[t]);
        }
        cout<<"push back"<<endl;
        pt->print(llvm::outs()) ;
        cout<<endl;
        //vt.push_back(llvm::FunctionType::get(pt, argsType, false )); // Boolean for variable nb of arg
    }
    std::cout<<"end two loop"<<endl;


    llvm::ArrayRef<llvm::Type*> ar = llvm::makeArrayRef(vt);

    llvm::StructType* st = llvm::StructType::create(TheContext , ar, classe->getTypeID() );

    // Add new type in global table -> TODO : c'est toujours utile de le faire ? on le connait déjà non ?
    ClassesType[classe->getTypeID()] = st;

    // TODO continu implementation
    // TODO implementation of get field/method access with the GEP => getElementPointer llvm defined function

    //TODO what follow has to be removed, it is just to call the rest of the tree and see what happens

    classe->getBody()->accept(this);

    return nullptr;



}


llvm::Value* CodeGenerator::visit(Block* block) {

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

//implement Field method declaration.
llvm::Value* CodeGenerator::visit(FieldMethod* fieldMethod) {

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

//implement the prototype of a method
llvm::Value* CodeGenerator::visit(Method* method) {

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


llvm::Value* CodeGenerator::visit(Body* body) {

    //TODO implementation
    std::cout << "Body" << '\n';

    body->getFieldMethod()->accept(this);

    return nullptr;

}


llvm::Value* CodeGenerator::visit(If* anIf) {

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


llvm::Value* CodeGenerator::visit(While* aWhile) {

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


llvm::Value* CodeGenerator::visit(Assign* assign) {

    std::cout << "Assign" << '\n';

    llvm::Value* Val = assign->getExpr()->accept(this);

    llvm::Value* Obj = NamedValues[assign->getObjID()];

    Builder.CreateStore(Val, Obj);

    return Val; // C style => x = (y = z) applied in vsop

}


llvm::Value* CodeGenerator::visit(Let* let) {

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

//generate code for call to a function
llvm::Value* CodeGenerator::visit(Function* function) {

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
llvm::Value* CodeGenerator::visit(Pow* pow) { std::cout << "pow" << '\n'; return nullptr;}


//TODO IMPLEMENT DYNAMIC DISPATCH 
llvm::Value* CodeGenerator::visit(Dot* dot) { std::cout << "dot" << '\n'; return nullptr;}

//TODO IMPLEMENT NEW
llvm::Value* CodeGenerator::visit(New* anew) {std::cout << "new" << '\n'; return nullptr;}

llvm::Value* CodeGenerator::visit(ObjID* objId) { std::cout << "ObjID" << '\n';
//TODO The goal is to send back the value only, no need to load in a register CHANGE THAT IF NEEDED
    return Builder.CreateLoad(NamedValues[objId->getID()], objId->getID()); } // No check done, should not cause error access (still bad habit)


llvm::Value* CodeGenerator::visit(IntLit* intLit) {std::cout << "IntLit" << '\n'; return llvm::ConstantInt::get(TheContext, llvm::APInt(32, intLit->getValue(), true));}


llvm::Value* CodeGenerator::visit(StrLit* strLit) { std::cout << "StrLit" << '\n'; return llvm::ConstantDataArray::getString(TheContext, llvm::StringRef(strLit->getValue())); }


llvm::Value* CodeGenerator::visit(BoolLit* boolLit) {std::cout << "BoolLit" << '\n'; return Builder.getInt1(boolLit->getValue()); }


llvm::Value* CodeGenerator::visit(And* anAnd) {std::cout << "And" << '\n'; return Builder.CreateAnd(anAnd->getLeft()->accept(this), anAnd->getRight()->accept(this), "andop"); }


llvm::Value* CodeGenerator::visit(Not* aNot) {std::cout << "Not" << '\n'; return Builder.CreateNot(aNot->getExpr()->accept(this), "notop"); }


llvm::Value* CodeGenerator::visit(Equal* equal) {std::cout << "Equal" << '\n'; return Builder.CreateICmpEQ(equal->getLeft()->accept(this), equal->getRight()->accept(this), "equbool"); }


llvm::Value* CodeGenerator::visit(Lower* lower) {std::cout << "Lower" << '\n'; return Builder.CreateICmpSLT(lower->getLeft()->accept(this), lower->getRight()->accept(this), "lowtha"); }


llvm::Value* CodeGenerator::visit(LowerEqual* lowerEqual) {std::cout << "LowerEqual" << '\n'; return Builder.CreateICmpSLE(lowerEqual->getLeft()->accept(this), lowerEqual->getRight()->accept(this), "lowequ");}


llvm::Value* CodeGenerator::visit(Plus* plus) {std::cout << "Plus" << '\n'; return Builder.CreateFAdd(plus->getLeft()->accept(this), plus->getRight()->accept(this), "addop");}


llvm::Value* CodeGenerator::visit(Minus* minus) {std::cout << "Minus" << '\n'; return Builder.CreateFSub(minus->getLeft()->accept(this), minus->getRight()->accept(this), "subop");}


llvm::Value* CodeGenerator::visit(Times* times) {std::cout << "Times" << '\n'; return Builder.CreateFMul(times->getLeft()->accept(this), times->getRight()->accept(this), "mulop");}


llvm::Value* CodeGenerator::visit(Div* div) {std::cout << "Div" << '\n'; return Builder.CreateFDiv(div->getLeft()->accept(this), div->getRight()->accept(this), "divop"); }


llvm::Value* CodeGenerator::visit(Minus1* minus1) {std::cout << "Minus1" << '\n'; return Builder.CreateFSub( llvm::ConstantInt::get(TheContext, llvm::APInt(32,0)), minus1->getExpr()->accept(this), "minusop"); }


llvm::Value* CodeGenerator::visit(IsNull* isNull) {std::cout << "IsNull" << '\n'; return Builder.CreateIsNull(isNull->getExpr()->accept(this), "isnullbool"); }

llvm::Value* CodeGenerator::visit(Parenthese* parenthese) {std::cout << "()" << '\n'; return parenthese->getExpr()->accept(this);}

llvm::Value* CodeGenerator::visit(Formal* formal) {std::cout << "Formal" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Formalx* formalx) {std::cout << "Formalx" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Formals* formals) {std::cout << "Formals" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Exprx* exprx) {std::cout << "Exprx" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Exprxx* exprxx) {std::cout << "Exprxx" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Expr* expr) {std::cout << "Expr" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Dual* dual) {std::cout << "dual" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Unary* unary) {std::cout << "unary" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Args* args) {std::cout << "args" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Literal* literal) {std::cout << "literal" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Lpar* lpar) {std::cout << "lpar" << '\n'; return nullptr;}
llvm::Value* CodeGenerator::visit(Rpar* rpar) {std::cout << "rpar" << '\n'; return nullptr;}

void CodeGenerator::fill_class_type(){
    //push primitive type into the ClasseType
    ClassesType["int32"] = llvm::Type::getInt32Ty(TheContext);
    ClassesType["bool"] = llvm::Type::getInt1Ty(TheContext);
    ClassesType["string"] = llvm::Type::getInt8PtrTy(TheContext);    
    for (std::pair<std::string, ClassPrototype> element : prototype)
    {
        if(ClassesType.find(element.first) == ClassesType.end()){
            fill_class_type_aux(element.first);
        }
        cout << "class " << element.first << " was pushed in ClassesType";
    }
    for (std::pair<std::string, llvm::Type *> element : ClassesType){
        if(element.second != nullptr){
        element.second->print(llvm::outs()) ;
        printf("\n");            
        }

    }
}

void CodeGenerator::fill_class_type_aux(string classID){
    unordered_map<std::string,FieldPrototype> fields = prototype[classID].field;
    std::vector<llvm::Type*> field_type = vector<llvm::Type*>();  //content type of inner field
    std::cout<<"making class " << classID << "with " << fields.size() << " fields" << endl;
    //classes without any field, nothing to do.
    if(fields.size() == 0){
        ClassesType[classID] = nullptr;
        return;
    }
    //classes with field => make a struct.
    for (auto field : fields){
        //deal before with unknow field
        cout << "making field " << field.first << endl;
        if(ClassesType.find(field.second.type) == ClassesType.end()){
            fill_class_type_aux(field.second.type);
        }
        field_type.push_back(ClassesType[field.second.type]);
        llvm::ArrayRef<llvm::Type*> field_array = llvm::ArrayRef<llvm::Type*>(field_type);
        llvm::Type* struct_type = llvm::StructType::create(TheContext,field_array,classID,true);
        if(struct_type == nullptr){
            cerr << "struct type is null";
        }
        cout << struct_type << endl;
        ClassesType[classID] = struct_type;
    }
    std::cout << "Type of class " << classID << " was done " << endl;
}

void CodeGenerator::fill_method_proto(){
    //check classes Types
    cout << "printing Classes Type" << endl;
    for (auto& it: ClassesType) {
    // Do stuff
    cout << it.first << endl;;
    }
    //TODO VTABLE 

    //loop over all classes
    for(auto class_pair: prototype){
        cout<<"making method proto of class " << class_pair.first << endl;
        //loop over all methods
        for(auto method_pair : class_pair.second.method){
            cout << "making method "<< method_pair.first << endl;
            std::vector<llvm::Type*> args_type = vector<llvm::Type*>();
            //push a pointer to classes as the first argument of the method
            if(ClassesType.find(class_pair.first) == ClassesType.end()){
                cerr<<"unknown class " << class_pair.first << endl;
            }
            //if there is any field in class, create a pointer to the class
            if(ClassesType[class_pair.first] != nullptr){
                llvm::Type* pointer_to_class = ClassesType[class_pair.first]->getPointerElementType();
                args_type.push_back(pointer_to_class);
            }
            //take the arguments of the methods
            for (auto arg : method_pair.second.arguments){
                if(ClassesType.find(arg) == ClassesType.end()){
                    cerr<<"unknown class: " << arg << endl;
                }
                cout << arg <<endl;
                args_type.push_back(ClassesType[arg]);
            }
            std::string method_name = class_pair.first + method_pair.first ;
            cout << "llvm stuff of " << method_name <<  endl;
            cout << method_pair.second.return_type << endl;
            //if class return type has no field, return of llvm::Type::voidTY, else the struct representing the field.
            llvm::Type* ret_type = llvm::Type::getVoidTy(TheContext);
            if(ClassesType[method_pair.second.return_type] != nullptr){
                ret_type = ClassesType[method_pair.second.return_type];
            }
            llvm::FunctionType *FT = llvm::FunctionType::get(ret_type, args_type, false);
            cout<<"create function"<<endl;
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, method_name, TheModule.get());
            cout << "end making method "<< method_name << endl;
        }
    }
}
