//
//  Parser.cpp
//  Parser
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//


#include "Parser.h"


Parser::Parser(FileDescriptor* fd, SymbolTableList* GLOBAL) {
    fileDescriptor = fd;
    scanner = new Scanner(fileDescriptor);
    global = GLOBAL;
    semanticChecker = new SemanticChecker(fd);
    codeGenerator = new CodeGenerator();
    fout.open("out.txt");
    offset=0;
    numberOfFormal=0;
}

void Parser::match(Lexeme t) {
    if (t == token->type) {
        delete token;
        token = scanner->scan();
        return;
    }
    
    else fatal_error(fileDescriptor, "");
}


ast_list* Parser::parse() {
    token = scanner->scan();
    return parse_program();
}


ast_list* Parser::parse_program() {
    ast_list* program;
    program = parse_decl_list(new ast_list());
    return program;
}


ast_list* Parser::parse_decl_list(ast_list* list) {
    ast_list * decl_list = list;
    
    if (token->type == LX_EOF) {
        AST* eof;
        eof = make_ast_node(AST_EOF);
        decl_list->head = eof;               //add EOF_AST to list λ case
        decl_list->next = nullptr;
        SymbolTableEntry* entry;
        entry = global->getMySymbol("main");
 
        if (!entry or entry->type != STE_ROUTINE) {
            cout << "no main function\n\n";
            remove("out.txt");
            remove("assmbly.asm");
            exit(1);
        }
    }
    
    else {
        AST* decl;
        decl = parse_decl();
        match(LX_SEMICOLON);
        decl_list->head = decl;             //add decl to list
        decl_list->next = new ast_list();   //creat new ast_list
        parse_decl_list(list->next);
    }
    
    return decl_list;
}


AST* Parser::parse_decl() {
    AST* decl = NULL;
    
    if (token->type == KW_VAR) {
        j_type  type;
        SymbolTableEntry* name;
        string varName;
        match(KW_VAR);
        varName = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON);
        type = parse_type();
        name = global->getHead()->putSymbol(varName, STE_VAR, type);
        if (name != nullptr) decl = make_ast_node(AST_VAR_DECL, name, type);
        else fatal_error(fileDescriptor, "name already definded");
        codeGenerator->generate(decl);
    }
    
    
    else if (token->type == KW_CONSTANT) {
        SymbolTableEntry* name;
        int  value;
        string constName;
        match(KW_CONSTANT);
        AST* expr;
        constName = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_EQ);
        expr = parse_expr();
        if (!expr )fatal_error(fileDescriptor, "invalid assign for constant");
        value =eval_ast_expr(fout,fileDescriptor,expr);
        name = global->getHead()->putSymbol(constName, STE_CONST,value);
        if (name != nullptr)
            decl = make_ast_node(AST_CONST_DECL, name, value);
        
        else fatal_error(fileDescriptor, "name already definded");
        codeGenerator->generate(decl);

        
    }
    
    
    
    else if (token->type == KW_FUNCTION) {
        SymbolTableEntry* name;
        string funcName;
        j_type result_type;
        AST* block = nullptr;
        ste_list* formals_list;
        match(KW_FUNCTION);
        funcName = token->stringValue;
        match(LX_IDENTIFIER);
        global->enterScope();
        offset=0;
        numberOfFormal=0;
        formals_list = parse_formal_list();
        match(LX_COLON);
        result_type = parse_type();
        block = parse_block();
        global->exitScope();
        name = global->getHead()->putSymbol(funcName, STE_ROUTINE, result_type);
        if (name != nullptr){
            name->routine.formals=formals_list;
            name->routine.formals=formals_list;
            decl = make_ast_node(AST_ROUTINE_DECL, name, formals_list,result_type,block);
            semanticChecker->check_Statement(decl,result_type);
        }
        else fatal_error(fileDescriptor, "name already definded");
        decl->a_routine_decl.num=offset;
        decl->a_routine_decl.num_of_formal=numberOfFormal;
        codeGenerator->generate(decl);

    }
    
    else if (token->type == KW_PROCEDURE) {
        SymbolTableEntry* name;
        ste_list* formals_list;
        AST* block;
        match(KW_PROCEDURE);
        string funcName = token->stringValue;
        match(LX_IDENTIFIER);
        global->enterScope();
        offset=0;
        numberOfFormal=0;
        formals_list = parse_formal_list();
        block = parse_block();
        global->exitScope();
        name = global->getHead()->putSymbol(funcName, STE_ROUTINE, TYPE_NONE);
        if (name != nullptr){
            name->routine.formals=formals_list;
            decl = make_ast_node(AST_ROUTINE_DECL, name, formals_list,TYPE_NONE ,block);
            semanticChecker->check_Statement(decl,TYPE_NONE);
        }
        else fatal_error(fileDescriptor, "name already definded");
        decl->a_routine_decl.num=offset;
        decl->a_routine_decl.num_of_formal=numberOfFormal;
        codeGenerator->generate(decl);
    }
    
    else fatal_error(fileDescriptor, "name already definded");
    
    return decl;
}





j_type Parser::parse_type() {
    
    if (token->type == KW_INTEGER) {
        match(KW_INTEGER);
        return TYPE_INTEGER;
    }
    
    else if (token->type == KW_STRING) {
        match(KW_STRING);
        return TYPE_STRING;
    }
    
    else if (token->type == KW_BOOLEAN) {
        match(KW_BOOLEAN);
        return TYPE_BOOLEAN;
    }
    
    else if (token->type == KW_FLOAT ) {
        match(KW_FLOAT);
        return TYPE_FLOAT;
    }
    else
        fatal_error(fileDescriptor, "unknown type");
    
    return TYPE_NONE;
}


ste_list* Parser::parse_formal_list() {
    ste_list* formal_list = nullptr;
    ste_list* formal_list_tail = nullptr;
    match(LX_LPAREN);
    formal_list_tail = parse_formal_list_tail();
    formal_list = formal_list_tail;
    return formal_list;
}


ste_list* Parser::parse_formal_list_tail() {
    ste_list* formal_list_tail = nullptr;
    
    if (token->type == LX_RPAREN) {
        match(LX_RPAREN);
        formal_list_tail =new ste_list();
    }
    
    else {
        ste_list* formals;
        formals = parse_formals();
        match(LX_RPAREN);
        formal_list_tail = formals;
    }
    
    return formal_list_tail;
}


ste_list* Parser::parse_formals() {
    ste_list* formals = new ste_list();
    j_type type;
    string id = token->stringValue;
    match(LX_IDENTIFIER);
    match(LX_COLON);
    type = parse_type();
    formals->head=global->getHead()->putSymbol(id,STE_VAR, type);
    formals->head->offset=offset++;
    numberOfFormal++;
    formals->next=new ste_list();
    parse_formals_tail(formals->next);
    return formals;
}


ste_list* Parser::parse_formals_tail(ste_list* formals) {
    ste_list* formals_tail = nullptr;
    
    if (token->type == LX_COMMA) {
        formals_tail = formals;
        j_type type;
        match(LX_COMMA);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON);
        type = parse_type();
        formals->head=global->getHead()->putSymbol(id,STE_VAR, type);
        formals->head->offset=offset++;
        numberOfFormal++;
        formals_tail->next=new ste_list();
        parse_formals_tail(formals_tail->next);
    }
    return formals_tail;
}

AST* Parser::parse_stmt() {
    AST* stmt = nullptr;
    
    if (token->type == LX_IDENTIFIER) {
        string varName;
        varName = token->stringValue;
        match(LX_IDENTIFIER);
        SymbolTableEntry* entry;
        entry = global->getMySymbol(varName);
        if (entry == nullptr) fatal_error(fileDescriptor, "use undefined var ");
        stmt = parse_id_tail(entry);
    }
    
    else if (token->type == KW_IF) {
        AST* expr, * stmt_tail = nullptr;
        match(KW_IF);
        expr = parse_expr();
        if (!expr )fatal_error(fileDescriptor, "invalid condition in if statement");
        match(KW_THEN);
        stmt = parse_stmt();
        stmt_tail = parse_stmt_tail();
        stmt = make_ast_node(AST_IF, expr, stmt, stmt_tail);
        semanticChecker->check_Statement(stmt,TYPE_BOOLEAN);
    }
    
    
    else if (token->type == KW_WHILE) {
        AST* expr = nullptr;
        match(KW_WHILE);
        expr = parse_expr();
        if (!expr )fatal_error(fileDescriptor, "invalid condition in while statement");
        match(KW_DO);
        stmt = parse_stmt();
        match(KW_OD);
        stmt = make_ast_node(AST_WHILE, expr, stmt);
        semanticChecker->check_Statement(stmt,TYPE_BOOLEAN);
        
    }
    else if (token->type == KW_FOR) {
        AST* expr_lower_bound = nullptr;
        AST* expr_upper_bound = nullptr;
        match(KW_FOR);
        string id=token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON_EQ);
        SymbolTableEntry * var  = global->getMySymbol(id);
        if (var == nullptr) fatal_error(fileDescriptor, "use undefined var ");
        expr_lower_bound = parse_expr();
        if (!expr_lower_bound )fatal_error(fileDescriptor, "invalid lower_bound of for statmnte");
        match(KW_TO);
        expr_upper_bound = parse_expr();
        if (!expr_upper_bound )fatal_error(fileDescriptor, "invalid upper_bound of for statmnte");
        match(KW_DO);
        stmt = parse_stmt();
        match(KW_OD);
        stmt = make_ast_node(AST_FOR,var,expr_lower_bound,expr_upper_bound, stmt);
        semanticChecker->check_Statement(stmt,TYPE_INTEGER);
    }
    
    else if (token->type == KW_READ) {
        SymbolTableEntry* entry;
        match(KW_READ);
        match(LX_LPAREN);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_RPAREN);
        entry = global->getMySymbol(id);
        if (entry == nullptr) fatal_error(fileDescriptor, "use undefined var ");
        else stmt = make_ast_node(AST_READ, entry);
        semanticChecker->check_Statement(stmt,TYPE_NONE);
    }
    
    else if (token->type == KW_WRITE) {
        SymbolTableEntry* entry;
        match(KW_WRITE);
        match(LX_LPAREN);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_RPAREN);
        entry = global->getMySymbol(id);
        if (entry == nullptr) fatal_error(fileDescriptor, "use undefined var ");
        else stmt = make_ast_node(AST_WRITE, entry);
    }
    else if (token->type == KW_RETURN) {
        AST* expr = nullptr;
        match(KW_RETURN);
        match(LX_LPAREN);
        expr = parse_expr();
        match(LX_RPAREN);
        stmt = make_ast_node(AST_RETURN, expr);
    }
    else if (token->type == KW_BEGIN) {
        
        AST* block;
        global->enterScope();
        block = parse_block();
        global->exitScope();
        stmt = block;
        
    }
    
    else fatal_error(fileDescriptor, "invalid statement");
    
    return stmt;
}


AST* Parser::parse_id_tail(SymbolTableEntry* identifier) {
    
    AST* id_tail = nullptr;
    
    if (token->type == LX_COLON_EQ) {
        AST* expr;
        match(LX_COLON_EQ);
        expr=parse_expr();
        if (!expr )fatal_error(fileDescriptor, "invalid assign statement");
        j_type expectedType =identifier->getType();
        id_tail = make_ast_node(AST_ASSIGN, identifier, expr);
        semanticChecker->check_Statement(id_tail,expectedType);

    }
    else {
        ast_list* argList;
        argList = parse_arg_list();
        id_tail = make_ast_node(AST_CALL, identifier, argList);
        j_type expectedType= identifier->getType();
        semanticChecker->check_Statement(id_tail,expectedType);
    }
    
    return id_tail;
    
}



AST* Parser::parse_stmt_tail() {
    AST* stmt_tail = nullptr;
    if (token->type == KW_FI) {
        match(KW_FI);
    }
    else if (token->type == KW_ELSE) {
        match(KW_ELSE);
        stmt_tail = parse_stmt();
        match(KW_FI);
    }
    
    return stmt_tail;
}



AST* Parser::parse_block() {
    AST* block = nullptr;
    ste_list* var_decl_list = new ste_list();
    ast_list* stmt_list;
    match(KW_BEGIN);
    parse_var_decl_list(var_decl_list);
    stmt_list =new ast_list();
    parse_stmt_list(stmt_list);
    match(KW_END);
    block = make_ast_node(AST_BLOCK, var_decl_list,stmt_list);
    return block;
}


ste_list* Parser::parse_var_decl_list(ste_list* s) {
    ste_list* var_decl_list = nullptr;
    if (token->type == KW_VAR) {
        var_decl_list = s;
        SymbolTableEntry* entry = new SymbolTableEntry();
        entry = parse_var_decl();
        var_decl_list->head = entry;
        var_decl_list->head->offset=offset++;
        var_decl_list->next=new ste_list();
        match(LX_SEMICOLON);
        parse_var_decl_list(var_decl_list->next);
    }
    
    return var_decl_list;
}


SymbolTableEntry* Parser::parse_var_decl() {
    j_type  type;
    SymbolTableEntry* var_decl = nullptr;
    string varName;
    match(KW_VAR);
    varName = token->stringValue;
    match(LX_IDENTIFIER);
    match(LX_COLON);
    type = parse_type();
    var_decl = global->getHead()->putSymbol(varName, STE_VAR, type);
    if (var_decl == nullptr) fatal_error(fileDescriptor, "already definded");
    return var_decl;
}


ast_list* Parser::parse_stmt_list(ast_list* s) {
    
    ast_list* stmt_list = nullptr;
    if (first_of_stmt()) {
        stmt_list = s;
        AST* stmt;
        stmt = parse_stmt();
        match(LX_SEMICOLON);
        stmt_list->head = stmt;
        stmt_list->next=new ast_list();
        parse_stmt_list(stmt_list->next);
    }
    return stmt_list;
}




ast_list* Parser::parse_arg_list() {
    ast_list* arg_list = nullptr;
    match(LX_LPAREN);
    arg_list = parse_arg_list_tail();
    return arg_list;
}


ast_list* Parser::parse_arg_list_tail() {
    ast_list * arg_list_tail =new ast_list();
    if (token->type == LX_RPAREN)
        match(LX_RPAREN);
    else {
        parse_args(arg_list_tail);
        match(LX_RPAREN);
    }
    
    return arg_list_tail;
    
}


ast_list* Parser::parse_args(ast_list* s) {
    ast_list* args = s;
    AST* expr;
    expr = parse_expr();
    if (!expr )fatal_error(fileDescriptor, "invalid arg");
    args->head = expr;
    args->next=new ast_list();
    parse_args_tail(args->next);
    return args;
}


AST* Parser::parse_args_tail(ast_list* s) {
    AST* args_tail = nullptr;
    if (token->type == LX_COMMA) {
        match(LX_COMMA);
        parse_args(s);
    }
    
    return args_tail;
}


AST* Parser::parse_expr() {
    AST* expr1 = nullptr;
    AST* expr_tail = nullptr;
    expr1 = parse_expr1();
    expr_tail = parse_expr_tail(expr1);
    return expr_tail;
}


AST* Parser::parse_expr_tail(AST*expr1) {
    Lexeme t=token->type;
    if (t== KW_AND or t == KW_OR) {
        AST* rel_conj, * expr_tail;
        rel_conj = parse_rel_conj();
        expr_tail = parse_expr1();
        if (!expr1 or !expr_tail)fatal_error(fileDescriptor, "invalid expretion");
        if(t==KW_AND)expr1=make_ast_node(AST_AND, expr1,expr_tail);
        else expr1=make_ast_node(AST_OR, expr1,expr_tail);
        expr1=parse_expr_tail(expr1);
        
    }
    return expr1;
}


AST* Parser::parse_expr1() {
    AST* expr2 = nullptr;
    AST* expr1_tail = nullptr;
    expr2 = parse_expr2();
    expr1_tail = parse_expr1_tail(expr2);
    return expr1_tail;
}



AST* Parser::parse_expr1_tail(AST* expr2) {
    AST* expr1_tail = nullptr;
    Lexeme t=token->type;
    if (is_rel_op()) {
        AST* rel_op;
        rel_op = parse_rel_op();
        expr1_tail = parse_expr2();
        if (!expr2 or !expr1_tail)fatal_error(fileDescriptor, "invalid expretion");
        if (t == LX_EQ)expr2=make_ast_node(AST_EQ, expr2,expr1_tail);
        else if (t == LX_NEQ)expr2=make_ast_node(AST_NEQ, expr2,expr1_tail);
        else if (t == LX_LT)expr2=make_ast_node(AST_LT, expr2,expr1_tail);
        else if (t == LX_LE)expr2=make_ast_node(AST_LE, expr2,expr1_tail);
        else if (t == LX_GT)expr2=make_ast_node(AST_GT, expr2,expr1_tail);
        else if (t == LX_GE)expr2=make_ast_node(AST_GE, expr2,expr1_tail);
        expr2=parse_expr1_tail(expr2);
    }
    
    return expr2;
}


AST* Parser::parse_expr2() {
    AST* expr3;
    AST* expr2_tail;
    expr3 = parse_expr3();
    expr2_tail = parse_expr2_tail(expr3);
    return expr2_tail;
}


AST* Parser::parse_expr2_tail(AST* expr3) {
    AST* expr2_tail = nullptr;
    Lexeme t=token->type;
    
    if (t == LX_PLUS or t == LX_MINUS) {
        AST* arith_tail= nullptr;
        arith_tail = parse_arith_op_tail1();
        expr2_tail = parse_expr3();
        if (!expr3 or !expr2_tail)fatal_error(fileDescriptor, "invalid expretion");
        if (t == LX_PLUS)expr3=make_ast_node(AST_PLUS, expr3,expr2_tail);
        else expr3=make_ast_node(AST_MINUS, expr3,expr2_tail);
        expr3 =parse_expr2_tail(expr3);
    }
    
    return expr3;
}




AST* Parser::parse_expr3() {
    AST* expr4 = nullptr;
    AST* expr3_tail = nullptr;
    expr4 = parse_expr4();
    expr3_tail = parse_expr3_tail(expr4);
    return expr3_tail;
}


AST* Parser::parse_expr3_tail(AST * expr4) {
    AST* expr3_tail = nullptr;
    AST* arith_tail2=nullptr;
    Lexeme t=token->type;
    if (t == LX_STAR or t == LX_SLASH) {
        arith_tail2 = parse_arith_op_tail2();
        expr3_tail = parse_expr4();
        if (!expr4 or !expr3_tail)fatal_error(fileDescriptor, "invalid expretion");
        if (t == LX_STAR)expr4=make_ast_node(AST_TIMES, expr4,expr3_tail);
        else expr4=make_ast_node(AST_DIVIDE, expr4,expr3_tail);
        expr4=parse_expr3_tail(expr4);
        
    }
    return expr4;
}



AST* Parser::parse_expr4() {
    AST* expr4 = nullptr;
    Lexeme t=token->type;
    AST* expr5;
    if (token->type == LX_MINUS || token->type == KW_NOT) {
        AST* unary;
        unary = parse_unary_op();
        expr4 = parse_expr4();
        if (!expr4 )fatal_error(fileDescriptor, "invalid expretion");
        if (t == LX_MINUS)expr4=make_ast_node(AST_UMINUS, expr4);
        else expr4=make_ast_node(AST_NOT, expr4);
    }
    else {
        expr5 = parse_expr5();
        expr4=expr5;
    }
    
    return expr4;
}


AST* Parser::parse_expr5() {
    AST* expr5 = nullptr;
    if (token->type == LX_IDENTIFIER) {
        ast_list* expr_id_tail;
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        SymbolTableEntry* entry;
        entry = global->getMySymbol(id);
        if (entry == nullptr) fatal_error(fileDescriptor, "use undefined var ");
        else {
            expr_id_tail = parse_expr_id_tail();
            
            if (expr_id_tail == nullptr) {
                if(entry->type == STE_ROUTINE )fatal_error(fileDescriptor,entry->name+ "use undefined var ");
                expr5 = make_ast_node(AST_VAR, entry);
            }
            else {
                
                if(entry->type==STE_VAR or entry->type==STE_CONST )
                    fatal_error(fileDescriptor,entry->name+ " is not a fuction ");
                
                if(entry->type==STE_ROUTINE  and entry->getType()==TYPE_NONE)
                    fatal_error(fileDescriptor, "procedure not return a value");
                
                expr5 = make_ast_node(AST_CALL, entry, expr_id_tail);
                if(entry->type==STE_ROUTINE )
                    semanticChecker->check_Statement(expr5,TYPE_NONE);
                
            }
            
            
        }
    }
    
    else if (token->type == LX_INTEGER) {
        int value = token->intValue;
        match(LX_INTEGER);
        expr5 = make_ast_node(AST_INTEGER, value);
    }
    else if (token->type == LX_FLOAT) {
        float value = token->floatValue;
        match(LX_FLOAT);
        expr5 = make_ast_node(AST_FLOAT, value);
    }
    
    else if (token->type == LX_STRING) {
        string value = token->stringValue;
        match(LX_STRING);
        expr5 = make_ast_node(AST_STRING, value.c_str());
    }
    else if (token->type == KW_TRUE) {
        match(KW_TRUE);
        expr5 = make_ast_node(AST_BOOLEAN, true);
    }
    else if (token->type == KW_FALSE) {
        match(KW_FALSE);
        expr5 = make_ast_node(AST_BOOLEAN, false);
    }
    else if (token->type == LX_LPAREN) {
        match(LX_LPAREN);
        expr5 = parse_expr();
        match(LX_RPAREN);
    }
    
    return expr5;
}



ast_list* Parser::parse_expr_id_tail() {
    ast_list* expr_id_tail = nullptr;
    if (token->type == LX_LPAREN) {
        expr_id_tail = parse_arg_list();
    }
    return expr_id_tail;
}


AST* Parser::parse_arith_op_tail1() {
    AST* arith_op_tail1 = nullptr;
    if (token->type == LX_PLUS) match(LX_PLUS);
    else if (token->type == LX_MINUS) match(LX_MINUS);
    
    return arith_op_tail1;
}


AST* Parser::parse_arith_op_tail2() {
    AST* arith_op_tail2 = nullptr;
    if (token->type == LX_STAR) match(LX_STAR);
    else if (token->type == LX_SLASH) match(LX_SLASH);
    
    return arith_op_tail2;
    
}


AST* Parser::parse_rel_op() {
    AST* rel_op = nullptr;
    if (token->type == LX_EQ) match(LX_EQ);
    else if (token->type == LX_NEQ) match(LX_NEQ);
    else if (token->type == LX_LT) match(LX_LT);
    else if (token->type == LX_LE) match(LX_LE);
    else if (token->type == LX_GT) match(LX_GT);
    else if (token->type == LX_GE) match(LX_GE);
    
    return rel_op;
}


AST* Parser::parse_rel_conj() {
    AST* rel_conj = nullptr;
    if (token->type == KW_AND) match(KW_AND);
    else if (token->type == KW_OR) match(KW_OR);
    
    return rel_conj;
}


AST* Parser::parse_unary_op() {
    AST* unary_op = nullptr;
    if (token->type == LX_MINUS) match(LX_MINUS);
    else if (token->type == KW_NOT) match(KW_NOT);
    
    return unary_op;
}


void Parser::print(ast_list* s) {
    while (s->next != NULL) {
        print_ast_node(fout, s->head);
        s = s->next;
    }
}


bool Parser::first_of_stmt() {
    if (token->type == LX_IDENTIFIER
        or token->type == KW_IF
        or token->type == KW_WHILE
        or token->type == KW_FOR
        or token->type == KW_READ
        or token->type == KW_WRITE
        or token->type == KW_RETURN
        or token->type == KW_BEGIN)
        return true;
    return false;
}

bool Parser::is_rel_op() {
    if (token->type == LX_EQ
        or token->type == LX_NEQ
        or token->type == LX_LT
        or token->type == LX_LE
        or token->type == LX_GT
        or token->type == LX_GE)
        return true;
    return false;
}


Parser::~Parser() {
    fout.close();
    delete token;
    delete scanner;
}













