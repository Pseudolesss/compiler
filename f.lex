/*
* Compiler project 1 : Lexical Analyser
* Julien L'hoest and Cédric Schils
*/

%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	
	unsigned line = 1;
	unsigned column = 1;
	char buffer[1000];

	void bin2Decimal(char* bin_digit,char* buffer){
		char** p;
		unsigned long number = 0;
		number = strtoul(&bin_digit[2],p,2);
		sprintf(buffer, "%lu", number);
		return;
	}
	void hex2Decimal(char* hex_digit,char* buffer){
		char** p;
		unsigned long number = 0;
		number = strtoul(&hex_digit[2],p,16);
		sprintf(buffer, "%lu", number);
		return;
	}
	void update_clm_line(char* str,unsigned* column,unsigned* line){
		unsigned i = 0;
		while(str[i] != '\0'){
			*column++;
			if(str[i] == '\n')
				*line++;
			if(str[i] == '\r')
				*column = 1;
		}
	}
	char* strValue(char* str){
		unsigned length = strlen(str);
		char* value = NULL;
		unsigned i =0;
		value = (char*) malloc(2*sizeof(char)*length);
		int j = 0;
		while(str[i] != '\0'){
			if(str[i] == '\\'){
				i++;
				if(str[i] == 'n'){
					value[j] = '\\';
					value[j+1] = 'x';
					value[j+2] = '0';
					value[j+3] = 'a';
					j += 3;
				}
				if(str[i] == 'r'){
					value[j] = '\\';
					value[j+1] = 'x';
					value[j+2] = '0';
					value[j+3] = 'd';
					j += 3;
				}
				if(str[i] == 't'){
					value[j] = '\\';
					value[j+1] = 'x';
					value[j+2] = '0';
					value[j+3] = '8';
					j += 3;
				}
				if(str[i] == '\\'){
					value[j] = '\\';
				}
				else{
					value[j] = str[i];
				}		
			}
			else{
				value[j] = str[i];
			}
			i++;
			j++;
		}
		value[j] = '\0';
		return value;
	}
%}

/* regular definitions */
lowercase-letter 	[a-z]
uppercase-letter 	[A-Z]
letter 			{lowercase-letter}|{uppercase-letter}
bin-digit 		0|1
digit 			({bin-digit}|[2-9])
hex-digit 		({digit}|[a-fA-F])
tab 			"\t"
lf 			"\n"
ff 			"\f"  
cr 			"\r" 
whitespace 		" "|tab|lf|ff|cr 	

comment-line 		("//"(.)*) 
block-comment		("(*"(.|{whitespace})*"*)")

integer-literal 	{digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

type-identifier 	{uppercase-letter}({letter}|{digit}|"_")*

object-identifier 	{lowercase-letter}({letter}|{digit}|"_")*

escape-sequence 	"b"|"t"|"n"|"r"|'"'|"\"|"x"{hex-digit}	//|lf(" "|{tab})*
escaped-char 		"\"{escape-sequence}
regular-char 		^[\n\r{escaped-char}]
string-literal		("\""({regular-char}|{escaped-char})*"\"") 

lbrace				"{"
rbrace				"}"
lpar				"("
rpar				")"
colon				":"
semicolon			";"
comma				","
plus				"+"
minus				"-"
times				"*"
div					"/"
pow					"^"
dot					"."
equal				"="
lower				"<"
lower-equal			"<="
assign				"<-"

%%

lf			{line += 1;}
rc 			{column = 1;}
tab			{column += yyleng;}
whitespace 	{;}

"and"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"bool"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"class"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"do" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"else"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"extends"		{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"false"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"if"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"in" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"int32"			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"isnull"		{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"let" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"new" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"not" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"string" 		{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"then" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"true" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"unit" 			{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}
"while" 		{printf("%d,%d,%s\n", line, column, yytext); column += yyleng;}

"{"  			{printf("%d,%d,lbrace\n", line, column); column += yyleng;}
"}"  			{printf("%d,%d,rbrace\n", line, column); column += yyleng;}
"("  			{printf("%d,%d,lpar\n", line, column); column += yyleng;}
")"  			{printf("%d,%d,rpar\n", line, column); column += yyleng;}
":"  			{printf("%d,%d,colon\n", line, column); column += yyleng;}
";"  			{printf("%d,%d,semicolon\n", line, column); column += yyleng;}
","  			{printf("%d,%d,coma\n", line, column); column += yyleng;}
"+"  			{printf("%d,%d,plus\n", line, column); column += yyleng;}
"-"  			{printf("%d,%d,minus\n", line, column); column += yyleng;}
"*"  			{printf("%d,%d,times\n", line, column); column += yyleng;}
"/"  			{printf("%d,%d,div\n", line, column); column += yyleng;}
"^"  			{printf("%d,%d,pow\n", line, column); column += yyleng;}
"."  			{printf("%d,%d,dot\n", line, column); column += yyleng;}
"="  			{printf("%d,%d,equal\n", line, column); column += yyleng;}
"<"  			{printf("%d,%d,lower\n", line, column); column += yyleng;}
"<="  			{printf("%d,%d,lower-equal\n", line, column); column += yyleng;}
"<-"  			{printf("%d,%d,assign\n", line, column); column += yyleng;}

{digit}+		{printf("%d,%d,integer-literal,%s\n", line, column, yytext); column += yyleng;}
"0b"{bin-digit}+	{bin2Decimal(yytext,buffer); printf("%d,%d,integer-literal,%s\n", line, column, buffer); column += yyleng;}
"0x"{hex-digit}+	{hex2Decimal(yytext,buffer); printf("%d,%d,integer-literal,%s\n", line, column,buffer); column += yyleng;}

{type-identifier}	{printf("%d,%d,type-identifier,%s\n", line, column, yytext); column += yyleng;}
{object-identifier}	{printf("%d,%d,type-identifier,%s\n", line, column, yytext); column += yyleng;}

{comment-line} 		{column += yyleng;}
{block-comment}		{update_clm_line(yytext,&column,&line);}
("\""(.)*"\"")	{char* val = strValue(yytext) ;printf("%d,%d,string-literal,%s\n", line, column,val);free(val);column += yyleng;}
{integer-literal}{custom}* {cerr << "Unrecognized token: " << line << coma << column << coma <<"'"<< yytext <<"'"<< endl; column += yyleng;}

%%
int main (void) {
   yylex ();
   return 0;
   }
