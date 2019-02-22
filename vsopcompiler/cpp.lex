/*
* Compiler project 1 : Lexical Analyser
* Julien L'hoest and Cédric Schils
*/

%{
	#include <stdio.h>
	#include <string>
	#include <stdlib.h>
	#include <iostream>
  	using namespace std;
  	extern int yylex();
	const char coma = ',';
	unsigned line = 1;
	unsigned column = 1;
	unsigned temp_line;
	unsigned temp_column;
	string str;
	string fileName;

	void setFile(char* file){
		fileName = string(file);
	}

	void printToken(string token){
		cout << line << coma << column << coma << token << endl;
		column += yyleng;
	}

	void printToken(string type, string token){
		cout << line << coma << column << coma << type << coma << token << endl;
		column += yyleng;
	}

	void faultHandler(string error){
		cout << fileName << ":" << line << ":" <<column << ": lexical error: " << error << endl;
	}

%}

%option noyywrap
%option stack

/* regular definitions */
lowercase-letter 	[a-z]
uppercase-letter 	[A-Z]
letter 			{lowercase-letter}|{uppercase-letter}
bin-digit 		[0-1]
digit 			{bin-digit}|[2-9]
hex-digit 		{digit}|[a-fA-F]

whitespaces 	\n|\f|\r|\t|" "
whitespaces-custom \f|\r|\t|" "

comment-line 		"//"[^\n\r]*

integer-literal 	{digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

type-identifier 	{uppercase-letter}({letter}|{digit}|"_")*

object-identifier 	{lowercase-letter}({letter}|{digit}|"_")*

escape-sequence 	b|t|r|\"|\\|x{hex-digit}{2}
escaped-char 		\\{escape-sequence}

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

operators 			({lbrace}|{rbrace}|{lpar}|{rpar}|{colon}|{semicolon}|{comma}|{plus}|{minus}|{times}|{div}|{pow}|{dot}|{equal}|{lower}|{lower-equal}|{assign})

custom 				[^ \t\n\r\f\{\}\(\)\:;,+\-\*\/\^.=<"<=""<\-"]

%x l_comment b_comment str_lit

%%

\n			{line += 1;}
\r 			{column = 1;}
[ \t\f] 		{column += yyleng;}

"and"			printToken(yytext);
"bool"			printToken(yytext);
"class"			printToken(yytext);
"do" 			printToken(yytext);
"else"			printToken(yytext);
"extends"		printToken(yytext);
"false"			printToken(yytext);
"if"			printToken(yytext);
"in" 			printToken(yytext);
"int32"			printToken(yytext);
"isnull"		printToken(yytext);
"let" 			printToken(yytext);
"new" 			printToken(yytext);
"not" 			printToken(yytext);
"string" 		printToken(yytext);
"then" 			printToken(yytext);
"true" 			printToken(yytext);
"unit" 			printToken(yytext);
"while" 		printToken(yytext);

"{"  			printToken("lbrace");
"}"  			printToken("rbrace");
"("  			printToken("lpar");
")"  			printToken("rpar");
":"  			printToken("colon");
";"  			printToken("semicolon");
","  			printToken("coma");
"+"  			printToken("plus");
"-"  			printToken("minus");
"*"  			printToken("times");
"/"  			printToken("div");
"^"  			printToken("pow");
"."  			printToken("dot");
"="  			printToken("equal");
"<"  			printToken("lower");
"<="  			printToken("lower-equal");
"<-"  			printToken("assign");

{digit}+		printToken("integer-literal", yytext);
"0b"{bin-digit}+	{string buff = yytext; printToken("integer-literal", to_string(stoi(buff.erase(0, 2), nullptr, 2)));}
"0x"{hex-digit}+	printToken("integer-literal", to_string(stoi(yytext, nullptr, 0)));

{type-identifier}	printToken("type-identifier", yytext);
{object-identifier}	printToken("object-identifier", yytext);

{comment-line} 		{column += yyleng; yy_push_state(l_comment);}
<l_comment>[^\n\r]*\n 	{column += yyleng; line++; yy_pop_state();}
<l_comment>[^\n\r]*\r 	{column = 1; yy_push_state(l_comment); yy_pop_state();}
<l_comment><<EOF>> 	{yy_pop_state();}

\"							{str.clear(); str.append(yytext); temp_line = 0; temp_column = 1; yy_push_state(str_lit);}
<str_lit>{escaped-char} 	{str.append(yytext); temp_column += yyleng;}
<str_lit>\" 				{str.append(yytext); printToken("string-literal", str); column += --temp_column; line += temp_line; yy_pop_state();}
<str_lit>\r 				{column = 1; temp_column++;}
<str_lit>"\\"\n{whitespaces-custom}* {temp_column += yyleng-1; temp_line++;}
<str_lit>"\\"\r\n{whitespaces-custom}* {column = 1; temp_column = yyleng-2; temp_line++;}
<str_lit>[^\n\0\\]			{str.append(yytext); temp_column += yyleng;}
<str_lit>(\n|\0)  			{faultHandler(string(" \\n or \\0 in string")); yy_pop_state();}
<str_lit><<EOF>>			{faultHandler(string("non terminated string")); yy_pop_state();}


"(*"         {yy_push_state(b_comment);}

<b_comment>"(*"			{yy_push_state(b_comment);}
<b_comment>[^(*)\n]*      	{column += yyleng;}  /* eat anything that's not a '*' a '(' or a ')' */
<b_comment>"*"+[^(*)\n]*   	{column += yyleng;}/* eat up '*'s not followed by ')'s */
<b_comment>"("+[^(*\n]*		{column += yyleng;}
<b_comment>\n           	{line++;}
<b_comment><<EOF>>		{faultHandler(string("non terminated block comment")); yy_pop_state();}
<b_comment>"*)"        		{yy_pop_state();}

<<EOF>>				{return 0;}
.				{faultHandler(string("Invalid character: ").append(yytext));}
{integer-literal}{custom}* 	{faultHandler(string("Unrecognized token: ").append(yytext));}

%%
int main(int argc, char** argv) {

	FILE* f = NULL;
	if(argc == 3){
		setFile(argv[2]);	
		f = fopen(argv[2], "r");
 		if(!f) {
    			cerr << "Unable to open file specify in " << argv[2] << endl;
    			return -1; 	
		}
  		// set lex to read from it instead of defaulting to STDIN:
  		yyin = f;
  	}
  
  	// lex through the input:
	line = 1;
	column = 1;
  	while(yylex());
  	if(f){
 		fclose(f);
	}
  	return 0;

}
