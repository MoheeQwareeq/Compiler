//
//  SymbolTable.cpp
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#include "SymbolTable.h"
#include "ctype.h"
#include <fstream>
#include <iostream>


void SymbolTable::init(unsigned long tableSize)
{
    size = tableSize;
    Table = new EntryList[tableSize];
    
}


SymbolTable::SymbolTable()
{
    init(DEFAULT_SIZE);
    fold_case = 0;
    number_entries = 0;
    number_probes = 0;
    number_hits = 0;
    max_search_dist = 0;
}


SymbolTable::SymbolTable(int foldCase)
{
    init(DEFAULT_SIZE);
    fold_case = foldCase;
    number_entries = 0;
    number_probes = 0;
    number_hits = 0;
    max_search_dist = 0;
}


SymbolTable::SymbolTable(unsigned long size, int foldCase)
{
    init(size);
    fold_case = foldCase;
    number_entries = 0;
    number_probes = 0;
    number_hits = 0;
    max_search_dist = 0;
}



void SymbolTable::reset(unsigned long tableSize)
{
    clear_symbol_table();
    init(tableSize);
}


unsigned long SymbolTable::ElfHash( char *str )
{
    unsigned long   h = 0, high; // h=0, g
    unsigned char *s = (unsigned char *) str;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;  //1/4 of bits
        if ( (high = h & 0xF0000000) )
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h%size;
}


SymbolTableEntry * SymbolTable::putSymbol(string entryName, ste_entry_type Type, int constValue)
{
    
    char* newName = new char[ entryName.length()+1];
    if (fold_case) {
        for (int i = 0; i < entryName.length(); i++) {
            newName[i] = tolower(entryName[i]);
        }
        newName[entryName.length()] = '\0';
    }
    else strcpy(newName, entryName.c_str());
    
    unsigned long index = ElfHash(newName);
    SymbolTableEntry* st = Table[index].addEntry(newName, Type, constValue);
    if (st) {
        number_entries++;
    }
    number_probes++;
    return st;
}


SymbolTableEntry* SymbolTable::putSymbol(string entryName, ste_entry_type Type, j_type jType)
{
    char* newName = new char[entryName.length() + 1];
    if (fold_case) {
        for (int i = 0; i < entryName.length(); i++) {
            newName[i] = tolower(entryName[i]);
        }
        newName[entryName.length()] = '\0';
    }
    else strcpy(newName, entryName.c_str());
    
    unsigned long index = ElfHash(newName);
    SymbolTableEntry* st = Table[index].addEntry(newName, Type, jType);
    if (st) {
        number_entries++;
    }
    number_probes++;
    return st;
}


SymbolTableEntry * SymbolTable::getSymbol(string entryName){
    char* newName = new char[entryName.length()+1];
    if (fold_case) {
        for (int i = 0; i < entryName.length(); i++) {
            newName[i] = tolower(entryName[i]);
        }
        newName[entryName.length()] = '\0';
    }
    else strcpy(newName, entryName.c_str());
    unsigned long index = ElfHash(newName);
    SymbolTableEntry *ste = Table[index].findEntry(newName);
    number_probes++;
    if (ste) {
        number_hits++;
        return ste;
    }
    return ste;
}



void  SymbolTable::findAndPrintEntry(string entryName, ofstream &fout)// may be find and print entry
{
    
    char* newName = new char[ entryName.length()+1];
    if (fold_case) {
        for (int i = 0; i < entryName.length(); i++) {
            newName[i] = tolower(entryName[i]);
        }
        newName[entryName.length()] = '\0';
    }
    else strcpy(newName, entryName.c_str());
    unsigned long index = ElfHash(newName);
    SymbolTableEntry *ste = Table[index].findEntry(newName);
    number_probes++;
    if (ste) {
        number_hits++;
        fout<<entryName+": Entry found = "+ ste->toString()+"\n"; // to be fixed , se
    }
    else fout<<entryName+": Entry not found \n"+entryName;
    
}



void SymbolTable::printAll(ofstream &fout)
{
    unsigned long i;
    for (i =0; i < size; i++)
    {
        fout << "T[" << i << "]: " << Table[i].count() << " ste:\t";
        cout << "T[" << i << "]: " << Table[i].count() << " ste:\t";
        Table[i].printAll(fout);
        cout << endl;
    }
    
}




void SymbolTable::print_symbol_stats(ofstream & fout) {
    int maxLength = 0;
    for (int i = 0; i < size; i++) {
        if (Table[i].count() > maxLength) {
            maxLength = Table[i].count();
        }
    }
    max_search_dist = maxLength;
    fout<<"============================ \n";
    fout<<"Number of entries:  "<<number_entries<<"\n";
    fout<<"Number of probes:  "<< number_probes<<" \n";
    fout<<"Number of hits: "<<number_hits<<"\n";
    fout<<"Number of max search distance:  "<<max_search_dist<<" \n";
}




void SymbolTable::clear_symbol_table()
{
    unsigned long i;
    for (i =0; i < size; i++)
    {
        Table[i].clear();
    }
    number_entries = 0;
    delete[]Table;
    
}


SymbolTable::~SymbolTable()
{
    clear_symbol_table();
}


