//
//  SymbolTable.h
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#ifndef SymbolTable_h
#define SymbolTable_h
#include "EntryList.h"
#define DEFAULT_SIZE 19



class SymbolTable
{
    
private:
    
    unsigned long size;
    int fold_case;
    int number_entries; // Number of entries in table
    int number_probes; // Number of probes into table
    int number_hits; // Number of probes that immediately found entry
    int max_search_dist; // Maximum entries search
    EntryList *Table;   // Dynamic Array of size = Size
    void init(unsigned long tableSize);
    unsigned long ElfHash(char * str);
    
    
public:
    SymbolTable* next;
    SymbolTable();
    SymbolTable(int foldCase);
    SymbolTable(unsigned long tableSize, int foldCase);
    void reset(unsigned long tableSize);
    SymbolTableEntry*   putSymbol(string entryName, ste_entry_type Type, j_type jType = TYPE_NONE);
    SymbolTableEntry *  putSymbol(string entryName, ste_entry_type Type, int constValue = 0);
    SymbolTableEntry *  getSymbol(string entryName);
    void findAndPrintEntry(string entryName, ofstream &fout);//finds and prints the Entry if it exist
    void printAll(ofstream &fout);
    void print_symbol_stats(ofstream &fout);
    void clear_symbol_table();
    ~SymbolTable();
};



#endif /* SymbolTable_h */
