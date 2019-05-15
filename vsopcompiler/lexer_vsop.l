/* Lexer  */

/* reste à faire :
-strings ne fonctionnent pas bien, et j'arrive pas à faire fonctionner l'expression
\042({escape-char}|{regular-char})*\042
-error handling (j'ai juste testé une erreur avec integer literal)
-token value renvoyée en output pour les strings et integer literal 
(yytext renvoye un char*, faut transfo en int decimal quelque soit l'input format)
-sûrement d'autres subtilités, y'a plein de petit cas spécifiques.
*/

/*
    -Multi line comment OK
        -> Need to handle error now
*/

%{
/* need this for the call to atof() below */
#include <math.h>
#include <assert.h>
#include <stdio.h>

/**
 * \fn static char* modstr(char* str)
 * \brief Take a non NULL string and returns the string with all escaped char in \xhh format
 */
static char* modstr(char* str);

/**
 * \fn static int modinteger(char* integer)
 * \brief Take an integer in format 0b, 0x or in decimal format and returns it in decimal mode
 */
static int modinteger(char* integer);

int linepos = 0;
int columnpos = 0;
#define YY_DECL int yylex( filename ) char* filename;

static char* modstr(char* str){
    char* newString;
    const unsigned int LENGTH = strlen(str);
    
    newString = (char *) malloc(sizeof(char) * LENGTH);
    if (newString == NULL)
        return str;
        
    unsigned int i = 0, j = 0;
    
    for(; i < LENGTH; ++i){
        if (str[i] == '\\'){
            if(str[i+1] != '\n'){
                newString[j] = str[i];
                ++j;
                
                switch(str[++i]){
                    case 'b': newString[j] = 'x'; ++j; newString[j] = '0'; newString[++j] = '8'; j++; break;
                    
                    case 't': newString[j] = 'x'; ++j; newString[j] = '0'; newString[++j] = '9'; j++; break;
                    
                    case 'n': newString[j] = 'x'; ++j; newString[j] = '0'; newString[++j] = 'a'; j++; break;
                    
                    case 'r': newString[j] = 'x'; ++j; newString[j] = '0'; newString[++j] = 'd'; j++; break;
                    
                    default: newString[j] = str[i]; ++j; break;
                } //end switch 
            } else {
                //escape the 2 chars:\\lf 
                ++i;
            }
        } else {
            newString[j] = str[i];
            ++j;
        }
    }
    
    return newString;
}

static int modinteger(char* integer){
    assert(integer != NULL);
    
    if(strlen(integer) > 2 && integer[0] == '0'){
        switch(integer[1]){
            case 'x': return strtol(integer, NULL, 0);
            
            case 'b': return strtol(integer+2, NULL, 2);
        }
    }else{
        return atoi(integer);
    }
    
    return -1;
}

%}
%option noyywrap

/* regular definitions */
lowercaseletter             [a-z]
uppercaseletter             [A-Z]
letter                      ({lowercaseletter}|{uppercaseletter})
bin-digit                   "0"|"1"
digit                       ({bin-digit}|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9")
hex-digit                   ({digit}|[A-Fa-f])
unhex-letter                [G-Zg-z]
tab                         \011
lf                          \012
ff                          \014
cr                          \015
whitespace                  (" "|{tab}|{lf}|{ff}|{cr})
whitespacenolf              (" "|{tab}|{ff}|{cr})
base10number                {digit}({digit})*
base16number                "0""x"{hex-digit}({hex-digit})*
base2number                 "0""b"{bin-digit}({bin-digit})*
integer-literal             ({base10number}|{base16number}|{base2number})
letter_digit_underscore     ({letter}|{digit}|"_")
type-identifier             {uppercaseletter}({letter_digit_underscore})*
object-identifier           {lowercaseletter}({letter_digit_underscore})*
linecomment                 "//"(.)*("\n"|"EOF")
%x multilinecomment         
lfwhitespace                {lf}(" "|{tab})*
escape-sequence1            "b"|"t"|"n"|"r"|\042|\134
escape-sequence2            "x"{hex-digit}{hex-digit}
escape-sequence             ({escape-sequence1}|{escape-sequence2}|{lfwhitespace})
escape-char                 \134{escape-sequence}
incorrect-escape-seq        \134(.)*
regular-char                [^"\\\n\0EOF]        
string-literal              ["]({regular-char}|{escape-char})*["]
incorrect-str-lit           \042({regular-char}|{escape-char})*
incorrrectbackslash         \134{whitespacenolf}
invalid-char                (.)

%%

    int comment_nesting = 0;  /* Line 4 */

"and"               {fprintf(stdout,  "%d,%d,and\n" , columnpos,linepos);linepos += yyleng;}
"bool"              {fprintf(stdout,  "%d,%d,bool\n" , columnpos,linepos);linepos += yyleng;}
"class"             {fprintf(stdout,  "%d,%d,class\n" , columnpos,linepos);linepos += yyleng;}
"do"                {fprintf(stdout,  "%d,%d,do\n" , columnpos,linepos);linepos += yyleng;}
"else"              {fprintf(stdout,  "%d,%d,else\n" , columnpos,linepos);linepos += yyleng;}
"extends"           {fprintf(stdout,  "%d,%d,extends\n" , columnpos,linepos);linepos += yyleng;}
"false"             {fprintf(stdout,  "%d,%d,false\n" , columnpos,linepos);linepos += yyleng;}
"if"                {fprintf(stdout,  "%d,%d,if\n" , columnpos,linepos);linepos += yyleng;}
"in"                {fprintf(stdout,  "%d,%d,in\n" , columnpos,linepos);linepos += yyleng;}
"int32"             {fprintf(stdout,  "%d,%d,int32\n" , columnpos,linepos);linepos += yyleng;}
"isnull"            {fprintf(stdout,  "%d,%d,isnull\n" , columnpos,linepos);linepos += yyleng;}
"let"               {fprintf(stdout,  "%d,%d,let\n" , columnpos,linepos);linepos += yyleng;}
"new"               {fprintf(stdout,  "%d,%d,new\n" , columnpos,linepos);linepos += yyleng;}
"not"               {fprintf(stdout,  "%d,%d,not\n" , columnpos,linepos);linepos += yyleng;}
"string"            {fprintf(stdout,  "%d,%d,string\n" , columnpos,linepos);linepos += yyleng;}
"then"              {fprintf(stdout,  "%d,%d,then\n" , columnpos,linepos);linepos += yyleng;}
"true"              {fprintf(stdout,  "%d,%d,true\n" , columnpos,linepos);linepos += yyleng;}
"unit"              {fprintf(stdout,  "%d,%d,unit\n" , columnpos,linepos);linepos += yyleng;}
"while"             {fprintf(stdout,  "%d,%d,while\n" , columnpos,linepos);linepos += yyleng;}
{integer-literal}   {fprintf(stdout,  "%d,%d,integer-literal,%d\n" , columnpos,linepos,modinteger(yytext));linepos += yyleng;}
{type-identifier}   {fprintf(stdout,  "%d,%d,type-identifier,%s\n" , columnpos,linepos,yytext);linepos += yyleng;}
{object-identifier} {fprintf(stdout,  "%d,%d,object-identifier,%s\n" , columnpos,linepos,yytext);linepos += yyleng;}
{string-literal}    {fprintf(stdout,  "%d,%d,string-literal,%s\n" ,columnpos,linepos,modstr(yytext));
                     int i = 0;
                     while(yytext[i] != '\0'){
                        if(yytext[i] == '\n'){
                            columnpos++;
                            linepos=1;
                        } else {
                            linepos++;
                        }
                        ++i;
                     }           
                    }
"{"                 {fprintf(stdout,  "%d,%d,lbrace\n" , columnpos,linepos);linepos += yyleng;}
"}"                 {fprintf(stdout,  "%d,%d,rbrace\n" , columnpos,linepos);linepos += yyleng;}
"("                 {fprintf(stdout,  "%d,%d,lpar\n" , columnpos,linepos);linepos += yyleng;}
")"                 {fprintf(stdout,  "%d,%d,rpar\n" , columnpos,linepos);linepos += yyleng;}
":"                 {fprintf(stdout,  "%d,%d,colon\n" , columnpos,linepos);linepos += yyleng;}
";"                 {fprintf(stdout,  "%d,%d,semicolon\n" , columnpos,linepos);linepos += yyleng;}
","                 {fprintf(stdout,  "%d,%d,comma\n" , columnpos,linepos);linepos += yyleng;}
"+"                 {fprintf(stdout,  "%d,%d,plus\n" , columnpos,linepos);linepos += yyleng;}
"-"                 {fprintf(stdout,  "%d,%d,minus\n" , columnpos,linepos);linepos += yyleng;}
"*"                 {fprintf(stdout,  "%d,%d,times\n" , columnpos,linepos);linepos += yyleng;}
"/"                 {fprintf(stdout,  "%d,%d,div\n" , columnpos,linepos);linepos += yyleng;}
"^"                 {fprintf(stdout,  "%d,%d,pow\n" , columnpos,linepos);linepos += yyleng;}
"."                 {fprintf(stdout,  "%d,%d,dot\n" , columnpos,linepos);linepos += yyleng;}
"="                 {fprintf(stdout,  "%d,%d,equal\n" , columnpos,linepos);linepos += yyleng;}
"<"                 {fprintf(stdout,  "%d,%d,lower\n" , columnpos,linepos);linepos += yyleng;}
"<="                {fprintf(stdout,  "%d,%d,lowerequal\n" , columnpos,linepos);linepos += yyleng;}
"<-"                {fprintf(stdout,  "%d,%d,assign\n" , columnpos,linepos);linepos += yyleng;}
{lf}                {columnpos++;linepos=1;}
{whitespacenolf}    {linepos+=yyleng;}
{linecomment}       {columnpos++;linepos=1;}

"(*"             { /*fprintf(stdout,  "%d,%d,DUBUG_COMMENT\n" , columnpos,linepos)*/;BEGIN(multilinecomment); }
<multilinecomment>{
  "(*"           { ++comment_nesting; linepos += yyleng;}
  "*"+")"        { linepos += yyleng;
                   if (comment_nesting) --comment_nesting;
                   else BEGIN(INITIAL); }
  "*"+           {linepos += yyleng;} /* Match as many * as possible */
  <<EOF>>        {fprintf(stderr,  "%s,%d,%d,lexical error :unfinished comment:%s\n",filename, columnpos, linepos, yytext);}
  [^(*\n]+       {linepos += yyleng;} /* Match everything but (,* and \n */
  [(]            {linepos += yyleng;} /* Match ( */
  \n             {columnpos++;linepos=1;}
  
}

"*)"             {fprintf(stderr,  "%s,%d,%d,lexical error : unbegan comment:%s\n",filename, columnpos, linepos, yytext);}

{integer-literal}({type-identifier}|{object-identifier}) {fprintf(stderr,  "%s,%d,%d,lexical error :integer-literal not valid :%s\n",filename, columnpos, linepos, yytext);}
{incorrrectbackslash} {fprintf(stderr,  "%s,%d,%d,lexical error :incorrect character :%s\n",filename, columnpos, linepos, yytext);}
{incorrect-str-lit} {fprintf(stderr,  "%s,%d,%d,lexical error :string not terminated:%s\n",filename, columnpos, linepos, yytext);}
{incorrect-escape-seq} {fprintf(stderr,  "%s,%d,%d,lexical error :incorrect escape sequence:%s\n",filename, columnpos, linepos, yytext);}
{invalid-char} {fprintf(stderr,  "%s,%d,%d,lexical error :incorrect character:%s\n",filename, columnpos, linepos, yytext);}




%%



main( argc, argv )
int argc;
char **argv;
    {
    ++argv, --argc;  /* skip over program name */
    columnpos++;
    linepos++;
    if ( argc > 0 && !strncmp(argv[0], "-lex", 5))
            yyin = fopen( argv[1], "r" );
    else
            return -1;

    yylex(argv[0]);
    return 0;
    }




