//
//  EntryList.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#ifndef EntryList_h
#define EntryList_h
#include "SymbolTableEntry.h"

class EntryList{
private:
    
    int counter;
    SymbolTableEntry * head;
    
public:
    
    EntryList();
    SymbolTableEntry * findEntry(string);
    SymbolTableEntry * addEntry(string, STE_ENTRY_TYPE,J_TYPE = TYPE_NONE,int = 0);
    void printAll(ofstream &);
    int count();
    void clear();
    ~EntryList();
    
};

#endif /* EntryList_h */
