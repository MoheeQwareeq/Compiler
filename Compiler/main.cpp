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
    SymbolTableList * global=new SymbolTableList(false); //fold case is true
    Parser  parser(&fileDescriptor,global);
    ast_list * s=parser.parse();
    parser.print(s);
    cout<<"Your code has been successfully compiled without any errors"<<endl;
    cout<<endl;
}


