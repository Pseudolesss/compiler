//
// Created by Julien L'hoest on 14/05/19.
//
#include "CodeGenerator.hh"
#include "ASTnode.hh"

llvm::Value* CodeGenerator::visit(ASTnode* astNode) { return nullptr; }


llvm::Value* CodeGenerator::visit(Field* field) {
    std::cout << "Field" <<std::endl;
    //Keep the ssigned field value in a table if it exist.
    if(field->getExpr() != nullptr){
        Def_field_value[classID + field->getID()] = field->getExpr()->accept(this);
    }
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Programm* programm) {
    fill_class_type();
    fill_method_proto();
    //create object main

    //lauch method main.


    programm->getClasses()->accept(this);
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Classes* classes) { //TODO implementation(actually just for demo purposes)

    std::cout << "Classes" <<std::endl;
    while(classes->getClass() != nullptr) {
        classes->getClass()->accept(this);
        classes = classes->nextClass();
    }
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Classe* classe) {
    std::cout << "Classe" <<std::endl;
    //set the current visited class;
    classID = classe->getTypeID();
    //clear the NameSpace
    NamedValues.clear();
    //Deal with the body 
    classe->getBody()->accept(this);
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Block* block) {

    std::cout << "Block" << endl;

    if(block->getExpr() == nullptr)
        return llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext)); // Noop
    //TODO Check the meaning of a no op and if this could be an ok alternative

    Exprx* e = block->getExprx();
    Expr* exp = block->getExpr();
    while(e->getExpr() != nullptr){
        cout<<"in while loop"<<endl;
        exp->accept(this);
        exp = e->getExpr();
        e = e->getExprx();
    }

    return exp->accept(this);

}

llvm::Value* CodeGenerator::visit(FieldMethod* fieldMethod) {
    std::cout << "FieldMethod" <<std::endl;
    std::vector<Field*> f = std::vector<Field*>();
    std::vector<Method*> m = std::vector<Method*>();
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

//implement the method
llvm::Value* CodeGenerator::visit(Method* method) {
    cout<<"Method: "<< method->getID() << endl;
    llvm::Function *F = TheModule->getFunction(classID + method->getID());
    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "method" + method->getID() + "entry", F);
    Builder.SetInsertPoint(BB);
    //create alloca for all argument of the function
    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : F->args()) {
        // Create an alloca for this variable.
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(F, Arg.getType(), Arg.getName());
        // Store the initial value into the alloca.
        Builder.CreateStore(&Arg, Alloca);
        // Add arguments to variable symbol table for the body.
        NamedValues[Arg.getName()] = Alloca;
    }
    // Return instruction handle by Block 
    cout<<"making block of method "<< method->getID() << endl;
    llvm::Value* Block = method->getBlock()->accept(this);
    // Type has already be checked, it is "safe" enough to just take the function type as condition, Block last expr is ok tho
    if(Block->getType() == llvm::Type::getVoidTy(TheContext))
        Builder.CreateRetVoid();
    else
        Builder.CreateRet(Block);
    cout << "method " << method->getID() << "done " << endl;
    return nullptr;
}


llvm::Value* CodeGenerator::visit(Body* body) {
    std::cout << "Body" <<std::endl;
    body->getFieldMethod()->accept(this);
    return nullptr;
}


llvm::Value* CodeGenerator::visit(If* anIf) {

    std::cout << "If" <<std::endl;

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

    std::cout << "While" <<std::endl;

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

    std::cout << "Assign" <<std::endl;

    llvm::Value* Val = assign->getExpr()->accept(this);

    llvm::Value* Obj = NamedValues[assign->getObjID()];

    Builder.CreateStore(Val, Obj);

    return Val; // C style => x = (y = z) applied in vsop

}


llvm::Value* CodeGenerator::visit(Let* let) {

    std::cout << "Let" <<std::endl;

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

//generate code for call to a function.
llvm::Value* CodeGenerator::visit(Function* function) {
    std::cout << "Function" <<std::endl;
    std::vector<llvm::Value*> ArgsVal;
    std::vector<Expr*> ArgsExpr;

    //Lookup for the right name in the global module table.
    std::cout << classID <<std::endl;
    llvm::Function* functionCalled = TheModule->getFunction(classID + function->getID());

    std::string classe = classID;    
    while(functionCalled->empty()){
        classe = prototype[classe].direct_parent;
        functionCalled = TheModule->getFunction(classe + function->getID());
        if(functionCalled == nullptr){
            cout<<"undefined function "<< classe + function->getID() << endl;
            return nullptr;
        }         
    }
    std::cout<<"loaded function " << classe + function->getID() << endl;
    //push pointer to the object as first argument, but object main not yet create !
    //ArgsVal.push_back();

    // Need to isolate the arguments of the function and get their llvm expression
    // Check if the first one is empty
    if(function->getArgs()->getExpr() != nullptr) {
        ArgsExpr.push_back( function->getArgs()->getExpr());
        struct Exprxx* exprxx = function->getArgs()->getExprxx();
        while(exprxx->getExprxx() != nullptr){        
            std::cout<<"getting args"<<std::endl;
            ArgsExpr.push_back( exprxx->getExpr());
            exprxx = exprxx->getExprxx();
        }
    }
    std::cout<<ArgsExpr.size()<<std::endl;
    for (int i = 0, s = ArgsExpr.size(); i < s; ++i) {    
        std::cout<<"pushing args"<<std::endl;
        ArgsVal.push_back(ArgsExpr[i]->accept(this));
        std::cout<<"args "<<ArgsExpr[i]->getLocation() << "pushed" <<std::endl;
    }
    std::cout<<"create call"<<std::endl;
    //return Builder.CreateCall(functionCalled, ArgsVal, "fctcall");
    return nullptr;
}

//TODO IMPLEMENT POWER
llvm::Value* CodeGenerator::visit(Pow* pow) { std::cout << "pow" <<std::endl; return nullptr;}


//TODO IMPLEMENT DYNAMIC DISPATCH 
llvm::Value* CodeGenerator::visit(Dot* dot) { 
    
    
    
    
    std::cout << "dot" <<std::endl; return nullptr;}

//TODO IMPLEMENT NEW
llvm::Value* CodeGenerator::visit(New* anew) {
    
    std::cout << "new" <<std::endl; return nullptr;}

llvm::Value* CodeGenerator::visit(ObjID* objId) { std::cout << "ObjID" << std::endl;
//TODO The goal is to send back the value only, no need to load in a register CHANGE THAT IF NEEDED
    if(NamedValues.find(objId->getID()) != NamedValues.end()){
        std::cout<<"load object " << objId->getID() << std::endl;
        llvm::Value* v = Builder.CreateLoad(NamedValues[objId->getID()], objId->getID()); 
        std::cout<<"value loaded" << std::endl;
        return v;
    } 
    std::cout << "object " << objId->getID() << "unknown" << std::endl;
    return nullptr;}


llvm::Value* CodeGenerator::visit(IntLit* intLit) {std::cout << "IntLit" <<std::endl; return llvm::ConstantInt::get(TheContext, llvm::APInt(32, intLit->getValue(), true));}


llvm::Value* CodeGenerator::visit(StrLit* strLit) { std::cout << "StrLit" <<std::endl; return llvm::ConstantDataArray::getString(TheContext, llvm::StringRef(strLit->getValue())); }


llvm::Value* CodeGenerator::visit(BoolLit* boolLit) {std::cout << "BoolLit" <<std::endl; return Builder.getInt1(boolLit->getValue()); }


llvm::Value* CodeGenerator::visit(And* anAnd) {std::cout << "And" <<std::endl; return Builder.CreateAnd(anAnd->getLeft()->accept(this), anAnd->getRight()->accept(this), "andop"); }


llvm::Value* CodeGenerator::visit(Not* aNot) {std::cout << "Not" <<std::endl; return Builder.CreateNot(aNot->getExpr()->accept(this), "notop"); }


llvm::Value* CodeGenerator::visit(Equal* equal) {std::cout << "Equal" <<std::endl; return Builder.CreateICmpEQ(equal->getLeft()->accept(this), equal->getRight()->accept(this), "equbool"); }


llvm::Value* CodeGenerator::visit(Lower* lower) {std::cout << "Lower" <<std::endl; return Builder.CreateICmpSLT(lower->getLeft()->accept(this), lower->getRight()->accept(this), "lowtha"); }


llvm::Value* CodeGenerator::visit(LowerEqual* lowerEqual) {std::cout << "LowerEqual" <<std::endl; return Builder.CreateICmpSLE(lowerEqual->getLeft()->accept(this), lowerEqual->getRight()->accept(this), "lowequ");}


llvm::Value* CodeGenerator::visit(Plus* plus) {std::cout << "Plus" <<std::endl; return Builder.CreateFAdd(plus->getLeft()->accept(this), plus->getRight()->accept(this), "addop");}


llvm::Value* CodeGenerator::visit(Minus* minus) {std::cout << "Minus" <<std::endl; return Builder.CreateFSub(minus->getLeft()->accept(this), minus->getRight()->accept(this), "subop");}


llvm::Value* CodeGenerator::visit(Times* times) {std::cout << "Times" <<std::endl; return Builder.CreateFMul(times->getLeft()->accept(this), times->getRight()->accept(this), "mulop");}


llvm::Value* CodeGenerator::visit(Div* div) {std::cout << "Div" <<std::endl; return Builder.CreateFDiv(div->getLeft()->accept(this), div->getRight()->accept(this), "divop"); }


llvm::Value* CodeGenerator::visit(Minus1* minus1) {std::cout << "Minus1" <<std::endl; return Builder.CreateFSub( llvm::ConstantInt::get(TheContext, llvm::APInt(32,0)), minus1->getExpr()->accept(this), "minusop"); }


llvm::Value* CodeGenerator::visit(IsNull* isNull) {std::cout << "IsNull" <<std::endl; return Builder.CreateIsNull(isNull->getExpr()->accept(this), "isnullbool"); }

llvm::Value* CodeGenerator::visit(Parenthese* parenthese) {std::cout << "()" <<std::endl; return parenthese->getExpr()->accept(this);}

llvm::Value* CodeGenerator::visit(Formal* formal) {std::cout << "Formal" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Formalx* formalx) {std::cout << "Formalx" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Formals* formals) {std::cout << "Formals" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Exprx* exprx) {std::cout << "Exprx" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Exprxx* exprxx) {std::cout << "Exprxx" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Expr* expr) {std::cout << "Expr" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Dual* dual) {std::cout << "dual" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Unary* unary) {std::cout << "unary" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Args* args) {std::cout << "args" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Literal* literal) {std::cout << "literal" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Lpar* lpar) {std::cout << "lpar" << endl; return nullptr;}
llvm::Value* CodeGenerator::visit(Rpar* rpar) {std::cout << "rpar" << endl; return nullptr;}
void CodeGenerator::fill_class_type(){
    //push primitive type into the ClasseType
    ClassesType["int32"] = llvm::Type::getInt32Ty(TheContext);
    ClassesType["bool"] = llvm::Type::getInt1Ty(TheContext);
    ClassesType["string"] = llvm::Type::getInt8PtrTy(TheContext);  
    std::cout << "hello"<<std::endl;  
    for (std::pair<std::string, ClassPrototype> element : prototype)
    {
        if(ClassesType.find(element.first) == ClassesType.end()){
            fill_class_type_aux(element.first);
        }
        cout<<"printing field of "<<element.first<<endl;
        for(auto field : element.second.fieldKeys){
            cout << field << " , ";
        }
        cout<<"printing method of "<<element.first<<endl;
        for(auto method : element.second.methodKeys){
            cout << method << " , ";
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
    std::vector<std::string> fields = prototype[classID].fieldKeys;
    std::vector<llvm::Type*> field_type = vector<llvm::Type*>();  //content type of inner field
    std::cout<<"making class " << classID << "with " << fields.size() << " fields" << endl;
    //push a ghost field "class" to get the dynamic type when needed
    field_type.push_back(ClassesType["string"]);
    //classes with field => make a struct.
    for (auto field : fields){
        //deal before with unknow field
        std::string type = ::prototype[classID].field[field].type;
        if(ClassesType.find(type) == ClassesType.end()){
            fill_class_type_aux(type);
        }
        field_type.push_back(ClassesType[type]);
        //llvm::ArrayRef<llvm::Type*> field_array = llvm::ArrayRef<llvm::Type*>(field_type);
    }
    llvm::Type* struct_type = llvm::StructType::create(TheContext,field_type,classID,true);
    if(struct_type == nullptr){
        cerr << "struct type is null";
    }
    ClassesType[classID] = struct_type;    
    std::cout << "Type of class " << classID << " was done " << endl;
}

void CodeGenerator::fill_method_proto(){
    //check classes Types
    //cout << "printing Classes Type" << endl;
    //loop over all classes
    for(auto class_pair: prototype){
        cout<<"making method proto of class " << class_pair.first << endl;
        //loop over all methods
        for(auto method_pair : class_pair.second.method){
            cout << "making method "<< method_pair.first << endl;
            std::vector<llvm::Type*> args_type = vector<llvm::Type*>();
            //push a pointer to classes as the first argument of the method
            //if there is any field in class, create a pointer to the class
            if(ClassesType[class_pair.first] != nullptr){
                llvm::Type* pointer_to_class = ClassesType[class_pair.first]->getPointerElementType();
                args_type.push_back(pointer_to_class);
            }
            //take the arguments of the methods
            for (auto arg : method_pair.second.arguments){
                if(ClassesType.find(arg) == ClassesType.end()){
                    //cerr<<"unknown class: " << arg << endl;
                }
                cout << arg <<endl;
                args_type.push_back(ClassesType[arg]);
            }
            std::string method_name = class_pair.first + method_pair.first ;
            //if class return type has no field, return of llvm::Type::voidTY, else the struct representing the field.
            llvm::Type* ret_type = llvm::Type::getVoidTy(TheContext);
            if(ClassesType[method_pair.second.return_type] != nullptr){
                ret_type = ClassesType[method_pair.second.return_type];
            }
            llvm::FunctionType *FT = llvm::FunctionType::get(ret_type, args_type, false);
            cout<<"create function"<<endl;
            llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, method_name, TheModule.get());
            cout << "args_name size: "<<prototype[classID].method[method_pair.first].arguments_name.size()<<endl;
            auto args_name = method_pair.second.arguments_name.begin();  
            int i=0;
            cout<<"function arg size: " << F->arg_size();
            for (auto &Arg : F->args()){
                //because first arg of a method is pointer on the class, should pass it.
                if( i> 0){
                    cout<<"i "<< i << endl;              
                    Arg.setName(*args_name);
                    ++args_name;                     
                }
                i++;
            }
            cout << "end making method "<< method_name << endl;
        }
    }
}

void CodeGenerator::allocator(std::string classID, llvm::Function* f, std::string VarName){
    llvm::Type* type = ClassesType[classID];
    llvm::Value* def_value = llvm::Constant::getNullValue(type);
    llvm::AllocaInst* alloca = CreateEntryBlockAlloca(f,type,VarName);
    Builder.CreateStore(def_value, alloca);
}
