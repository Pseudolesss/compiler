%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define YYSTYPE double /* Main semantic type */
extern int yylex(void);
void yyerror(char *s);
extern char *yytext; /* Global variables of Flex */
extern int yylineno;
extern FILE *yyin;
%}

%token LBRACE
%token RBRACE
%token LPAR
%token RPAR
%token COLON
%token SEMICOLON
%token COMMA
%token PLUS
%token MINUS
%token TIMES
%token DIV
%token POW
%token DOT
%token EQUAL
%token LOWER
%token LOWER-EQUAL
%token ASSIGN

%token AND
%token BOOL
%token CLASS
%token DO
%token ELSE
%token EXTENDS
%token FALSE
%token IF
%token IN
%token INT32
%token ISNULL
%token LET
%token NEW
%token NOT
%token STRING
%token THEN
%token TRUE
%token UNIT
%token WHILE

%token TYPE-IDENTIFIER
%token OBJECT-IDENTIFIER
%token INTEGER-LITERAL
%token STRING-LITERAL

%start program

%nonassoc THEN DO
%nonassoc ELSE
%right ASSIGN
%left AND
%right NOT
%nonassoc EQUAL LOWER-EQUAL LOWER
%left PLUS MINUS
%left TIMES DIVIDE
%right ISNULL TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
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
	CLASS TYPE-IDENTIFIER class-body
	| CLASS TYPE-IDENTIFIER EXTENDS TYPE-IDENTIFIER class-body
;
class-body:
	LBRACE field-method RBRACE
;
field-method:
	/*espilon*/
	| field
	| method
;
field:
	OBJECT-IDENTIFIER COLON type SEMICOLON
	| OBJECT-IDENTIFIER COLON type ASSIGN expr SEMICOLON;
;
method:
	OBJECT-IDENTIFIER LPAR formals RPAR COLON type block
;
type:
	TYPE-IDENTIFIER
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
	OBJECT-IDENTIFIER COLON type
;
block:
	LBRACE expr exprx RBRACE
;
exprx:
	/*epsilon*/
	| SEMICOLON exprx
;
stmt:
	IF expr THEN stmt
	| IF expr stmt ELSE stmt
	| WHILE expr DO stmt
;
expr:
	stmt
	| LET OBJECT-IDENTIFIER COLON type IN expr
	| LET OBJECT-IDENTIFIER COLON type ASSIGN expr IN expr
	| OBJECT-IDENTIFIER ASSIGN expr
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
	| OBJECT-IDENTIFIER LPAR args RPAR
	| expr DOT OBJECT-IDENTIFIER LPAR args RPAR
	| NEW TYPE-IDENTIFIER
	| OBJECT-IDENTIFIER
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
	INTEGER-LITERAL
	| STRING-LITERAL
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