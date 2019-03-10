bison -v -o parser.cc parser.yy
g++ -c -o comp.o comp.cc
g++ -c -o driver.o driver.cc
g++ -c -o parser.o parser.cc
flex -o scanner.cc scanner.ll
g++ -c -o scanner.o scanner.cc
g++ -o test comp.o driver.o parser.o scanner.o


