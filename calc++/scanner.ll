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

    void faultHandler(string error){
        cerr << "filename" << ":" << line << ":" <<column << ": lexical error: " << error << endl;
    }

    string escapedAscii(char c){
    switch (c) {
      case '\\':
        return "\\";
      case 'b':
        return "\b";
      case 't':
        return "\t";
      case 'n':
        return "\n";
      case 'r':
        return "\r";
      case '\"':
        return "\"";
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

// Old versions of Flex (2.5.35) generate an incomplete documentation comment.
//
//  In file included from src/scan-code-c.c:3:
//  src/scan-code.c:2198:21: error: empty paragraph passed to '@param' command
//        [-Werror,-Wdocumentation]
//   * @param line_number
//     ~~~~~~~~~~~~~~~~~^
//  1 error generated.
#if FLEX_VERSION < 206 && defined __clang__
# pragma clang diagnostic ignored "-Wdocumentation"
#endif

// Old versions of Flex (2.5.35) use 'register'.  Warnings introduced in
// GCC 7 and Clang 6.
#if FLEX_VERSION < 206
# if defined __clang__ && 6 <= __clang_major__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
# elif defined __GNUC__ && 7 <= __GNUC__
#  pragma GCC diagnostic ignored "-Wregister"
# endif
#endif

#if FLEX_VERSION < 206
# if defined __clang__
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wshorten-64-to-32"
#  pragma clang diagnostic ignored "-Wsign-conversion"
# elif defined __GNUC__
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
# endif
#endif
%}



%option noyywrap nounput noinput batch debug
%option stack 
%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}



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





"and"           { printToken(yytext); return yy::parser::make_AND(loc); }
"bool"          { printToken(yytext); return yy::parser::make_BOOL(loc); }
"class"         { printToken(yytext); return yy::parser::make_CLASS(loc); }
"do"            { printToken(yytext); return yy::parser::make_DO(loc); }
"else"          { printToken(yytext); return yy::parser::make_ELSE(loc); }
"extends"       { printToken(yytext); return yy::parser::make_EXTENDS(loc); }
"false"         { printToken(yytext); return yy::parser::make_FALSE(loc); }
"if"            { printToken(yytext); return yy::parser::make_IF(loc); }
"in"            { printToken(yytext); return yy::parser::make_IN(loc); }
"int32"         { printToken(yytext); return yy::parser::make_INT32(loc); }
"isnull"        { printToken(yytext); return yy::parser::make_ISNULL(loc); }
"let"           { printToken(yytext); return yy::parser::make_LET(loc); }
"new"           { printToken(yytext); return yy::parser::make_NEW(loc); }
"not"           { printToken(yytext); return yy::parser::make_NOT(loc); }
"string"        { printToken(yytext); return yy::parser::make_STRING(loc); }
"then"          { printToken(yytext); return yy::parser::make_THEN(loc); }
"true"          { printToken(yytext); return yy::parser::make_TRUE(loc); }
"unit"          { printToken(yytext); return yy::parser::make_UNIT(loc); }
"while"         { printToken(yytext); return yy::parser::make_WHILE(loc); }

"{"             { printToken("lbrace"); return yy::parser::make_LBRACE(loc); }
"}"             { printToken("rbrace"); return yy::parser::make_RBRACE(loc); }
"("             { printToken("lpar"); return yy::parser::make_LPAR(loc); }
")"             { printToken("rpar"); return yy::parser::make_RPAR(loc); }
":"             { printToken("colon"); return yy::parser::make_COLON(loc); }
";"             { printToken("semicolon"); return yy::parser::make_SEMICOLON(loc); }
","             { printToken("comma"); return yy::parser::make_COMMA(loc); }
"+"             { printToken("plus"); return yy::parser::make_PLUS(loc); }
"-"             { printToken("minus"); return yy::parser::make_MINUS(loc); }
"*"             { printToken("times"); return yy::parser::make_TIMES(loc); }
"/"             { printToken("div"); return yy::parser::make_DIV(loc); }
"^"             { printToken("pow"); return yy::parser::make_POW(loc); }
"."             { printToken("dot"); return yy::parser::make_DOT(loc); }
"="             { printToken("equal"); return yy::parser::make_EQUAL(loc); }
"<"             { printToken("lower"); return yy::parser::make_LOWER(loc); }
"<="            { printToken("lower-equal"); return yy::parser::make_LOWER_EQUAL(loc); }
"<-"            { printToken("assign"); return yy::parser::make_ASSIGN(loc); }

{digit}+            { printToken("INTEGER_LITERAL", to_string(stoi(yytext))); return yy::parser::make_INTEGER_LITERAL( stoi(yytext), loc); }
"0b"{bin-digit}+    { string buff = yytext; buff = to_string(stoi(buff.erase(0, 2), nullptr, 2)); printToken("INTEGER_LITERAL", buff); return yy::parser::make_INTEGER_LITERAL(stoi(buff), loc); }
"0x"{hex-digit}+    { string buff = to_string(stoi(yytext, nullptr, 0)); printToken("INTEGER_LITERAL", buff); return yy::parser::make_INTEGER_LITERAL(stoi(buff), loc); }

{TYPE_IDENTIFIER}   { printToken("TYPE_IDENTIFIER", yytext); return yy::parser::make_TYPE_IDENTIFIER(yytext, loc); }
{OBJECT_IDENTIFIER} { printToken("OBJECT_IDENTIFIER", yytext); return yy::parser::make_OBJECT_IDENTIFIER(yytext, loc); }

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
<str_lit>\"                 {str.append(yytext); printToken("string-literal", str); column = ++temp_column; line = temp_line; BEGIN(INITIAL); return yy::parser::make_STRING_LITERAL(yytext,loc);}
<str_lit>\r                 temp_column = 1;
<str_lit>"\\"\n[ \t]*       {temp_column = 1; temp_column += yyleng - 2; temp_line++;}
<str_lit>"\\"\r\n[ \t]*     {temp_column = 1; temp_column += yyleng - 3; temp_line++;}
<str_lit>[^\n\0\\]          {str.append(yytext); temp_column += yyleng;}
<str_lit>\0                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\000' is illegal in this context."); /*return -1;*/}
<str_lit>\n                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\n' is illegal in this context."); /*return -1;*/}
<str_lit>\\                 {column = --temp_column; line = temp_line; faultHandler(" lexical error\r\n  character '\\' is illegal in this context."); /*return -1;*/}
<str_lit>\\[^\"]            {faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); /*return -1;*/}
<str_lit>\\x[^\"]{2}        {faultHandler(string("lexical error\r\n  ") + string(yytext) + string(" is not a valid escape sequence.")); /*return -1;*/}
<str_lit><<EOF>>            {faultHandler("non terminated string"); /*return -1;*/}


"(*"                        {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}

<b_comment>"(*"             {s.push(pair<int,int> (line, column)); column += 2; yy_push_state(b_comment);}
<b_comment>[^(*\n]*         column += yyleng;  /* eat anything that's not composed of '*', '(' or a '\n' */
<b_comment>"*"+[^()\n]      column += yyleng;  /* eat up '*'s not followed by '(', ')' or '\n' */
<b_comment>"("+[^(*\n]      column += yyleng;  /* eat up '('s not followed by '(' or '*'  */
<b_comment>\n               column = 1; line++;
<b_comment><<EOF>>          {cerr << "fileName" << ":" << s.top().first << ":" << s.top().second << ": lexical error " << endl;
                             column += yyleng; /*return -1;*/}
<b_comment>"*)"             {s.pop(); column += 2; yy_pop_state();}

<<EOF>>                                return yy::parser::make_END (loc);     
.                                           {faultHandler(" lexical error\r\n  character '" + string(yytext) + "' is illegal in this context."); /*return -1;*/}
{INTEGER_LITERAL}{whitespaces-operator}*    {faultHandler((string(yytext) + string(" is not a valid integer literal."))); /*return -1;*/}

 
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
