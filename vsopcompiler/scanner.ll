%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.hh"
#include <stack>
using namespace std;

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

    void faultHandler(string fileName, string error){
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

/* regular definitions */

lowercase-letter    [a-z]
uppercase-letter    [A-Z]
letter          {lowercase-letter}|{uppercase-letter}

bin-digit       [0-1]
digit           {bin-digit}|[2-9]
hex-digit       {digit}|[a-fA-F]

comment-line        "//"[^\n\r]*

INTEGER_LITERAL     {digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

TYPE_IDENTIFIER     {uppercase-letter}({letter}|{digit}|"_")*

OBJECT_IDENTIFIER   {lowercase-letter}({letter}|{digit}|"_")*

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


%{
// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0.
#if defined __GNUC__ && !defined __clang__ && 6 <= __GNUC__
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// Of course, when compiling C as C++, expect warnings about NULL.
#if defined __clang__
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)

%}



%option noyywrap nounput noinput batch debug
%option stack 



%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

\n          {column = 1; line += 1;loc.lines (yyleng);}
\r          {column = 1;loc.step () ;}
\r\n        {column = 1; line += 1;loc.lines (yyleng);}
[ \t\f]     {column ++ ;loc.step ();} 





"and"           { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1) return yy::parser::make_AND(loc); }
"bool"          { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_BOOL(loc); }
"class"         { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_CLASS(loc); }
"do"            { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_DO(loc); }
"else"          { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_ELSE(loc); }
"extends"       { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_EXTENDS(loc); }
"false"         { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_FALSE(false, loc); }
"if"            { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_IF(loc); }
"in"            { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_IN(loc); }
"int32"         { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_INT32(loc); }
"isnull"        { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_ISNULL(loc); }
"let"           { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_LET(loc); }
"new"           { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_NEW(loc); }
"not"           { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_NOT(loc); }
"string"        { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_STRING(loc); }
"then"          { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_THEN(loc); }
"true"          { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_TRUE(true, loc); }
"unit"          { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_UNIT(loc); }
"while"         { if(drv.setting == 0) printToken(yytext); if(drv.setting == 1)  return yy::parser::make_WHILE(loc); }

"{"             { if(drv.setting == 0) printToken("lbrace"); if(drv.setting == 1)  return yy::parser::make_LBRACE(loc); }
"}"             { if(drv.setting == 0) printToken("rbrace"); if(drv.setting == 1)  return yy::parser::make_RBRACE(loc); }
"("             { if(drv.setting == 0) printToken("lpar"); if(drv.setting == 1)  return yy::parser::make_LPAR(loc); }
")"             { if(drv.setting == 0) printToken("rpar"); if(drv.setting == 1)  return yy::parser::make_RPAR(loc); }
":"             { if(drv.setting == 0) printToken("colon"); if(drv.setting == 1)  return yy::parser::make_COLON(loc); }
";"             { if(drv.setting == 0) printToken("semicolon"); if(drv.setting == 1)  return yy::parser::make_SEMICOLON(loc); }
","             { if(drv.setting == 0) printToken("comma"); if(drv.setting == 1)  return yy::parser::make_COMMA(loc); }
"+"             { if(drv.setting == 0) printToken("plus"); if(drv.setting == 1)  return yy::parser::make_PLUS(loc); }
"-"             { if(drv.setting == 0) printToken("minus"); if(drv.setting == 1)  return yy::parser::make_MINUS(loc); }
"*"             { if(drv.setting == 0) printToken("times"); if(drv.setting == 1)  return yy::parser::make_TIMES(loc); }
"/"             { if(drv.setting == 0) printToken("div"); if(drv.setting == 1)  return yy::parser::make_DIV(loc); }
"^"             { if(drv.setting == 0) printToken("pow"); if(drv.setting == 1)  return yy::parser::make_POW(loc); }
"."             { if(drv.setting == 0) printToken("dot"); if(drv.setting == 1)  return yy::parser::make_DOT(loc); }
"="             { if(drv.setting == 0) printToken("equal"); if(drv.setting == 1)  return yy::parser::make_EQUAL(loc); }
"<"             { if(drv.setting == 0) printToken("lower"); if(drv.setting == 1)  return yy::parser::make_LOWER(loc); }
"<="            { if(drv.setting == 0) printToken("lower-equal"); if(drv.setting == 1)  return yy::parser::make_LOWER_EQUAL(loc); }
"<-"            { if(drv.setting == 0) printToken("assign"); if(drv.setting == 1)  return yy::parser::make_ASSIGN(loc); }

{digit}+            { if(drv.setting == 0) printToken("integer-literal", to_string(stoi(yytext))); if(drv.setting == 1)  return yy::parser::make_INTEGER_LITERAL( stoi(yytext), loc); }
"0b"{bin-digit}+    { string buff = yytext; buff = to_string(stoi(buff.erase(0, 2), nullptr, 2)); if(drv.setting == 0) printToken("integer-literal", buff); if(drv.setting == 1)  return yy::parser::make_INTEGER_LITERAL(stoi(buff), loc); }
"0x"{hex-digit}+    { string buff = to_string(stoi(yytext, nullptr, 0)); if(drv.setting == 0) printToken("integer-literal", buff); if(drv.setting == 1)  return yy::parser::make_INTEGER_LITERAL(stoi(buff), loc); }

{TYPE_IDENTIFIER}   { if(drv.setting == 0) printToken("type-identifier", yytext); if(drv.setting == 1)  return yy::parser::make_TYPE_IDENTIFIER(yytext, loc); }
{OBJECT_IDENTIFIER} { if(drv.setting == 0) printToken("object-identifier", yytext); if(drv.setting == 1)  return yy::parser::make_OBJECT_IDENTIFIER(yytext, loc); }

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
<str_lit>\"                 {str.append(yytext);if(drv.setting == 0)  printToken("string-literal", str); column = ++temp_column; line = temp_line; BEGIN(INITIAL); if(drv.setting == 1)  return yy::parser::make_STRING_LITERAL(str, loc);}
<str_lit>\r                 temp_column = 1;
<str_lit>"\\"\n[ \t]*       {temp_column = 1; temp_column += yyleng - 2; temp_line++;}
<str_lit>"\\"\r\n[ \t]*     {temp_column = 1; temp_column += yyleng - 3; temp_line++;}
<str_lit>[^\n\0\\]          {str.append(yytext); temp_column += yyleng;}
<str_lit>\0                 {column = --temp_column; line = temp_line; faultHandler(drv.file, " lexical error\r\n  character '\\000' is illegal in this context."); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<str_lit>\n                 {column = --temp_column; line = temp_line; faultHandler(drv.file, " lexical error\r\n  character '\\n' is illegal in this context."); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<str_lit>\\                 {column = --temp_column ; line = temp_line; faultHandler(drv.file," lexical error\r\n  character '\\' is illegal in this context."); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<str_lit>\\[^\"]            {faultHandler(drv.file, string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<str_lit>\\x[^\"]{2}        {faultHandler(drv.file, string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<str_lit><<EOF>>            {faultHandler(drv.file,"non terminated string"); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}


"(*"                        {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}

<b_comment>"(*"             {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}
<b_comment>[^(*\n]*         column += yyleng;  /* eat anything that's not composed of '*', '(' or a '\n' */
<b_comment>"*"+[^()\n]      column += yyleng;  /* eat up '*'s not followed by '(', ')' or '\n' */
<b_comment>"("+[^(*\n]      column += yyleng;  /* eat up '('s not followed by '(' or '*'  */
<b_comment>\n               column = 1; line++;
<b_comment><<EOF>>          {cerr << drv.file << ":" << s.top().first << ":" << s.top().second << ": lexical error " << endl;
                             column += yyleng; if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
<b_comment>"*)"             {s.pop(); column += 2; yy_pop_state();}

<<EOF>>                                { if(drv.setting == 0) return yy::parser::make_SUCCEED(loc); return yy::parser::make_END(loc);}
.                                           {faultHandler(drv.file, " lexical error\r\n  character '" + string(yytext) + "' is illegal in this context."); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}
{INTEGER_LITERAL}{whitespaces-operator}*    {faultHandler(drv.file,(string(yytext) + string(" is not a valid integer literal."))); if(drv.setting == 0) return yy::parser::make_FAIL(loc);}

 
%%


void
driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void
driver::scan_end ()
{
  fclose (yyin);
}
