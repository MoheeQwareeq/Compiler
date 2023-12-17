//
//  Parser.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#ifndef Parser_h
#define Parser_h


#include "FileDescriptor.h"
#include "AST.h"
#include "Scanner.h"
#include "SymbolTableList.h"
#include "SemanticChecker.h"
#include "CodeGenerator.h"

class Parser {
    
private:
    FileDescriptor * fileDescriptor;
    Scanner * scanner;
    SymbolTableList * global;
    SemanticChecker * semanticChecker;
    CodeGenerator * codeGenerator;
    Token * token;
    ofstream fout;
    int offset;
    int numberOfFormal;
    void match(Lexeme );
    ast_list * parse_program();
    ast_list * parse_decl_list(ast_list *);
    AST * parse_decl();
    J_TYPE parse_type();
    ste_list * parse_formal_list();
    ste_list * parse_formal_list_tail();
    ste_list * parse_formals();
    ste_list * parse_formals_tail(ste_list *);
    AST* parse_stmt();
    AST* parse_stmt_tail();
    AST* parse_block();
    ste_list * parse_var_decl_list(ste_list *);
    SymbolTableEntry * parse_var_decl();
    ast_list * parse_stmt_list(ast_list *);
    AST * parse_id_tail(SymbolTableEntry *);
    ast_list * parse_arg_list();
    ast_list * parse_arg_list_tail();
    ast_list * parse_args(ast_list *);
    AST * parse_args_tail(ast_list *);
    AST * parse_expr();
    AST * parse_expr_tail(AST *);
    AST * parse_expr1();
    AST * parse_expr1_tail(AST *);
    AST * parse_expr2();
    AST * parse_expr2_tail(AST *);
    AST * parse_expr3();
    AST * parse_expr3_tail(AST *);
    AST * parse_expr4();
    AST * parse_expr5();
    AST * parse_arith_op_tail1();
    AST * parse_arith_op_tail2();
    AST * parse_rel_op();
    AST * parse_rel_conj();
    AST * parse_unary_op();
    bool first_of_stmt();
    bool is_rel_op();
    ast_list * parse_expr_id_tail();

public:

    Parser(FileDescriptor & , SymbolTableList &);
    ast_list * parse();
    void print(ast_list *);
    ~Parser();
    
};


#endif /* Parser_h */






