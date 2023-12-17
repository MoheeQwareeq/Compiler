//
//  SymbolTableList.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 12/08/2023.
//

#include "SymbolTableList.h"
#include <iostream>


SymbolTableList::SymbolTableList() {
    fout.open("symbol tabel.txt");

    head = new SymbolTable();
    head->next=NULL;
    count = 0;
}

SymbolTableList::SymbolTableList(bool foldCase) {
    fout.open("symbol tabel.txt");

    head = new SymbolTable(foldCase);
    head->next=NULL;
    count = 0;
}

SymbolTable* SymbolTableList::getHead() {
    return head;
}

void SymbolTableList::enterScope() {
    SymbolTable * scope = new SymbolTable();
    scope->next = head;
    head = scope;
    count++;
}

void SymbolTableList::exitScope() {
    SymbolTable * scope = head;
    scope->printAll(fout);
    head = head->next;
    count--;
   //delete scope;
}


void SymbolTableList::printTable() {
    SymbolTable * tmp = head;
    int counter = 0;
    while (tmp) {
        tmp->printAll(fout);
        tmp = tmp->next;
        counter++;
    }
}


SymbolTableEntry * SymbolTableList::getMySymbol(string name) {
    SymbolTable* st;
    SymbolTableEntry* ste;
    st = head;
    while (st) {
        ste = st->getSymbol(name);
        if (ste) return ste;
        else
            st = st->next;
        
    }
    return nullptr;
}


SymbolTableList::~SymbolTableList(){
    
}
