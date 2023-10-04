//
//  AST.cpp
//  Parser
//
//  Created by MOHEE QWAREEQ on 08/08/2023.
//

#include "AST.h"
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
using namespace std;


void nl_indent (ofstream &fout, int);
void p_a_n (ofstream &fout, AST *, int);
void print_ast_list (ofstream &fout, ast_list *, string, int);
void print_ste_list (ofstream &fout, ste_list *,string,string, int);

void fatal_error(FileDescriptor *fd , string message) {
    fd->reportError("Syntax Error: "+message);
    cout<<endl;
    remove("out.txt");
    remove("assmbly.asm");
    exit(EXIT_FAILURE);
}



AST * make_ast_node (AST_type type, ...){
    AST *n = (AST *)malloc(sizeof(AST));
    if (n == nullptr) return nullptr;
    n->type = type;
    va_list ap;
    
    va_start (ap,type);
    switch (type) {
        case AST_VAR_DECL:
            n->a_var_decl.name = va_arg (ap, SymbolTableEntry *);
            n->a_var_decl.type = va_arg (ap, j_type);
            break;
            
        case AST_CONST_DECL:
            n->a_const_decl.name = va_arg (ap,SymbolTableEntry *);
            n->a_const_decl.value = va_arg (ap, int);
            break;
            
        case AST_ROUTINE_DECL:
            n->a_routine_decl.name = va_arg (ap, SymbolTableEntry *);
            n->a_routine_decl.formals = va_arg (ap, ste_list *);
            n->a_routine_decl.result_type = va_arg (ap, j_type );
            n->a_routine_decl.body = va_arg (ap,AST *);
            break;
            
        case AST_ASSIGN:
            n->a_assign.lhs = va_arg (ap, SymbolTableEntry *);
            n->a_assign.rhs = va_arg (ap, AST *);
            break;
            
        case AST_IF:
            n->a_if.predicate = va_arg (ap,AST *);
            n->a_if.conseq    = va_arg (ap, AST *);
            n->a_if.altern    = va_arg (ap, AST *);
            break;
            
        case AST_WHILE:
            n->a_while.predicate = va_arg (ap, AST *);
            n->a_while.body = va_arg (ap, AST *);
            break;
            
        case AST_FOR:
            n->a_for.var = va_arg (ap, SymbolTableEntry *);
            n->a_for.lower_bound = va_arg(ap, AST *);
            n->a_for.upper_bound = va_arg (ap, AST *);
            n->a_for.body = va_arg (ap, AST *);
            break;
            
        case AST_READ:
            n->a_read.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_WRITE:
            n->a_write.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_CALL:
            n->a_call.callee = va_arg (ap, SymbolTableEntry *);
            n->a_call.arg_list = va_arg (ap, ast_list *);
            break;
            
        case AST_BLOCK:
            n->a_block.vars = va_arg (ap, ste_list *);
            n->a_block.stmts = va_arg (ap, ast_list *);
            break;
            
        case AST_RETURN:
            n->a_return.expr = va_arg (ap, AST *);
            n->a_return.returnType=va_arg (ap,j_type);
            break;
            
        case AST_VAR:
            n->a_var.var = va_arg (ap, SymbolTableEntry *);
            break;
            
        case AST_INTEGER:
            n->a_integer.int_value = va_arg (ap, int);
            break;
            
        case AST_STRING:
            n->a_string.string_value = va_arg (ap, char *);
            break;
            
        case AST_BOOLEAN:
            n->a_boolean.value = va_arg (ap,int);
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
            n->a_binary_op.larg = va_arg (ap, AST *);
            n->a_binary_op.rarg = va_arg (ap, AST *);
            break;
            
        case AST_NOT:
        case AST_UMINUS:
            n->a_unary_op.arg = va_arg (ap, AST *);
            break;
            
        case AST_FLOAT:
            n->a_float.float_value = static_cast<float>(va_arg(ap, double));
            break;
            
        case AST_ITOF:
            n->a_itof.arg = va_arg (ap, AST *);
            break;
            
        case AST_EOF:
            break;
            
        default: n=nullptr;
    }
    va_end(ap);
    return (n);
}





float eval_ast_expr (ofstream  &fout,FileDescriptor*fileDescriptor,AST *n)
{
    if (n == nullptr) fatal_error (fileDescriptor,"invalid expretione");
    
    switch (n->type){
        case AST_VAR:
            if (n->a_var.var->type == STE_CONST)
                return (SymbolTableEntry::ste_const_value(n->a_var.var));
            else
                fatal_error (fileDescriptor,"Cannot use variables in constant expressions");
            return (0);
        case AST_INTEGER:
            return (n->a_integer.int_value);
        case AST_FLOAT:
            return (n->a_float.float_value);
            
        case AST_TIMES:
            return (eval_ast_expr (fout,fileDescriptor,n->a_binary_op.larg)
                    * eval_ast_expr (fout,fileDescriptor, n->a_binary_op.rarg));
        case AST_DIVIDE:
            return (eval_ast_expr (fout,fileDescriptor, n->a_binary_op.larg)
                    / eval_ast_expr (fout,fileDescriptor,n->a_binary_op.rarg));
        case AST_PLUS:
            return (eval_ast_expr (fout,fileDescriptor,n->a_binary_op.larg)
                    + eval_ast_expr (fout,fileDescriptor, n->a_binary_op.rarg));
        case AST_MINUS:
            return (eval_ast_expr (fout,fileDescriptor, n->a_binary_op.larg)
                    -eval_ast_expr (fout, fileDescriptor,n->a_binary_op.rarg));
            
        case AST_UMINUS:
            return (- eval_ast_expr (fout,fileDescriptor, n->a_unary_op.arg));
        default:
            fatal_error (fileDescriptor,"invalid expretione");
            return (0);
    }
}




void print_ast_node (ofstream &fout, AST *n)
{
    p_a_n (fout, n, 0);
}

void p_a_n (ofstream &fout, AST *n, int d){
    if (n == nullptr) return;
    switch (n->type)
    {
            // new page 6
        case AST_VAR_DECL:
            fout << "var " << SymbolTableEntry::ste_name(n->a_var_decl.name) << ": "
            << type_names[n->a_var_decl.type] << ";";
            nl_indent (fout, d);
            break;
        case AST_CONST_DECL:
            fout << "constant " << SymbolTableEntry::ste_name(n->a_const_decl.name) << " = "
            << n->a_const_decl.value << ";";
            nl_indent (fout, d);
            break;
        case AST_ROUTINE_DECL:
        {
            if (n->a_routine_decl.result_type == TYPE_NONE)
                fout <<"\n\n\nprocedure " << SymbolTableEntry::ste_name(n->a_routine_decl.name) << " (";
            else
                fout << "\n\n\nfunction " << SymbolTableEntry::ste_name(n->a_routine_decl.name) << " (";
            print_ste_list (fout, n->a_routine_decl.formals, "", ", ", -1);
            
            
            
            if (n->a_routine_decl.result_type == TYPE_NONE){
                fout << ")";
                nl_indent (fout, d + 2);
            } else{
                fout << ") : " << type_names[n->a_routine_decl.result_type];
                nl_indent (fout, d + 2);
            }
            p_a_n (fout, n->a_routine_decl.body, d + 2);
            fout << ";";
            nl_indent (fout, d);
            break;
        }
        case AST_ASSIGN:
            fout << SymbolTableEntry::ste_name(n->a_assign.lhs) << " := ";
            p_a_n (fout, n->a_assign.rhs, d);
            break;
        case AST_IF:
            fout << "if ";
            p_a_n (fout, n->a_if.predicate, d);
            fout << " then";
            nl_indent (fout, d + 2);
            p_a_n (fout, n->a_if.conseq, d + 2);
            if (n->a_if.altern != nullptr){
                nl_indent (fout, d);
                fout << "else";
                nl_indent (fout, d + 2);
                p_a_n (fout, n->a_if.altern, d + 2);
            }
            break;
        case AST_WHILE:
            fout << "while ";
            p_a_n (fout, n->a_while.predicate, d);
            fout << " do";
            nl_indent (fout, d + 2);
            p_a_n (fout, n->a_while.body, d + 2);
            nl_indent (fout, d);
            fout << "od";
            break;
        case AST_FOR:
            fout << "for " << SymbolTableEntry::ste_name(n->a_for.var) << " = ";
            p_a_n (fout, n->a_for.lower_bound, d);
            fout << " to ";
            p_a_n (fout, n->a_for.upper_bound, d);
            fout << " do";
            nl_indent (fout, d + 2);
            p_a_n (fout, n->a_for.body, d + 2);
            nl_indent (fout, d);
            fout << "od";
            break;
        case AST_READ:
            fout << "read (" << SymbolTableEntry::ste_name(n->a_read.var) << ")";
            break;
        case AST_WRITE:
            fout << "write (" << SymbolTableEntry::ste_name(n->a_read.var) << ")";
            break;
        case AST_CALL:
            fout << SymbolTableEntry::ste_name(n->a_call.callee) << " (";
            print_ast_list (fout, n->a_call.arg_list, ", ", -1);
            fout << ")";
            break;
        case AST_BLOCK:
            fout << "begin\n";
            nl_indent (fout, d + 2);
            print_ste_list (fout, n->a_block.vars, "var ", ";", d + 2);
            print_ast_list (fout, n->a_block.stmts, ";", d + 2);
            nl_indent (fout, d);
            fout << "end";
            break;
        case AST_RETURN:
            fout << "return (";
            p_a_n (fout, n->a_return.expr, d);
            fout << ")";
            break;
        case AST_VAR:
            fout << SymbolTableEntry::ste_name(n->a_var.var);
            break;
        case AST_INTEGER:
            fout << n->a_integer.int_value;
            break;
        case AST_FLOAT:
            fout << n->a_float.float_value;
            break;
        case AST_STRING:
            fout << "\"" << n->a_string.string_value << "\"";
            break;
        case AST_BOOLEAN:
            fout << (n->a_boolean.value ? "true" : "false");
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
            p_a_n (fout, n->a_binary_op.larg, d);
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
            p_a_n (fout, n->a_binary_op.rarg, d);
            fout<<")";
            break;
        case AST_NOT:
            fout<<"(not ";
            p_a_n (fout, n->a_unary_op.arg, d);
            fout<<")";
            break;
        case AST_UMINUS:
            fout<<"( -";
            p_a_n (fout, n->a_unary_op.arg, d);
            fout<<")";
            break;
        case AST_EOF:
            fout<<"EOF";
            break;
        default:
            break;
    }
}


void nl_indent (ofstream &fout, int d)
{
    fout << "\n";
    while (d > 0) {
        fout << " ";
        d--;
    }
}


void print_ste_list (ofstream &fout, ste_list *L, string prefix, string sep, int d) {
    for ( ; L->next != nullptr;  L=L->next) {
        fout << prefix << SymbolTableEntry::ste_name(L->head) << " : " << type_names[SymbolTableEntry::ste_var_type(L->head)];
        if (L->next->next || d >= 0) fout << sep;
        if (d >= 0) nl_indent (fout, d);
    }
}


void print_ast_list (ofstream &fout, ast_list *L,string sep, int d){
    for ( ; L->next != nullptr; L = L->next){
        p_a_n (fout, L->head, d);
        if (L->next->next || d>0){
            fout << sep;
            if (L->next && d >= 0) nl_indent (fout, d);
        }
    }
}



