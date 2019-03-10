
%skeleton "lalr1.cc" /* -*- C++ -*- */

%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hh"
}

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
%token TYPE_IDENTIFIER
%token OBJECT_IDENTIFIER
%token INTEGER_LITERAL
%token STRING_LITERAL

%start program

%nonassoc IN
%nonassoc THEN DO
%nonassoc ELSE
%right ASSIGN
%left AND
%right NOT
%nonassoc EQUAL LOWER-EQUAL LOWER
%left PLUS MINUS
%left TIMES DIV
%right ISNULL 
%right POW
%left DOT

%%
program:
	classes
;
classes:
	classes class
	| class
;
class:
	CLASS TYPE_IDENTIFIER class-body
	| CLASS TYPE_IDENTIFIER EXTENDS TYPE_IDENTIFIER class-body
;
class-body:
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
	| expr LOWER-EQUAL expr
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
void yyerror(char *s) {
	printf("%s on line %d - %s\n", s, yylineno, yytext);
}

int main(int argc, char **argv) {
	/* if any input file has been specified */
	if (argc >= 2) {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			fprintf(stderr, "Failed to open input file\n");
			return EXIT_FAILURE;
		}
	}
	if (yyparse() == 0) {
		fprintf(stdout, "Successful parsing\n");
	}
	fclose(yyin);
	fprintf(stdout, "End of processing\n");
	return EXIT_SUCCESS;
}




