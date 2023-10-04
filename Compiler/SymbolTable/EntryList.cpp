//
//  EntryList.cpp
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//
#include "EntryList.h"

EntryList::EntryList()
{
    counter = 0;
    head = nullptr;
}




SymbolTableEntry* EntryList::findEntry(string entryName)
{
    SymbolTableEntry *ste = head;
    while (ste != nullptr)
    {
        if( entryName==ste->name) return ste;
        ste = ste->next;
    }
    return nullptr;
    
}



SymbolTableEntry* EntryList::addEntry(string entryName, ste_entry_type Type, j_type jType)
{
    SymbolTableEntry *ste = findEntry(entryName);
    
    if(ste)
    {
        return nullptr;
    }
    else
    {
        ste = new SymbolTableEntry(entryName, Type, jType);
        ste->next = head;
        head = ste;
        
        counter++;
    }
    return ste;
}

SymbolTableEntry* EntryList::addEntry(string entryName, ste_entry_type Type, int constValue)
{
    SymbolTableEntry* ste = findEntry(entryName);
    
    if (ste)
    {
        return nullptr;
    }
    else
    {
        ste = new SymbolTableEntry(entryName, Type, constValue);
        ste->next = head;
        head = ste;
        
        counter++;
    }
    return ste;
}



void EntryList::printAll(ofstream &fout)
{
    SymbolTableEntry *ste = head;
    while (ste != nullptr)
    {
        ste->print(fout);
        ste = ste->next;
    }
    fout<<"\n";
    
}



int EntryList::count()
{
    return counter;
}



void EntryList::clear()
{
    SymbolTableEntry *ste;
    while (head != nullptr)
    {
        ste = head;
        head = head->next;
        delete ste;
    }
    counter =0;
}




EntryList::~EntryList()
{
    clear();
}


