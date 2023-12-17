//
//  SymbolTableList.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 12/08/2023.
//

#ifndef SymbolTableList_h
#define SymbolTableList_h

#include "SymbolTable.h"

class SymbolTableList{
private:
    ofstream fout;
    SymbolTable * head;
    int count;
    
public:
    SymbolTableList();
    SymbolTableList(bool);
    void enterScope();
    void exitScope();
    void printTable();
    SymbolTable* getHead();
    SymbolTableEntry * getMySymbol(string);
    ~SymbolTableList();

};





#endif /* SymbolTableList_h */
