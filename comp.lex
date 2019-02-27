%{
	#include <stdlib.h>
	#include <string>
	#define YYSTYPE double /* Define the main semantic type */
	#include "calc.tab.h" /* Define the token constants */

	#include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <stack>
    using namespace std;

    void yyerror(string s);

    stack<pair<int,int>> s;
    const char coma = ',';
    unsigned line = 1;
    unsigned column = 1;
    unsigned temp_line;
    unsigned temp_column;
    string str;

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

%option noyywrap stack yylineno

/* regular definitions */
lowercase-letter    [a-z]
uppercase-letter    [A-Z]
letter          {lowercase-letter}|{uppercase-letter}

bin-digit       [0-1]
digit           {bin-digit}|[2-9]
hex-digit       {digit}|[a-fA-F]

comment-line        "//"[^\n\r]*

integer-literal     {digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

type-identifier     {uppercase-letter}({letter}|{digit}|"_")*

object-identifier   {lowercase-letter}({letter}|{digit}|"_")*

escape-sequence     b|t|n|r|\"|\\|x{hex-digit}{2}
escaped-char        \\{escape-sequence}

lbrace              "{"
rbrace              "}"
lpar                "("
rpar                ")"
colon               ":"
semicolon           ";"
comma               ","
plus                "+"
minus               "-"
times               "*"
div                 "/"
pow                 "^"
dot                 "."
equal               "="
lower               "<"
lower-equal         "<="
assign              "<-"

whitespaces-operator    [^ \t\n\r\f\{\}\(\)\:;,+\-\*\/\^.=<"<=""<\-"]

%x l_comment b_comment str_lit

%%

{white} 	{}
{real} 		{ yylval = atof(yytext); return NUMBER; }
"+" 		{ return PLUS; }
"-"  		{ return MINUS; }
"*" 		{ return TIMES; }
"/" 		{ return DIVIDE; }
"(" 		{ return LEFT; }
")" 		{ return RIGHT; }
"\n" 		{ return END; }
. 			{ yyerror("Invalid token"); }

\n          {column = 1; line += 1;}
\r          column = 1;
\r\n        {column = 1; line += 1;}
[ \t\f]     column ++;

"and"           { printToken(yytext); return AND; }
"bool"          { printToken(yytext); return BOOL; }
"class"         { printToken(yytext); return CLASS; }
"do"            { printToken(yytext); return DO; }
"else"          { printToken(yytext); return ELSE; }
"extends"       { printToken(yytext); return EXTENDS; }
"false"         { printToken(yytext); return FALSE; }
"if"            { printToken(yytext); return IF; }
"in"            { printToken(yytext); return IN; }
"int32"         { printToken(yytext); return INT32; }
"isnull"        { printToken(yytext); return ISNULL; }
"let"           { printToken(yytext); return LET; }
"new"           { printToken(yytext); return NEW; }
"not"           { printToken(yytext); return NOT; }
"string"        { printToken(yytext); return STRING; }
"then"          { printToken(yytext); return THEN; }
"true"          { printToken(yytext); return TRUE; }
"unit"          { printToken(yytext); return UNIT; }
"while"         { printToken(yytext); return WHILE; }

"{"             { printToken("lbrace"); return LBRACE; }
"}"             { printToken("rbrace"); return RBRACE; }
"("             { printToken("lpar"); return LPAR; }
")"             { printToken("rpar"); return RPAR; }
":"             { printToken("colon"); return COLON; }
";"             { printToken("semicolon"); return SEMICOLON; }
","             { printToken("comma"); return COMMA; }
"+"             { printToken("plus"); return PLUS; }
"-"             { printToken("minus"); return MINUS; }
"*"             { printToken("times"); return TIMES; }
"/"             { printToken("div"); return DIV; }
"^"             { printToken("pow"); return POW; }
"."             { printToken("dot"); return DOT; }
"="             { printToken("equal"); return EQUAL; }
"<"             { printToken("lower"); return LOWER; }
"<="            { printToken("lower-equal"); return LOWER-EQUAL; }
"<-"            { printToken("assign"); return ASSIGN; }

{digit}+            { printToken("integer-literal", to_string(stoi(yytext))); return INTEGER-LITERAL; }
"0b"{bin-digit}+    { string buff = yytext; printToken("integer-literal", to_string(stoi(buff.erase(0, 2), nullptr, 2))); return INTEGER-LITERAL; }
"0x"{hex-digit}+    { printToken("integer-literal", to_string(stoi(yytext, nullptr, 0))); return INTEGER-LITERAL; }

{type-identifier}   { printToken("type-identifier", yytext); return TYPE-IDENTIFIER; }
{object-identifier} { printToken("object-identifier", yytext); return OBJECT-IDENTIFIER; }

{comment-line}          {column += yyleng; BEGIN(l_comment);}
<l_comment>[^\n\r]*\n   {column += yyleng; line++; column = 1; BEGIN(INITIAL);}
<l_comment>[^\n\r]*\r   column = 1; 
<l_comment><<EOF>>      BEGIN(INITIAL);

\"                          {str.clear(); str.append(yytext); temp_line = line; temp_column = column + 1; BEGIN(str_lit);}

<str_lit>\\x{hex-digit}{2}  {string tmp = string(yytext).substr(yyleng-2, 2);
                            if( 33 < stoi(tmp, nullptr, 16) && stoi(tmp, nullptr, 16) < 127 )
                                str.push_back((char)(int)strtol(tmp.c_str(), NULL, 16));
                            else
                                str.append(yytext);
                            temp_column += 4;}
<str_lit>{escaped-char}     {str.append(escapedAscii(string(yytext).back())); temp_column += 2;}
<str_lit>\"                 {str.append(yytext); printToken("string-literal", str); column = ++temp_column; line = temp_line; BEGIN(INITIAL);}
<str_lit>\r                 temp_column = 1;
<str_lit>"\\"\n[ \t]*       {temp_column = 1; temp_column += yyleng - 2; temp_line++;}
<str_lit>"\\"\r\n[ \t]*     {temp_column = 1; temp_column += yyleng - 3; temp_line++;}
<str_lit>[^\n\0\\]          {str.append(yytext); temp_column += yyleng;}
<str_lit>\0                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\000' is illegal in this context."); return -1;}
<str_lit>\n                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\n' is illegal in this context."); return -1;}
<str_lit>\\                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\' is illegal in this context."); return -1;}
<str_lit>\\[^\"]            {faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); return -1;}
<str_lit>\\x[^\"]{2}        {faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); return -1;}
<str_lit><<EOF>>            {faultHandler("non terminated string"); return -1;}


"(*"                        {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}

<b_comment>"(*"             {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}
<b_comment>[^(*\n]*         column += yyleng;  /* eat anything that's not composed of '*', '(' or a '\n' */
<b_comment>"*"+[^()\n]      column += yyleng;  /* eat up '*'s not followed by '(', ')' or '\n' */
<b_comment>"("+[^(*\n]      column += yyleng;  /* eat up '('s not followed by '(' or '*'  */
<b_comment>\n               column = 1; line++;
<b_comment><<EOF>>          {cerr << fileName << ":" << s.top().first << ":" << s.top().second << ": lexical error " << endl;
                             column += yyleng; return -1;}
<b_comment>"*)"             {s.pop(); column += 2; yy_pop_state();}

<<EOF>>                                     return 0;
.                                           {faultHandler(" lexical error\r\n  character '" + string(yytext) + "' is illegal in this context."); return -1;}
{integer-literal}{whitespaces-operator}*    {faultHandler((string(yytext) + string(" is not a valid integer literal."))); return -1;}