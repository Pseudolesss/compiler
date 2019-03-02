bison -v -d comp.y
flex -o comp.lex.c comp.lex
g++ -o test comp.lex.c comp.tab.c

