//
//  SymbolTableList.cpp
//  Parser
//
//  Created by MOHEE QWAREEQ on 12/08/2023.
//

#include "SymbolTableList.h"
#include <iostream>


SymbolTableList::SymbolTableList() {
    head = new SymbolTable();
    head->next=NULL;
    count = 0;
}

SymbolTableList::SymbolTableList(bool foldCase) {
    head = new SymbolTable(foldCase);
    head->next=NULL;
    count = 0;
}

SymbolTable* SymbolTableList::getHead() {
    return head;
}

void SymbolTableList::enterScope() {
    SymbolTable* scope = new SymbolTable();
    scope->next = head;
    head = scope;
    count++;
}

void SymbolTableList::exitScope() {
    head = head->next;
    count--;
}


ofstream fout("out.txt");
void SymbolTableList::printTable() {
    SymbolTable* tmp = head;
    int counterr = 0;
    while (tmp != nullptr) {
        cout << "Table[" << counterr << "]" << endl;
        tmp->printAll(fout);
        tmp = tmp->next;
        counterr++;
        cout << endl;
        
    }
}


SymbolTableEntry* SymbolTableList::getMySymbol(string name) {
    SymbolTable* st;
    SymbolTableEntry* ste;
    st = head;
    while (st != nullptr) {
        ste = st->getSymbol(name);
        if (ste) return ste;
        else {
            st = st->next;
        }
    }
    return nullptr;
}
