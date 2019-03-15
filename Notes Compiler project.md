# Notes Compiler project

- <u>Line, Column number in Flex:</u> Ambiguity with the behaviour \r\n \r and \n. Learn later the existence of yylineno for flex. According to some source yylineno is quite manual and can make the parser very slow in some situation.
- <u>Compilation problem (-lfl -ll):</u> Seems to be needed for subroutines to analyse several files at the same time. We have used the option % noyywrap for flex to be sure that it succeed to compile in c++. It seems to work compiling using gcc. The error returned is about the reference to the function yylex(). In the tutorial, it is referenced as **c++  name-mangling**.
- <u>Complement of regex:</u> Difficulties to announce the complement of a simple regex with is an "or" of fundamental ones. We have enumerate then all the char in bracket and complemented it. We should have arrange our rules to not have to do so.
- <u>Grammar recursion:</u> Space management in Bison seems to be more optimal with a left-recursive grammar. 
- <u>-lex argument:</u> Issues to launch the lexer independently because of tokens typed returns.