%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.3"
%defines

%define parse.assert

%code requires {
  # include <string>
  # include "ASTnode.hh"
  class driver;
}
%param { driver& drv }
%locations
%define parse.trace
%define parse.error verbose

%code {
# include "driver.hh"
}

%define api.token.constructor
%define api.value.type variant

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

//%printer { yyo << $$; } <*>;

%type <Programm*> program
%type <Classes*> classes
%type <Classe*> class
%type <Body*> class_body
%type <FieldMethod*> field-method
%type <Field*> field
%type <Method*> method
%type <Type*> type
%type <Expr*> expr
%type <Formals*> formals
%type <Block*> block
%type <Formalx*> formalx
%type <Formal*> formal
%type <Exprx*> exprx
%type <Exprxx*> exprxx
%%
program:
	classes {$$ = new Programm($1);}
;
classes:
	classes class  {$$ = new Classes($1,$2);}
	| class		  {$$ = new Classes($1);}
;
class:
	CLASS TYPE_IDENTIFIER class_body  {$$ = new Classe($2,$3);}
	| CLASS TYPE_IDENTIFIER EXTENDS TYPE_IDENTIFIER class_body {$$ = new Classe($2,$4,$5);}
;
class_body:
	LBRACE field-method RBRACE {$$ = new Body($2);}
;
field-method:
	/*espilon*/
	| field-method field {$$ = new FieldMethod($1,$2);}
	| field-method method {$$ = new FieldMethod($1,$2);}
;
field:
	OBJECT_IDENTIFIER COLON type SEMICOLON {$$ = new Field($1,$3);}
	| OBJECT_IDENTIFIER COLON type ASSIGN expr SEMICOLON {$$ = new Field($1,$3,$5);}
;
method:
	OBJECT_IDENTIFIER LPAR formals RPAR COLON type block {$$ = new Method($1,$3,$6,$7);}
;
type:
	TYPE_IDENTIFIER  {$$ = new Type($1);}
	| INT32			 {$$ = new Type(std::string("INT32"));}
	| BOOL			{$$ = new Type(std::string("BOOL"));}
	| STRING		{$$ = new Type(std::string("STRING"));}
	| UNIT			{$$ = new Type(std::string("UNIT"));}
;
formals:
	/*epsilon*/
	| formal formalx {$$ = new Formals($1,$2);}
;
formalx:
	/*epsilon*/
	| COMMA formal formalx  {$$ = new Formalx($2,$3);}
;
formal:
	OBJECT_IDENTIFIER COLON type {$$ = new Formal($1,$3);}
;
block:
	LBRACE expr exprx RBRACE {$$ = new Block($2,$3);}
;
exprx:
	/*epsilon*/
	| SEMICOLON expr exprx {$$ = new Exprx($2,$3);}
;
expr:
	IF expr THEN expr ELSE expr {$$ = new If($2,$4,$6);}
	| IF expr THEN expr			{$$ = new If($2,$4);}
	| WHILE expr DO expr		{$$ = new While($2,$4);}
	| LET OBJECT_IDENTIFIER COLON type IN expr	{$$ = new Let($2,$4,$6);}
	| LET OBJECT_IDENTIFIER COLON type ASSIGN expr IN expr {$$ = new Let($2,$4,$6,$8);}
	| OBJECT_IDENTIFIER ASSIGN expr		{$$ = new Assign($1,$3);}
	| NOT expr							{$$ = new Not($2);}
	| expr AND expr						{$$ = new And($1,$3);}
	| expr EQUAL expr					{$$ = new Equal($1,$3);}
	| expr LOWER expr					{$$ = new Lower($1,$3);}
	| expr LOWER_EQUAL expr				{$$ = new LowerEqual($1,$3);}
	| expr PLUS expr					{$$ = new Plus($1,$3);}
	| expr MINUS expr					{$$ = new Minus($1,$3);}
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
