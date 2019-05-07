#include "ASTnode.hh"

using namespace std;
ASTnode::ASTnode(yy::location l) : location(l)
{
    type = "";
    valueInh = "";
    valueSyn = "";
}
yy::location ASTnode::getLocation(){return location;}
std::string ASTnode::getValueInh() { return valueInh; }
std::string ASTnode::getValueSyn() { return valueSyn; }
std::string ASTnode::accept(Visitor *v) { return v->visit(this); }
void ASTnode::setType(std::string t) { type = t; }
void ASTnode::setValueInh(std::string v) { valueInh = v; }
void ASTnode::setValueSyn(std::string v) { valueSyn = v; }
llvm::Value* ASTnode::codegen() { return nullptr; }

Expr::Expr(yy::location l) : ASTnode(l){}
void Expr::setType(std::string type){dataType = type;}
string Expr::getDataType() { return dataType; }
std::string Expr::accept(Visitor *v) { return v->visit(this); }

Type::Type(string s,yy::location l) : ASTnode(l),typeID(s){};
std::string Type::getID() { return this->typeID; }
std::string Type::accept(Visitor *v) { return v->visit(this); }

Field::Field(string object_id, Type *a_type,yy::location l) : ASTnode(l),objID(object_id), type(a_type) { expr = nullptr; }
Field::Field(string object_id, Type *a_type, Expr *an_expr,yy::location l) : ASTnode(l),objID(object_id), type(a_type), expr(an_expr){};
string Field::getID() { return objID; }
Type *Field::getType() { return type; }
Expr *Field::getExpr() { return expr; }
std::string Field::accept(Visitor *v) { return v->visit(this); }

Formal::Formal(string s, Type *t,yy::location l) : ASTnode(l),objID(s), type(t){};
string Formal::getID() { return objID; }
Type *Formal::getType() { return type; }
std::string Formal::accept(Visitor *v) { return v->visit(this); }

Formalx::Formalx(yy::location l) : ASTnode(l)
{
    formal = nullptr;
    formalx = nullptr;
}
Formalx::Formalx(Formal *f, Formalx *fx,yy::location l) : ASTnode(l),formal(f), formalx(fx){};
Formal *Formalx::getFormal() { return formal; }
Formalx *Formalx::getFormalx() { return formalx; }
std::string Formalx::accept(Visitor *v) { return v->visit(this); }

Formals::Formals(yy::location l) : ASTnode(l)
{
    formal = nullptr;
    formalx = nullptr;
}
Formals::Formals(Formal *f, Formalx *fx,yy::location l) : ASTnode(l),formal(f), formalx(fx){};
Formal *Formals::getFormal() { return formal; }
Formalx *Formals::getFormalx() { return formalx; }
std::string Formals::accept(Visitor *v) { return v->visit(this); }

Exprx::Exprx(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprx = nullptr;
}
Exprx::Exprx(Expr *e, Exprx *ex,yy::location l) : ASTnode(l),expr(e), exprx(ex){};
Exprx *Exprx::getExprx() { return exprx; }
Expr *Exprx::getExpr() { return expr; }
std::string Exprx::accept(Visitor *v) { return v->visit(this); }

Block::Block(Expr *e, Exprx *ex,yy::location l) : Expr(l),expr(e), exprx(ex) { dataType = string("Block"); };
Expr *Block::getExpr() { return expr; }
Exprx *Block::getExprx() { return exprx; }
std::string Block::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Block::codegen() {

    if(getExpr() == nullptr)
        return llvm::Constant::getNullValue(llvm::Type::getVoidTy(TheContext)); // Noop
    //TODO Check the meaning of a no op and if this could be an ok alternative


    Exprx* e = getExprx();
    Expr* exp = getExpr();
    while(e->getExpr() != nullptr){
        exp->codegen();
        exp = e->getExpr();
        e = e->getExprx();
    }

    return e->codegen();

}

Method::Method(string s, Formals *fs, Type *t, Block *b,yy::location l) : ASTnode(l), objID(s), formals(fs), type(t), block(b){};
string Method::getID() { return objID; }
Formals *Method::getFormals() { return formals; }
Type *Method::getType() { return type; }
Block *Method::getBlock() { return block; }
std::string Method::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Method::codegen() {

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

FieldMethod::FieldMethod(yy::location l) : ASTnode(l)
{
    field = nullptr;
    method = nullptr;
    fieldMethod = nullptr;
}
FieldMethod::FieldMethod(FieldMethod *fm, Field *f,yy::location l) : ASTnode(l),fieldMethod(fm), field(f) { method = nullptr; }
FieldMethod::FieldMethod(FieldMethod *fm, Method *m,yy::location l) : ASTnode(l),fieldMethod(fm), method(m) { field = nullptr; }
Field *FieldMethod::getField() { return field; }
Method *FieldMethod::getMethod() { return method; }
FieldMethod *FieldMethod::getFieldMethod() { return fieldMethod; }
std::string FieldMethod::accept(Visitor *v) { return v->visit(this); }

Body::Body(FieldMethod *fm,yy::location l) : ASTnode(l),fieldMethod(fm) {}
FieldMethod *Body::getFieldMethod() { return fieldMethod; }
std::string Body::accept(Visitor *v) { return v->visit(this); }

Classe::Classe(string s, Body *b,yy::location l) : ASTnode(l),typeID(s), body(b) {}
Classe::Classe(string typeID, string parent, Body *b,yy::location l) : ASTnode(l),typeID(typeID), parentID(parent), body(b) {}
std::string Classe::getTypeID() { return typeID; }
std::string Classe::getParentID() { return parentID; }
Body *Classe::getBody() { return body; }
std::string Classe::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Classe::codegen() {

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
    // TODO implementation of get field/method access with the GEP

    return nullptr;



}

Classes::Classes(Classes *cs, Classe *c,yy::location l) : ASTnode(l),next_class(cs), a_class(c) {}
Classes::Classes(Classe *c,yy::location l) : a_class(c),ASTnode(l) { next_class = nullptr; }
Classe *Classes::getClass() { return a_class; }
Classes *Classes::nextClass() { return next_class; }
std::string Classes::accept(Visitor *v) { return v->visit(this); }

Programm::Programm(Classes *cs, yy::location l) : ASTnode(l),classes(cs) { classe = nullptr; }
Programm::Programm(Classe *c, yy::location l) : ASTnode(l), classe(c) { classes = nullptr; }
Classes *Programm::getClasses() { return classes; }
Classe *Programm::getClasse() { return classe; }
std::string Programm::accept(Visitor *v) { return v->visit(this); }

Dual::Dual(Expr *left, Expr *right,yy::location l) : Expr(l),left(left), right(right) { dataType = string("Dual"); }
Expr *Dual::getLeft() { return left; }
Expr *Dual::getRight() { return right; }
std::string Dual::accept(Visitor *v) { return v->visit(this); }

Unary::Unary(Expr *e,yy::location l) : Expr(l),expr(e) { dataType = string("Unary"); }
Expr *Unary::getExpr() { return expr; }
std::string Unary::accept(Visitor *v) { return v->visit(this); }

If::If(Expr *e1, Expr *e2, Expr *e3,yy::location l) :Expr(l), _if(e1), _then(e2), _else(e3) { dataType = string("If"); }
If::If(Expr *e1, Expr *e2,yy::location l) : Expr(l), _if(e1), _then(e2)
{
    _else = nullptr;
    dataType = string("If");
}
Expr *If::getElse() { return _else; }
Expr *If::getIf() { return _if; }
Expr *If::getThen() { return _then; }
std::string If::accept(Visitor *v) { return v->visit(this); }
llvm::Value* If::codegen() {

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

While::While(Expr *w, Expr *d,yy::location l) : Expr(l), _while(w), _do(d) { dataType = string("unit"); }
Expr *While::getWhile() { return _while; }
Expr *While::getDo() { return _do; }
std::string While::accept(Visitor *v) { return v->visit(this); }
llvm::Value* While::codegen() {

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

Let::Let(string s, Type *t, Expr *e,yy::location l) : Expr(l), ObjID(s), type(t), in(e)
{
    assign = nullptr;
    dataType = string("Let");
}
Let::Let(string s, Type *t, Expr *ass, Expr *in,yy::location l) : Expr(l),ObjID(s), type(t), assign(ass), in(in) { dataType = string("Let"); }
Type *Let::getType() { return type; }
string Let::getObjID() { return ObjID; }
Expr *Let::getAssign() { return assign; }
Expr *Let::getIn() { return in; }
std::string Let::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Let::codegen() {

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

Assign::Assign(string s, Expr *e,yy::location l) : Expr(l), ObjID(s), expr(e) { dataType = string("Assign"); }
Expr *Assign::getExpr() { return expr; }
std::string Assign::getObjID() { return ObjID; }
std::string Assign::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Assign::codegen() {

    llvm::Value* Val = getExpr()->codegen();

    llvm::Value* Obj = NamedValues[getObjID()];

    Builder.CreateStore(Val, Obj);

    return Val; // C style => x = (y = z) applied in vsop

}

Exprxx::Exprxx(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprxx = nullptr;
}
Exprxx::Exprxx(Expr *e, Exprxx *ex,yy::location l) : ASTnode(l),expr(e), exprxx(ex) {}
Exprxx *Exprxx::getExprxx() { return exprxx; }
Expr *Exprxx::getExpr() { return expr; }
std::string Exprxx::accept(Visitor *v) { return v->visit(this); }

Args::Args(yy::location l) : ASTnode(l)
{
    expr = nullptr;
    exprxx = nullptr;
}
Args::Args(Expr *e, Exprxx *ex,yy::location l) : ASTnode(l), expr(e), exprxx(ex) {}
Expr *Args::getExpr() { return expr; }
Exprxx *Args::getExprxx() { return exprxx; }
std::string Args::accept(Visitor *v) { return v->visit(this); }

Function::Function(string name, Args *a,yy::location l) : Expr(l),args(a), ID(name) { dataType = string("Function"); }
std::string Function::getID() { return ID; }
Args *Function::getArgs() { return args; }
std::string Function::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Function::codegen() {

    std::vector<llvm::Value*> ArgsVal;
    std::vector<Args*> ArgsVec;

    // Look up the name in the global module table.
    llvm::Function* functionCalled = TheModule->getFunction(getID());

    // Need to isolate the arguments of the function and get their llvm expression
    // Check if the first one is empty
    if(getArgs()->getExpr() != nullptr) {
        ArgsVec.push_back( (Args*) getArgs()->getExpr());
        while (ArgsVec.back()->getExpr() != nullptr)
            ArgsVec.push_back( (Args*) ArgsVec.back()->getExpr());
    }

    for (int i = 0, s = ArgsVec.size(); i < s; ++i) {
        ArgsVal.push_back(ArgsVec[i]->codegen());
    }

    return Builder.CreateCall(functionCalled, ArgsVal, "fctcall");
}

Dot::Dot(Expr *e, string s, Args *a,yy::location l) : Expr(l),expr(e), ID(s), args(a) { dataType = string("Dot"); }
Expr *Dot::getExpr() { return expr; }
Args *Dot::getArgs() { return args; }
std::string Dot::getID() { return ID; }
std::string Dot::accept(Visitor *v) { return v->visit(this); }

New::New(string s,yy::location l) : Expr(l),typeID(s) { dataType = s; }
string New::getTypeID() { return typeID; }
std::string New::accept(Visitor *v) { return v->visit(this); }

ObjID::ObjID(string s,yy::location l) : Expr(l),ObjId(s) { dataType = string("Object"); }
string ObjID::getID() { return ObjId; }
std::string ObjID::accept(Visitor *v) { return v->visit(this); }
llvm::Value* ObjID::codegen() { return Builder.CreateLoad(NamedValues[getID()], getID().c_str()); } // No check done, should not cause error access (still bad habit)

Literal::Literal(yy::location l) : Expr(l) {}

IntLit::IntLit(int i,yy::location l) : Literal(l),value(i) {}
int IntLit::getValue() { return value; }
std::string IntLit::accept(Visitor *v) { return v->visit(this); }
llvm::Value* IntLit::codegen() {return llvm::ConstantInt::get(TheContext, llvm::APInt(32, getValue(), true));}

StrLit::StrLit(string s,yy::location l) : Literal(l),value(s) {}
string StrLit::getValue() { return value; }
std::string StrLit::accept(Visitor *v) { return v->visit(this); }
llvm::Value* StrLit::codegen() { return llvm::ConstantDataArray::getString(TheContext, llvm::StringRef(getValue())); }

BoolLit::BoolLit(bool v,yy::location l) : Literal(l),value(v) {}
bool BoolLit::getValue() { return value; }
std::string BoolLit::accept(Visitor *v) { return v->visit(this); }
llvm::Value* BoolLit::codegen() { return Builder.getInt1(getValue()); }

And::And(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string And::accept(Visitor *v) { return v->visit(this); }
llvm::Value* And::codegen() { return Builder.CreateAnd(getLeft()->codegen(), getRight()->codegen(), "andop"); }

Not::Not(Expr *e,yy::location l) : Unary(e,l){};
std::string Not::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Not::codegen() { return Builder.CreateNot(getExpr()->codegen(), "notop"); }

Equal::Equal(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Equal::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Equal::codegen() { return Builder.CreateICmpEQ(getLeft()->codegen(), getRight()->codegen(), "equbool"); }

Lower::Lower(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Lower::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Lower::codegen() { return Builder.CreateICmpSLT(getLeft()->codegen(), getRight()->codegen(), "lowtha"); }

LowerEqual::LowerEqual(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string LowerEqual::accept(Visitor *v) { return v->visit(this); }
llvm::Value* LowerEqual::codegen() { return Builder.CreateICmpSLE(getLeft()->codegen(), getRight()->codegen(), "lowequ");}

Plus::Plus(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Plus::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Plus::codegen() {return Builder.CreateFAdd(getLeft()->codegen(), getRight()->codegen(), "addop");}

Minus::Minus(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Minus::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Minus::codegen() {return Builder.CreateFSub(getLeft()->codegen(), getRight()->codegen(), "subop");}


Times::Times(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Times::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Times::codegen() {return Builder.CreateFMul(getLeft()->codegen(), getRight()->codegen(), "mulop");}

Div::Div(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l){};
std::string Div::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Div::codegen() { return Builder.CreateFDiv(getLeft()->codegen(), getRight()->codegen(), "divop"); }

Pow::Pow(Expr *e1, Expr *e2,yy::location l) : Dual(e1, e2,l ){};
std::string Pow::accept(Visitor *v) { return v->visit(this); }

Minus1::Minus1(Expr *e,yy::location l) : Unary(e,l ){};
std::string Minus1::accept(Visitor *v) { return v->visit(this); }
llvm::Value* Minus1::codegen() { return Builder.CreateFSub( llvm::ConstantInt::get(TheContext, llvm::APInt(32,0)), getExpr()->codegen(), "minusop"); }

IsNull::IsNull(Expr *e,yy::location l) : Unary(e,l ){};
std::string IsNull::accept(Visitor *v) { return v->visit(this); }
llvm::Value* IsNull::codegen() { return Builder.CreateIsNull(getExpr()->codegen(), "isnullbool"); }

Parenthese::Parenthese(Expr *e,yy::location l) : Unary(e,l ){};
std::string Parenthese::accept(Visitor *v) { return v->visit(this); }
