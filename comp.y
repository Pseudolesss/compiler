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
/*
Ce ne sont pas des tokens
mais des non terminaux
%token class
%token class-body
%token field
%token method
%token type
%token formal
%token block
%token expr
%token args
%token literal
%token boolean-literal

%token NUMBER
%token PLUS MINUS TIMES DIVIDE
%token LEFT RIGHT
%token END

%start Input

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG */

%right ASSIGN
%left AND
%right NOT
%nonassoc EQUAL LOWER-EQUAL LOWER
%left PLUS MINUS
%left TIMES DIVIDE
%right ISNULL ?????
%right POW
%left DOT

%%
Input: 	/* epsilon */
		| Input Line
;
Line: 	END
		| Expression END { printf("Result: %f\n", $1); }
;

Expression:
	NUMBER { $$ = $1; }
	| Expression PLUS Expression { $$ = $1 + $3;}
	| Expression MINUS Expression { $$ = $1 - $3;}
	| Expression TIMES Expression { $$ = $1 * $3;}
	| Expression DIVIDE Expression { $$ = $1 / $3;}
	| MINUS Expression %prec NEG { $$ = -$2; }
	| LEFT Expression RIGHT { $$ = $2; }
;

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
expr:
	IF expr THEN expr
	| IF expr THEN expr ELSE expr
	| WHILE expr DO expr
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
arg:
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