//
//  AST.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 08/08/2023.
//

#include "Ast.h"
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
using namespace std;


void newLineIndent (ofstream &fout, int);
void printDecl (ofstream &fout, Ast *, int);
void printAstList (ofstream &fout, AstList *, string, int);
void printSteList (ofstream &fout, SteList *,string,string, int);

void fatalError(FileDescriptor *fd , string message){
    fd->reportError("Syntax Error: "+message);
    cout<<endl;
    remove("symbol tabel.txt");
    remove("out.txt");
    remove("assmbly.asm");
    exit(EXIT_FAILURE);
}


Ast * makeAstNode (AST_type type, ...){
    Ast *n = (Ast *)malloc(sizeof(Ast));
    if (n == nullptr) return nullptr;
    n->type = type;
    va_list ap;
    
    va_start (ap,type);
    switch (type) {
        case AST_VAR_DECL:
            n->aVarDecl.name = va_arg (ap, SymbolTableEntry *);
            n->aVarDecl.type = va_arg (ap, J_TYPE);
            break;
            
        case AST_CONST_DECL:
            n->aConstDecl.name = va_arg (ap,SymbolTableEntry *);
            n->aConstDecl.value = va_arg (ap, int);
            break;
            
        case AST_ROUTINE_DECL:
            n->aRoutineDecl.name = va_arg (ap, SymbolTableEntry *);
            n->aRoutineDecl.formals = va_arg (ap, SteList *);
            n->aRoutineDecl.resultType = va_arg (ap, J_TYPE );
            n->aRoutineDecl.body = va_arg (ap,Ast *);
            break;
            
        case AST_ASSIGN:
            n->aAssign.lhs = va_arg (ap, SymbolTableEntry *);
            n->aAssign.rhs = va_arg (ap, Ast *);
            break;
            
        case AST_IF:
            n->aIf.predicate = va_arg (ap,Ast *);
            n->aIf.conseq    = va_arg (ap, Ast *);
            n->aIf.altern    = va_arg (ap, Ast *);
            break;
            
        case AST_WHILE:
            n->aWhile.predicate = va_arg (ap, Ast *);
            n->aWhile.body = va_arg (ap, Ast *);
            break;
            
        case AST_FOR:
            n->aFor.var = va_arg (ap, SymbolTableEntry *);
            n->aFor.lowerBound = va_arg(ap, Ast *);
            n->aFor.upperBound = va_arg (ap, Ast *);
            n->aFor.body = va_arg (ap, Ast *);
            break;
            
        case AST_READ:
            n->aRead.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_WRITE:
            n->aWrite.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_CALL:
            n->aCall.callee = va_arg (ap, SymbolTableEntry *);
            n->aCall.argList = va_arg (ap, AstList *);
            break;
            
        case AST_BLOCK:
            n->aBlock.vars = va_arg (ap, SteList *);
            n->aBlock.stmts = va_arg (ap, AstList *);
            break;
            
        case AST_RETURN:
            n->aReturn.expr = va_arg (ap, Ast *);
            break;
            
        case AST_VAR:
            n->aVar.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_INTEGER:
            n->aInteger.int_value = va_arg (ap, int);
            break;
            
        case AST_STRING:
            n->aString.stringValue = va_arg (ap, char *);
            break;
            
        case AST_BOOLEAN:
            n->aBoolean.value = va_arg (ap,int);
            break;
            
        case AST_TIMES:
        case AST_DIVIDE:
        case AST_PLUS:
        case AST_MINUS:
        case AST_EQ:
        case AST_NEQ:
        case AST_LT:
        case AST_LE:
        case AST_GT:
        case AST_GE:
        case AST_AND:
        case AST_OR:
            n->aBinaryOp.larg = va_arg (ap, Ast *);
            n->aBinaryOp.rarg = va_arg (ap, Ast *);
            break;
            
        case AST_NOT:
        case AST_UMINUS:
            n->aUnaryOp.arg = va_arg (ap, Ast *);
            break;
            
        case AST_FLOAT:
            n->aFloat.floatValue = static_cast<float>(va_arg(ap, double));
            break;
            
        case AST_EOF:
            break;
            
        default: n=nullptr;
    }
    va_end(ap);
    return (n);
}


float evalAstExpr (ofstream  &fout,FileDescriptor * fileDescriptor,Ast * n){
    if (n == nullptr) fatalError (fileDescriptor,"invalid expretione");
    
    switch (n->type){
        case AST_VAR:
            if (n->aVar.var->type == STE_CONST)
                return (SymbolTableEntry::steConstValue(n->aVar.var));
            else
                fatalError (fileDescriptor,"Cannot use variables in constant expressions");
            return (0);
        case AST_INTEGER:
            return (n->aInteger.int_value);
        case AST_FLOAT:
            return (n->aFloat.floatValue);
            
        case AST_TIMES:
            return (evalAstExpr (fout,fileDescriptor,n->aBinaryOp.larg)
                    * evalAstExpr (fout,fileDescriptor, n->aBinaryOp.rarg));
        case AST_DIVIDE:
            return (evalAstExpr (fout,fileDescriptor, n->aBinaryOp.larg)
                    / evalAstExpr (fout,fileDescriptor,n->aBinaryOp.rarg));
        case AST_PLUS:
            return (evalAstExpr (fout,fileDescriptor,n->aBinaryOp.larg)
                    + evalAstExpr (fout,fileDescriptor, n->aBinaryOp.rarg));
        case AST_MINUS:
            return (evalAstExpr (fout,fileDescriptor, n->aBinaryOp.larg)
                    -evalAstExpr (fout, fileDescriptor,n->aBinaryOp.rarg));
            
        case AST_UMINUS:
            return (- evalAstExpr (fout,fileDescriptor, n->aUnaryOp.arg));
        default:
            fatalError (fileDescriptor,"invalid expretione");
            return (0);
    }
}




void printAstNode (ofstream &fout, Ast * n){
    printDecl (fout, n, 0);
}

void printDecl (ofstream &fout, Ast * n, int d){
    if (n == nullptr) return;
    switch (n->type){
        case AST_VAR_DECL:
            fout << "var " << SymbolTableEntry::steName(n->aVarDecl.name) << ": "
            << TYPE_NAMES[n->aVarDecl.type] << ";";
            newLineIndent (fout, d);
            break;
            
        case AST_CONST_DECL:
            fout << "constant " << SymbolTableEntry::steName(n->aConstDecl.name) << " = "
            << n->aConstDecl.value << ";";
            newLineIndent (fout, d);
            break;
            
        case AST_ROUTINE_DECL:
        {
            if (n->aRoutineDecl.resultType == TYPE_NONE)
                fout <<"\n\n\nprocedure " << SymbolTableEntry::steName(n->aRoutineDecl.name) << " (";
            else
                fout << "\n\n\nfunction " << SymbolTableEntry::steName(n->aRoutineDecl.name) << " (";
            printSteList (fout, n->aRoutineDecl.formals, "", ", ", -1);
            
            
            
            if (n->aRoutineDecl.resultType == TYPE_NONE){
                fout << ")";
                newLineIndent (fout, d + 2);
            } else{
                fout << ") : " << TYPE_NAMES[n->aRoutineDecl.resultType];
                newLineIndent (fout, d + 2);
            }
            printDecl (fout, n->aRoutineDecl.body, d + 2);
            fout << ";";
            newLineIndent (fout, d);
            break;
        }
        case AST_ASSIGN:
            fout << SymbolTableEntry::steName(n->aAssign.lhs) << " := ";
            printDecl (fout, n->aAssign.rhs, d);
            break;
            
        case AST_IF:
            fout << "if ";
            printDecl (fout, n->aIf.predicate, d);
            fout << " then";
            newLineIndent (fout, d + 2);
            printDecl (fout, n->aIf.conseq, d + 2);
            if (n->aIf.altern != nullptr){
                newLineIndent (fout, d);
                fout << "else";
                newLineIndent (fout, d + 2);
                printDecl (fout, n->aIf.altern, d + 2);
            }
            break;
            
        case AST_WHILE:
            fout << "while ";
            printDecl (fout, n->aWhile.predicate, d);
            fout << " do";
            newLineIndent (fout, d + 2);
            printDecl (fout, n->aWhile.body, d + 2);
            newLineIndent (fout, d);
            fout << "od";
            break;
            
        case AST_FOR:
            fout << "for " << SymbolTableEntry::steName(n->aFor.var) << " = ";
            printDecl (fout, n->aFor.lowerBound, d);
            fout << " to ";
            printDecl (fout, n->aFor.upperBound, d);
            fout << " do";
            newLineIndent (fout, d + 2);
            printDecl (fout, n->aFor.body, d + 2);
            newLineIndent (fout, d);
            fout << "od";
            break;
            
        case AST_READ:
            fout << "read (" << SymbolTableEntry::steName(n->aRead.var) << ")";
            break;
            
        case AST_WRITE:
            fout << "write (" << SymbolTableEntry::steName(n->aRead.var) << ")";
            break;
            
        case AST_CALL:
            fout << SymbolTableEntry::steName(n->aCall.callee) << " (";
            printAstList (fout, n->aCall.argList, ", ", -1);
            fout << ")";
            break;
            
        case AST_BLOCK:
            fout << "begin\n";
            newLineIndent (fout, d + 2);
            printSteList (fout, n->aBlock.vars, "var ", ";", d + 2);
            printAstList (fout, n->aBlock.stmts, ";", d + 2);
            newLineIndent (fout, d);
            fout << "end";
            break;
            
        case AST_RETURN:
            fout << "return (";
            printDecl (fout, n->aReturn.expr, d);
            fout << ")";
            break;
            
        case AST_VAR:
            fout << SymbolTableEntry::steName(n->aVar.var);
            break;
            
        case AST_INTEGER:
            fout << n->aInteger.int_value;
            break;
            
        case AST_FLOAT:
            fout << n->aFloat.floatValue;
            break;
        case AST_STRING:
            fout << "\"" << n->aString.stringValue << "\"";
            break;
            
        case AST_BOOLEAN:
            fout << (n->aBoolean.value ? "true" : "false");
            break;
            
        case AST_TIMES:
        case AST_DIVIDE: // new page
        case AST_PLUS:
        case AST_MINUS:
        case AST_EQ:
        case AST_NEQ:
        case AST_LT:
        case AST_LE:
        case AST_GT:
        case AST_GE:
        case AST_AND:
        case AST_OR:
            fout << "(";
            printDecl (fout, n->aBinaryOp.larg, d);
            switch (n->type) {
                case AST_TIMES:
                    fout<<" * ";
                    break;
                    
                case AST_DIVIDE:
                    fout<<" / ";
                    break;
                    
                case AST_PLUS:
                    fout<<" + ";
                    break;
                    
                case AST_MINUS:
                    fout<<" -";
                    break;
                    
                case AST_EQ:
                    fout<<" = ";
                    break;
                    
                case AST_NEQ:
                    fout<<" != ";
                    break;
                    
                case AST_LT:
                    fout<<" < ";
                    break;
                    
                case AST_LE:
                    fout<<" <= ";
                    break;
                    
                case AST_GT:
                    fout<<" > ";
                    break;
                    
                case AST_GE:
                    fout<<" >= ";
                    break;
                    
                case AST_AND:
                    fout<<" and ";
                    break;
                    
                case AST_OR:
                    fout<<" or ";
                    break;
                    
                default:
                    break;
            }
            printDecl (fout, n->aBinaryOp.rarg, d);
            fout<<")";
            break;
            
        case AST_NOT:
            fout<<"(not ";
            printDecl (fout, n->aUnaryOp.arg, d);
            fout<<")";
            break;
            
        case AST_UMINUS:
            fout<<"( -";
            printDecl (fout, n->aUnaryOp.arg, d);
            fout<<")";
            break;
            
        case AST_EOF:
            fout<<"EOF";
            break;
            
        default:
            break;
    }
}


void newLineIndent (ofstream &fout, int d){
    fout << "\n";
    while (d > 0) {
        fout << " ";
        d--;
    }
}


void printSteList (ofstream &fout, SteList * l, string prefix, string sep, int d) {
    for ( ; l->next != nullptr;  l=l->next) {
        fout << prefix << SymbolTableEntry::steName(l->head) << " : " << TYPE_NAMES[SymbolTableEntry::steVarType(l->head)];
        if (l->next->next || d >= 0) fout << sep;
        if (d >= 0) newLineIndent (fout, d);
    }
}


void printAstList (ofstream &fout, AstList * l,string sep, int d){
    for ( ; l->next != nullptr; l = l->next){
        printDecl (fout, l->head, d);
        if (l->next->next || d>0){
            fout << sep;
            if (l->next && d >= 0) newLineIndent (fout, d);
        }
    }
}



