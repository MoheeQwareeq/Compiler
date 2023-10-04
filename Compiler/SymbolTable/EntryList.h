//
//  EntryList.h
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#ifndef EntryList_h
#define EntryList_h
#include "SymbolTableEntry.h"

class EntryList
{
private:
    int counter;
    SymbolTableEntry *head;
public:
    
    EntryList();
    SymbolTableEntry * findEntry(string entryName);
    SymbolTableEntry * addEntry(string entryName, ste_entry_type Type, j_type jType = TYPE_NONE);
    SymbolTableEntry * addEntry(string entryName, ste_entry_type Type, int constValue = 0);
    void printAll(ofstream &fout);
    int count();
    void clear();
    ~EntryList();
};

#endif /* EntryList_h */
