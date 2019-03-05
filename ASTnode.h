//AST node class header.

using namespace std;

//faut pas oublier d'inclure visitor.cpp à cause du template !
struct ASTnode
{
    public:
        virtual template <class R> accept(Visitor<R>);
};
struct Programm : ASTnode
{
    public:
        Programm(Classes);
        Programm(Classe );        
        Classes getChildren();
        Classe getChild();

    private:
        Classes children;
        Classe child
};
struct Classes :  ASTnode
{
    public:
        Classes(Classes, Classe);
        Classes(Classe );
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
struct  FieldMethod : ASTnode
{
    public:
        FieldMethod(FieldMethod,Field);
        FieldMethod(FieldMethod,Method);
        FieldMethod()
        Field getField();
        Method getMethod();
        FieldMethod getFieldMethod();

    private:
        Field field;
        Method method;
        FieldMethod filed_method ;
};
struct Field : ASTnode
{
    public:
        Field(String,Type);
        Field(String,Type,Expr);
        getID();
        getType();
        getExpr();

    private:
        String objID;
        Type type;
        Expr expr;
}