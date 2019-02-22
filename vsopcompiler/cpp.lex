/*
* Compiler project 1 : Lexical Analyser
* Julien L'hoest and Cédric Schils
*/

%{
	#include <stdio.h>
	#include <string>
	#include <stdlib.h>
	#include <iostream>
	#include <stack>
  	using namespace std;
  	extern int yylex();
  	stack<pair<int,int>> s;
	
	const char coma = ',';
	unsigned line = 1;
	unsigned column = 1;
	unsigned temp_line;
	unsigned temp_column;
	string fileName;
	string str;

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
		cerr << fileName << ":" << line << ":" <<column << ": lexical error: " << error << endl;
	}

	string escapedAscii(char c){
	switch (c) {
	  case '\\':
	    return "\\\\";
	  case 'b':
	    return "\\x08";
      case 't':
	    return "\\x09";
      case 'n':
	    return "\\x0a";
      case 'r':
	    return "\\x0d";
      case '\"':
	    return "\\\"";
	  default:
	    return "\\x20";
	  }
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
whitespaces-custom \f|\t|" "

comment-line 		"//"[^\n\r]*

integer-literal 	{digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

type-identifier 	{uppercase-letter}({letter}|{digit}|"_")*

object-identifier 	{lowercase-letter}({letter}|{digit}|"_")*

escape-sequence 	b|t|n|r|\"|\\|x{hex-digit}{2}
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

\n			{column = 1; line += 1;}
\r 			column = 1;
\r\n 		{column = 1; line += 1;}
[ \t\f] 	column ++;

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
","  			printToken("comma");
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

{digit}+			printToken("integer-literal", to_string(stoi(yytext)));
"0b"{bin-digit}+	{string buff = yytext; printToken("integer-literal", to_string(stoi(buff.erase(0, 2), nullptr, 2)));}
"0x"{hex-digit}+	printToken("integer-literal", to_string(stoi(yytext, nullptr, 0)));

{type-identifier}	printToken("type-identifier", yytext);
{object-identifier}	printToken("object-identifier", yytext);

{comment-line} 		{column += yyleng; BEGIN(l_comment);}
<l_comment>[^\n\r]*\n 	{column += yyleng; line++; column = 1; BEGIN(INITIAL);}
<l_comment>[^\n\r]*\r 	column = 1; 
<l_comment><<EOF>> 	BEGIN(INITIAL);

\"							{str.clear(); str.append(yytext); temp_line = line; temp_column = column + 1; BEGIN(str_lit);}
<str_lit>\\x{hex-digit}{2}  {string tmp = string(yytext).substr(yyleng-2, 2);
							if( 33 < stoi(tmp, nullptr, 16) && stoi(tmp, nullptr, 16) < 127 )
								str.push_back((char)(int)strtol(tmp.c_str(), NULL, 16));
							else
								str.append(yytext);
							temp_column += 4;}
<str_lit>{escaped-char} 	{str.append(escapedAscii(string(yytext).back())); temp_column += 2;}
<str_lit>\" 				{str.append(yytext); printToken("string-literal", str); column = ++temp_column; line = temp_line; BEGIN(INITIAL);}
<str_lit>\r 				{temp_column = 1;}
<str_lit>"\\"\n{whitespaces-custom}* {temp_column = 1; temp_column += yyleng-2; temp_line++;}
<str_lit>"\\"\r\n{whitespaces-custom}* {temp_column = 1; temp_column += yyleng-3; temp_line++;}


<str_lit>[^\n\0\\]			{str.append(yytext); temp_column += yyleng;}
<str_lit>\0		  			{column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\000' is illegal in this context."); return -1;}
<str_lit>\n  				{column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\n' is illegal in this context."); return -1;}
<str_lit>\\					{column = --temp_column; line = temp_line; faultHandler(" Forbidden use of \\ in string literal"); return -1;}
<str_lit>\\[^\"]			{faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); return -1;}
<str_lit>\\x[^\"]{2}		{faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); return -1;}
<str_lit><<EOF>>			{faultHandler("non terminated string"); return -1;}


"(*"        				{s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}

<b_comment>"(*"				{s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}
<b_comment>[^(*\n]*      	column += yyleng;  /* eat anything that's not a '*' a '(' or a ')' */
<b_comment>"*"+[^*)\n]*   	column += yyleng; /* eat up '*'s not followed by ')'s */
<b_comment>"("+[^(*\n]*		column += yyleng;
<b_comment>\n           	column = 1; line++;
<b_comment><<EOF>>			{cerr << fileName << ":" << s.top().first << ":" << s.top().second << ": lexical error " << endl;
							 column += yyleng; return -1;}
<b_comment>"*)"        		{s.pop(); column += 2; yy_pop_state();}

<<EOF>>						return 0;
[^ \n\t\r\f]				{faultHandler(string("Invalid character: ")); return -1;}
{integer-literal}{custom}* 	{faultHandler((string(yytext) + string(" is not a valid integer literal."))); return -1;}


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
	int tmp = yylex();
  	if(f){
 		fclose(f);
	}
  	return tmp;

}
