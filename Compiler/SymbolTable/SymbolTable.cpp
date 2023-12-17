//
//  SymbolTable.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#include "SymbolTable.h"
#include "ctype.h"
#include <fstream>
#include <iostream>


void SymbolTable::init(unsigned long tableSize){
    size = tableSize;
    table = new EntryList[tableSize];
}


SymbolTable::SymbolTable(){
    init(DEFAULT_SIZE);
    foldCase = 0;
    numberEntries = 0;
    numberProbes = 0;
    numberHits = 0;
    maxSearchDist = 0;
}


SymbolTable::SymbolTable(bool foldCase){
    init(DEFAULT_SIZE);
    this->foldCase = foldCase;
    numberEntries = 0;
    numberProbes = 0;
    numberHits = 0;
    maxSearchDist = 0;
}


SymbolTable::SymbolTable(unsigned long size, bool foldCase){
    init(size);
    this->foldCase = foldCase;
    numberEntries = 0;
    numberProbes = 0;
    numberHits = 0;
    maxSearchDist = 0;
}


void SymbolTable::reset(unsigned long tableSize){
    clearSymbolTable();
    init(tableSize);
}


unsigned long SymbolTable::ElfHash( char * str ){
    unsigned long   h = 0, high; // h=0, g
    unsigned char *s = (unsigned char *) str;
    while (* s){
        h = ( h << 4 ) + *s++;  //1/4 of bits
        if ( (high = h & 0xF0000000) )
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h%size;
}


SymbolTableEntry * SymbolTable::putSymbol(string entryName,STE_ENTRY_TYPE type,J_TYPE jType,int constValue){
    
    char * newName = new char[ entryName.length()+1];
    if (foldCase)
        convertToLowerCase(newName,entryName);
    else strcpy(newName, entryName.c_str());
    
    unsigned long index = ElfHash(newName);
    SymbolTableEntry * st = table[index].addEntry(newName, type, jType,constValue);
    
    if (st)
        numberEntries++;
    
    numberProbes++;
    return st;
}


SymbolTableEntry * SymbolTable::getSymbol(string entryName){
    char * newName = new char[ entryName.length()+1];
    if (foldCase)
        convertToLowerCase(newName,entryName);
    else strcpy(newName, entryName.c_str());
    
    unsigned long index = ElfHash(newName);
    SymbolTableEntry * ste = table[index].findEntry(newName);
    numberProbes++;
    if (ste)
        numberHits++;
    
    return ste;
}



void  SymbolTable::findAndPrintEntry(string entryName, ofstream &fout){// may be find and print entry
    
    char * newName = new char[ entryName.length()+1];
    if (foldCase)
        convertToLowerCase(newName,entryName);
    else strcpy(newName, entryName.c_str());
    
    unsigned long index = ElfHash(newName);
    SymbolTableEntry * ste = table[index].findEntry(newName);
    numberProbes++;
    if (ste) {
        numberHits++;
        fout<<entryName+": Entry found = "+ ste->toString()+"\n"; // to be fixed , se
    }
    else
        fout<<entryName+": Entry not found \n"+entryName;
}



void SymbolTable::printAll(ofstream & fout){
    unsigned long i;
    for (i =0; i < size; i++){
        fout << "T[" << i << "]: " << table[i].count() << " ste:\t";
        table[i].printAll(fout);
    }
    
    printSymbolStats(fout);
}


void SymbolTable::printSymbolStats(ofstream & fout) {
    int maxLength = 0;
    for (int i = 0; i < size; i++)
        if (table[i].count() > maxLength)
            maxLength = table[i].count();
    
    maxSearchDist = maxLength;
    fout<<"============================ \n";
    fout<<"Number of entries:  "<<numberEntries<<"\n";
    fout<<"Number of probes:  "<< numberProbes<<" \n";
    fout<<"Number of hits: "<<numberHits<<"\n";
    fout<<"Number of max search distance:  "<<maxSearchDist<<" \n";
    fout<<"============================ \n";
}

void SymbolTable::convertToLowerCase(char * newName,string entryName){
    for (int i = 0; i < entryName.length(); i++) {
        newName[i] = tolower(entryName[i]);
    }
    newName[entryName.length()] = '\0';
}


void SymbolTable::clearSymbolTable(){
    unsigned long i;
    for (i =0; i < size; i++)
        table[i].clear();
    numberEntries = 0;
    delete [] table;
}


SymbolTable::~SymbolTable(){
    clearSymbolTable();
}


