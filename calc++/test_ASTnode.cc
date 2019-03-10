#include "ASTnode.hh"
#include <iostream>

//Unitary test.

using namespace std;
int main(){
    string s = string("yolo");
    Type test = Type(s);
    string a = test.getID();
    cout << a;
    a = "fafa";
    cout << test.getID();
    Block empty = Block();
    cout<<boolalpha;
    cout << empty.isEmpty();
}