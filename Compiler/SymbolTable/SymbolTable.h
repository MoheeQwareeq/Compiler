//
//  SymbolTable.h
//  Compiler
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
    bool foldCase;
    int numberEntries; // Number of entries in table
    int numberProbes; // Number of probes into table
    int numberHits; // Number of probes that immediately found entry
    int maxSearchDist; // Maximum entries search
    EntryList * table;   // Dynamic Array of size = Size
    void init(unsigned long );
    unsigned long ElfHash(char *);
    void convertToLowerCase(char * ,string);

    
public:
    SymbolTable * next;
    SymbolTable();
    SymbolTable(bool);
    SymbolTable(unsigned long , bool);
    void reset(unsigned long );
    SymbolTableEntry * putSymbol(string entryName, STE_ENTRY_TYPE Type, J_TYPE = TYPE_NONE ,int = 0);
    SymbolTableEntry * getSymbol(string );
    void findAndPrintEntry(string,ofstream &);//finds and prints the Entry if it exist
    void printAll(ofstream &);
    void printSymbolStats(ofstream &);
    void clearSymbolTable();
    ~SymbolTable();
};



#endif /* SymbolTable_h */
