//AST node class header.

using namespace std;

//faut pas oublier d'inclure visitor.cpp à cause du template !
struct ASTnode
{
  public:
    virtual template <class R>
    accept(Visitor<R>);
};
struct Programm : ASTnode
{
  public:
    Programm(Classes);
    Programm(Classe);
    Classes getChildren();
    Classe getChild();

  private:
    Classes children;
    Classe child
};
struct Classes : ASTnode
{
  public:
    Classes(Classes, Classe);
    Classes(Classe);
    Classe getClass();
    Classes nextClass();
    String getID();
    Body getBody();
    String getParentID();

  private:
    Classe a_class;
    Body body;
    String ID;
    String parent_ID;
    Classes next_class;
};
struct Body : ASTnode
{
  public:
    Body(Field_method);
    FieldMethod getFieldMethod();

  private:
    FieldMethod field_method;
};
struct FieldMethod : ASTnode
{
  public:
    FieldMethod(FieldMethod, Field);
    FieldMethod(FieldMethod, Method);
    FieldMethod()
        Field getField();
    Method getMethod();
    FieldMethod getFieldMethod();

  private:
    Field field;
    Method method;
    FieldMethod filed_method;
};
struct Field : ASTnode
{
  public:
    Field(String, Type);
    Field(String, Type, Expr);
    String getID();
    Type getType();
    Expr getExpr();

  private:
    String objID;
    Type type;
    Expr expr;
};

struct Method : ASTnode
{
  public:
    Method(String, Formals, Type, Block);
    String getID();
    Formals getFormals();
    Type getType();
    Block getBlock();

  private:
    String objID;
    Formals formals;
    Type type;
    Block block;
};

struct Type : ASTnode
{
  public:
    Type(String);
    Type getType();

  private:
    String Type;
};

struct Formals : ASTnode
{
  public:
    Formals()
        Formals(Formal, Formalx)
            Formal getFormal();
    Formalx getFormalx();

  private:
    Formal formal;
    Formalx formalx;
};

struct Formalx : ASTnode
{
  public:
    Formalx();
    Formalx(Formal, Formalx);
    Formal getFormal();
    Formalx getFormalx();

  private:
    Formal formal;
    Formalx formalx;
};

struct Formal : ASTnode
{
  public:
    Formal(String, Type);
    String getID();
    Type getType();

  private:
    String objID;
    Type type;
};

struct Block : Expr
{
  public:
    Block(Expr, Exprx);
    Expr getExpr();
    Exprx getExprx();

  private:
    Expr expr;
    Exprx exprx;
};

struct Expr : ASTnode
{ //nothing to do.... i think
};
struct Dual : Expr
{
  public:
    Dual(Expr, Expr)
        Expr getLeft();
    Expr getRight();

  private:
    Expr left;
    Expr right;
};
struct Unary : Expr
{
  public:
    Unary(Expr);
    getExpr();

  private:
    Expr expr;
};
struct If : Expr
{
  public:
    If(Expr, Expr, Expr);
    If(Expr, Expr);
    Expr getIf();
    Expr getThen();
    Expr getElse();

  private:
    Expr _if;
    Expr _then;
    Expr _else;
};
struct While : Expr
{
  public:
    While(Expr, Expr);
    Expr getWhile();
    Expr getDo();

  private:
    Expr _while;
    Expr _do;
};
struct Let : Expr
{
  public:
    Let(String, Type, Expr);
    Let(String, Type, Expr, Expr);
    String getObjID();
    Type getType();
    Expr getAssign();
    Expr getIn();

  private:
    String ObjID;
    Type type;
    Expr assign;
    Expr in;
};
struct Assign : Expr
{
  public:
    Assign(String, Expr);
    String getObjID();
    Expr getExpr();

  private:
    String ObjID;
    Expr expr;
};
struct Not : Unary
{
};
struct And : Dual
{
};
struct Equal : Dual
{
};
struct Lower : Dual
{
};
struct LowerEqual : Dual
{
};
struct Plus : Dual
{
};
struct Minus : Dual
{
};
struct Times : Dual
{
};
struct Div : Dual
{
};
struct Pow : Dual
{
};
struct Minus1 : Unary
{
};
struct IsNull : Unary
{
};
struct Function : Expr
{
  public:
    Function(String, Args);
    String getID();
    Args getArgs();

  private:
    String ID;
    Args args;
};
struct Dot : Expr
{
  public:
    Dot(Expr, String, Args);
    Expr getExpr();
    Args getArgs();

  private:
    Expr expr;
    String ID;
    Args args;
};
struct New : Expr
{
  public:
    New(String);
    String getTypeID();

  private:
    String typeID;
};
struct ObjID : Expr
{
  public:
    ObjID(String);
    String getID();

  private:
    String ObjID;
};
struct Literal : Expr
{
};
struct IntLit : Literal
{
  public:
    IntLit(int);
    int getValue();

  private:
    int value;
};
struct StrLit : Literal
{
  public:
    StrLit(String);
    String getValue();

  private:
    String value;
};
struct BoolLit : Literal
{
  public:
    BoolLit(bool);
    bool getValue();

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
};
struct Args
{
  public:
    Args();
    Args(Expr, Exprx);
    Expr getExpr();
    Exprxx getExprxx();

  private:
    Expr expr;
    Exprxx exprxx;
};
