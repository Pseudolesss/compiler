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
	extern void setFile(string);
	
	const char coma = ',';
	unsigned line = 1;
	unsigned column = 1;
	unsigned temp_line;
	unsigned temp_column;
	bool str_error;
	string str;
	string filename;

	void setFile(string file){
		filename = file;
	}

	void printToken(string token){
		cout << line << coma << column << coma << token << endl;
		column += yyleng;
	}

	void printToken(string type, string token){
		cout << line << coma << column << coma << type << coma << token << endl;
		column += yyleng;
	}

	void faultHandler(){
		cout << "ERROR string literal" << endl;
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

comment-line 		"//"[^\n\r]*

integer-literal 	{digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

type-identifier 	{uppercase-letter}({letter}|{digit}|"_")*

object-identifier 	{lowercase-letter}({letter}|{digit}|"_")*

escape-sequence 	b|t|r|\"|\\|x{hex-digit}{2}
escaped-char 		\\{escape-sequence}
regular-char 		[^(\"|\n|\0|<<EOF>>)]
string-line 		(\\\n{whitespaces}*)
string-literal		("\""({regular-char}|{escaped-char}|{string-line})*"\"") 

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
div				"/"
pow				"^"
dot				"."
equal				"="
lower				"<"
lower-equal			"<="
assign				"<-"

operators 			({lbrace}|{rbrace}|{lpar}|{rpar}|{colon}|{semicolon}|{comma}|{plus}|{minus}|{times}|{div}|{pow}|{dot}|{equal}|{lower}|{lower-equal}|{assign})

custom 				[^\n|\f|\r|\t|" "[a-fA-F][0-9]]

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

{digit}+{whitespaces}		printToken("integer-literal", yytext);
"0b"{bin-digit}+{whitespaces}	{string buff = yytext; printToken("integer-literal", to_string(stoi(buff.erase(0, 2), nullptr, 2)));}
"0x"{hex-digit}+{whitespaces}	printToken("integer-literal", to_string(stoi(yytext, nullptr, 0)));

{type-identifier}	printToken("type-identifier", yytext);
{object-identifier}	printToken("object-identifier", yytext);

{comment-line} 		{column += yyleng; yy_push_state(l_comment);}
<l_comment>[^\n\r]*\n 	{column += yyleng; line++; yy_pop_state();}
<l_comment>[^\n\r]*\r 	{column = 1; yy_push_state(l_comment); yy_pop_state();}
<l_comment><<EOF>> 	{yy_pop_state();}

{string-literal}	{printToken("string-literal",strValue(yytext));}

"(*"         {cout << yytext; yy_push_state(b_comment);}

<b_comment>"(*"			{cout << yytext; yy_push_state(b_comment);}
<b_comment>[^(*)\n]*      	{column += yyleng; cout << yytext;}  /* eat anything that's not a '*' a '(' or a ')' */
<b_comment>"*"+[^(*)\n]*   	{column += yyleng; cout << yytext;}/* eat up '*'s not followed by ')'s */
<b_comment>"("+[^(*\n]*		{column += yyleng; cout << yytext;}
<b_comment>\n           	{line++; cout << endl;}
<b_comment><<EOF>>		{cerr << "Unrecognized token: " << line << coma << column << coma <<"'"<< yytext <<"'"<< endl; column += yyleng; return 0;}
<b_comment>"*)"        		{cout << yytext; yy_pop_state();}

<<EOF>>				{cout << "End of file dear" << endl; return 0;}
.				{cerr << filename << ":" << line << ":" << column << ":" << "lexical error, unknow char" << yytext << endl; column += yyleng;}
0x([^\n|\f|\r|\t|" "[0-9][a-fA-F]])* 	{cerr << filename << ":" << line << ":" << column << ":" <<"lexical error, invalid hex integer" << yytext << endl; column += yyleng;}
0b([^\n|\f|\r|\t|" "[0-1]])* {cerr << filename << ":" << line << ":" << column << ":" <<"lexical error, invalid bin integer" << yytext << endl; column += yyleng;}
{digit}*[^\n|\f|\r|\t|" "[0-9]] {cerr << filename << ":" << line << ":" << column << ":" <<"lexical error, invalid decimal integer" << yytext << endl; column += yyleng;}

%%
int main(int argc, char** argv) {

	if(argv != NULL) {
   		cerr << "No file path specified" << endl;
   		//return -1;
	}

	if(argc < 1){
		cerr << "No file path specified" << endl;
		//return -1;
 	}

	FILE *f = fopen(argv[1], "r");
 	if(!f) {
    	cerr << "Unable to open file specify in " << argv[1] << endl;
    return -1;
  }
  
  // set lex to read from it instead of defaulting to STDIN:
  yyin = f;

  setFile(argv[1]);
  // lex through the input:
  while(yylex());
  
  fclose(f);

  return 0;

}
