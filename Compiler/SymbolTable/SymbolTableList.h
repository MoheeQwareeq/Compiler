//
//  SymbolTableList.h
//  Parser
//
//  Created by MOHEE QWAREEQ on 12/08/2023.
//

#ifndef SymbolTableList_h
#define SymbolTableList_h

#include "SymbolTable.h"

class SymbolTableList{
private:
    SymbolTable* head;
    int count;
    
public:
    SymbolTableList();
    SymbolTableList(bool foldCase);
    void enterScope();
    void exitScope();
    void printTable();
    SymbolTable* getHead();
    SymbolTableEntry* getMySymbol(string name);
    
};





#endif /* SymbolTableList_h */
