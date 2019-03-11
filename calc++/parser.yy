%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.3"
%defines

%define parse.assert

%code requires {
  # include <string>
  class driver;
}


%code {
# include "driver.hh"
# include "ASTnode.hh"
}

%union{
	struct ASTnode* astnode;
	Expr* expr;
	Type* type;
	Field* field;
	Formal* formal;
	Formalx* formalx;
	Formals* formals;
	Exprx* exprx;
	Block* block;
	Method* method;
	FieldMethod* fieldmethod;
	Body* body;
	Classe* classe;
	Classes* classes;
	Programm* programm;
	Dual* dual;
	Unary* unary;
	If* if;
	While* while;
	Let* let;
	Assign* assign;
	Not* not;
	And* and;
	Equal* equal;
	Lower* lower;
	LowerEqual* lowerequal;
	Plus* plus; 
	Minus* minus;
	Times* times;
	Div* div;
	Pow* pow ;
	Minus1* minus1 
	IsNull* isnull 
	Exprxx* exprxx 
	Args* args 
	Function* function 
	Dot* dot 
	New* new 
	ObjID* objid 
	Literal* literal 
	IntLit* intlit 
	StrLit* strlit 
	BoolLit* boolit 
	Lpar* lpar
	Rpar* rpar 
	Parenthese parenthese 
}
// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose


%define api.token.prefix {TOK_}

%token LBRACE
%token RBRACE
%token LPAR
%token RPAR
%token COLON
%token SEMICOLON
%token COMMA
%token ASSIGN
%token BOOL
%token CLASS
%token EXTENDS
%token FALSE
%token IF

%token INT32
%token ISNULL
%token LET
%token NEW
%token STRING
%token TRUE
%token UNIT
%token WHILE
%token <std::string> TYPE_IDENTIFIER
%token <std::string> OBJECT_IDENTIFIER
%token <int> INTEGER_LITERAL
%token <std::string> STRING_LITERAL

%start  program
%token END  0  "end of file"
%nonassoc IN
%nonassoc THEN DO
%nonassoc ELSE
%right ASSIGN
%left AND
%right NOT
%nonassoc EQUAL LOWER_EQUAL LOWER
%left PLUS MINUS
%left TIMES DIV
%right ISNULL 
%right POW
%left DOT

%printer { yyo << $$; } <*>;

%type <programm> program
%type <classes> classes 
%type <classe> class
%type <body> class_body



%%
program:
	classes {Programm* a = new Programm($1);drv.root=a; $$ = a;}
;
classes:
	classes class {$$ = Classes($1,$2);}
	| class		  {$$ = Classes($1);}
;
class:
	CLASS TYPE_IDENTIFIER class_body {new Classe($2,$3);}
	| CLASS TYPE_IDENTIFIER EXTENDS TYPE_IDENTIFIER class_body
;
class_body:
	LBRACE field-method RBRACE
;
field-method:
	/*espilon*/
	| field-method field
	| field-method method
;
field:
	OBJECT_IDENTIFIER COLON type SEMICOLON
	| OBJECT_IDENTIFIER COLON type ASSIGN expr SEMICOLON;
;
method:
	OBJECT_IDENTIFIER LPAR formals RPAR COLON type block
;
type:
	TYPE_IDENTIFIER
	| INT32
	| BOOL
	| STRING
	| UNIT
;
formals:
	/*epsilon*/
	| formal formalx
;
formalx:
	/*epsilon*/
	| COMMA formal formalx 
;
formal:
	OBJECT_IDENTIFIER COLON type
;
block:
	LBRACE expr exprx RBRACE
;
exprx:
	/*epsilon*/
	| SEMICOLON expr exprx
;
expr:
	IF expr THEN expr ELSE expr
	| IF expr THEN expr
	| WHILE expr DO expr
	| LET OBJECT_IDENTIFIER COLON type IN expr
	| LET OBJECT_IDENTIFIER COLON type ASSIGN expr IN expr
	| OBJECT_IDENTIFIER ASSIGN expr
	| NOT expr
	| expr AND expr
	| expr EQUAL expr
	| expr LOWER expr
	| expr LOWER_EQUAL expr
	| expr PLUS expr
	| expr MINUS expr
	| expr TIMES expr
	| expr DIV expr
	| expr POW expr
	| MINUS expr
	| ISNULL expr
	| OBJECT_IDENTIFIER LPAR args RPAR
	| expr DOT OBJECT_IDENTIFIER LPAR args RPAR
	| NEW TYPE_IDENTIFIER
	| OBJECT_IDENTIFIER
	| literal
	| LPAR RPAR
	| LPAR expr RPAR
	| block
;
args:
	/*epsilon*/
	| expr exprxx
;
exprxx:
	/*epsilon*/
	| COLON expr exprxx
;
literal:
	INTEGER_LITERAL
	| STRING_LITERAL
	| boolean-literal
;
boolean-literal:
	TRUE
	| FALSE
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
