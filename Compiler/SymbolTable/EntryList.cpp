//
//  EntryList.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//
#include "EntryList.h"

EntryList::EntryList(){
    counter = 0;
    head = nullptr;
}


SymbolTableEntry* EntryList::findEntry(string entryName){
    SymbolTableEntry * ste = head;
    while (ste){
        if( entryName == ste->name)
            return ste;
        ste = ste->next;
    }
    return nullptr;
}


SymbolTableEntry* EntryList::addEntry(string entryName,STE_ENTRY_TYPE steType,J_TYPE jType,int constValue){
    SymbolTableEntry * ste = findEntry(entryName);
    if(ste)
        return nullptr;
    
    else{
        ste = new SymbolTableEntry(entryName, steType,jType,constValue);
        ste->next = head;
        head = ste;
        counter++;
    }
    return ste;
}



void EntryList::printAll(ofstream & fout){
    SymbolTableEntry * ste = head;
    while (ste){
        ste->print(fout);
        ste = ste->next;
    }
    fout<<"\n";
}

int EntryList::count(){
    return counter;
}

void EntryList::clear(){
    SymbolTableEntry * ste;
    while (head){
        ste = head;
        head = head->next;
        delete ste;
    }
    counter = 0;
}


EntryList::~EntryList(){
    clear();
}


