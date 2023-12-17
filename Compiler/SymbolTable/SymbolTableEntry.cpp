//
//  SymbolTableEntry.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#include "SymbolTableEntry.h"
#include <iostream>


SymbolTableEntry::SymbolTableEntry(){
    next = nullptr;
    type = STE_UNDEFINED;
    name = ""; //empty String
    offset = -1;
}

SymbolTableEntry::SymbolTableEntry(string entryName, STE_ENTRY_TYPE steType, J_TYPE jType ,int constValue){
    next = nullptr;
    type = steType;
    offset = -1;
    name = entryName;
    if (steType == STE_VAR)
        var.type = jType;
    
    else if (steType == STE_CONST)
        constant.value = constValue;
    
    else if (steType == STE_ROUTINE)
        routine.resultType = jType;
}

string SymbolTableEntry:: toString(){
    J_TYPE tmp;
    string str;
    int v = 0;
    if ((type >= STE_UNDEFINED) || type< STE_VAR)
        type = STE_UNDEFINED;
    
    if (type == STE_VAR)
        tmp = var.type;
    
    else if (type == STE_ROUTINE)
        tmp = routine.resultType;
    
    else if (type == STE_CONST) {
        tmp = TYPE_INTEGER;
        v = constant.value;
    }
    else
        tmp = TYPE_NONE;
    
    if (type == STE_CONST)
        str = "(" + name + "," + ENTRY_TYPE_NAMES[type] + "," + TYPE_NAMES[tmp] + "," + to_string(v) + ")";
    
    else
        str = "(" + name + "," + ENTRY_TYPE_NAMES[type] + "," + TYPE_NAMES[tmp] + ")";
    
    return str;
}

void  SymbolTableEntry::print(ofstream & fout){
    fout << toString();
}

J_TYPE SymbolTableEntry::getType(){
    if(type == STE_VAR)
        return var.type ;
    else if(type == STE_CONST)
        return TYPE_INTEGER;
    else if(type == STE_ROUTINE)
        return routine.resultType ;

    return TYPE_NONE;
}


int SymbolTableEntry::steConstValue(SymbolTableEntry * e){
    return e->constant.value;
}


string SymbolTableEntry::steName(SymbolTableEntry * e){
    return e->name;
}


J_TYPE SymbolTableEntry::steVarType(SymbolTableEntry * e){
    return e->var.type;
}


