//
//  SymbolTableEntry.cpp
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#include "SymbolTableEntry.h"
#include <iostream>


SymbolTableEntry::SymbolTableEntry() {
    next = nullptr;
    type = STE_UNDEFINED;
    name=""; //empty String
    offset=-1;

}


SymbolTableEntry::SymbolTableEntry(string entryName, ste_entry_type Type, j_type jType)
{
    next= nullptr;
    type = Type;
    offset=-1;
    name=entryName;
    if (Type == STE_VAR) {
        var.type = jType;
    }
    else if (Type == STE_ROUTINE) {
        routine.result_type = jType;
    }
}

SymbolTableEntry::SymbolTableEntry(string entryName, ste_entry_type Type, int constValue)
{
    next = nullptr;
    type = Type;
    name = entryName;
    offset=-1;
    if (Type == STE_CONST) {
        constant.value = constValue;
    }
}


string SymbolTableEntry:: toString()
{
    j_type x;
    int v = 0;
    if ((type >= STE_UNDEFINED) || type< STE_VAR)  type = STE_UNDEFINED;
    if (type == STE_VAR) {
        x = var.type;
    }
    else if (type == STE_ROUTINE) {
        x = routine.result_type;
    }
    else if (type == STE_CONST) {
        x = TYPE_INTEGER;
        v = constant.value;
    }
    else {
        x = TYPE_NONE;
    }
    if (type == STE_CONST) {
        str = "(" + name + "," + entry_type_names[type] + "," + type_names[x] + "," + to_string(v) + ")";
    }
    else {
        str = "(" + name + "," + entry_type_names[type] + "," + type_names[x] + ")";
    }
    return str;
}



void  SymbolTableEntry::print(ofstream &fout)
{
    fout << toString();
    cout << toString();
}




j_type SymbolTableEntry::getType()
{
    
    if(type == STE_VAR) return var.type ;
    else if(type == STE_CONST) return TYPE_INTEGER;
    else if(type == STE_ROUTINE) return routine.result_type ;

    
    
    return TYPE_NONE;
    
}



int SymbolTableEntry::ste_const_value(SymbolTableEntry* e) {
    return e->constant.value;
}


string SymbolTableEntry::ste_name(SymbolTableEntry* e) {
    return e->name;
}


j_type SymbolTableEntry::ste_var_type(SymbolTableEntry* e) {
    return e->var.type;
}


