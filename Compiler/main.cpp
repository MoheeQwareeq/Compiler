//
//  main.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 26/08/2023.
//


#include "Parser.h"


int main() {
    string fileName = "program.COMP23";
    FileDescriptor fileDescriptor(fileName);
    SymbolTableList global(false); //fold case is false
    Parser  parser(fileDescriptor,global);
    parser.print(parser.parse());
    cout<<"Your code has been successfully compiled without any errors"<<endl;
    cout<<endl;
}


