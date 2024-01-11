//
//  Parser.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//


#include "Parser.h"


Parser::Parser(FileDescriptor & fileDescriptor, SymbolTableList & global) {
    this->fileDescriptor = & fileDescriptor;
    scanner = new Scanner(this->fileDescriptor);
    this->global = & global;
    semanticChecker = new SemanticChecker(this->fileDescriptor);
    codeGenerator = new CodeGenerator();
    fout.open("out.txt");
    offset = 0;
    numberOfFormal = 0;
}

void Parser::match(Lexeme l) {
    if (l == token->type) {
        delete token;
        token = scanner->scan();
        return;
    }
    
    else fatalError(fileDescriptor, "");
}


AstList * Parser::parse() {
    token = scanner->scan();
    return parseProgram();
}


AstList * Parser::parseProgram() {
    AstList * program;
    program = parseDeclList(new AstList());
    global->printTable();
    return program;
}


AstList * Parser::parseDeclList(AstList * list) {
    AstList * declList = list;
    
    if (token->type == LX_EOF) {
        Ast * eof;
        eof = makeAstNode(AST_EOF);
        declList->head = eof;               //add EOF_AST to list λ case
        declList->next = nullptr;
        SymbolTableEntry * entry;
        entry = global->getMySymbol("main");

 
        if (!entry or entry->type != STE_ROUTINE) {
            cout << "no main function\n\n";
            remove("out.txt");
            remove("assmbly.asm");
            remove("symbol tabel.txt");
            exit(1);
        }
    }
    
    else {
        Ast * decl;
        decl = parseDecl();
        match(LX_SEMICOLON);
        declList->head = decl;             //add decl to list
        declList->next = new AstList();   //creat new AstList
        parseDeclList(list->next);
    }
    
    return declList;
}


Ast * Parser::parseDecl() {
    Ast * decl = nullptr;
    
    if (token->type == KW_VAR) {
        J_TYPE  type;
        SymbolTableEntry * name;
        string varName;
        match(KW_VAR);
        varName = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON);
        type = parseType();
        name = global->getHead()->putSymbol(varName, STE_VAR, type);
        if (name) decl = makeAstNode(AST_VAR_DECL, name, type);
        else fatalError(fileDescriptor, "name already definded");
        codeGenerator->generate(decl);
    }
    
    
    else if (token->type == KW_CONSTANT) {
        SymbolTableEntry * name;
        int  value;
        string constName;
        match(KW_CONSTANT);
        Ast * expr;
        constName = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_EQ);
        expr = parseExpr();
        if (!expr )
            fatalError(fileDescriptor, "invalid assign for constant");
        value =evalAstExpr(fout,fileDescriptor,expr);
        name = global->getHead()->putSymbol(constName, STE_CONST,TYPE_INTEGER,value);
        if (name)
            decl = makeAstNode(AST_CONST_DECL, name, value);
        
        else fatalError(fileDescriptor, "name already definded");
        codeGenerator->generate(decl);

        
    }
    
    
    
    else if (token->type == KW_FUNCTION) {
        SymbolTableEntry * name;
        string funcName;
        J_TYPE resultType;
        Ast* block = nullptr;
        SteList * formalsList;
        match(KW_FUNCTION);
        funcName = token->stringValue;
        match(LX_IDENTIFIER);
        global->enterScope();
        offset = 0;
        numberOfFormal = 0;
        formalsList = parseFormalList();
        match(LX_COLON);
        resultType = parseType();
        block = parseBlock();
        global->exitScope();
        name = global->getHead()->putSymbol(funcName, STE_ROUTINE, resultType);
        if (name){
            name->routine.formals = formalsList;
            name->routine.formals = formalsList;
            decl = makeAstNode(AST_ROUTINE_DECL, name, formalsList,resultType,block);
            semanticChecker->checkStatement(decl,resultType);
        }
        else
            fatalError(fileDescriptor, "name already definded");
        
        decl->aRoutineDecl.num = offset;
        decl->aRoutineDecl.numOfFormal = numberOfFormal;
        codeGenerator->generate(decl);

    }
    
    else if (token->type == KW_PROCEDURE) {
        SymbolTableEntry * name;
        SteList * formalsList;
        Ast * block;
        match(KW_PROCEDURE);
        string funcName = token->stringValue;
        match(LX_IDENTIFIER);
        global->enterScope();
        offset = 0;
        numberOfFormal = 0;
        formalsList = parseFormalList();
        block = parseBlock();
        global->exitScope();
        name = global->getHead()->putSymbol(funcName, STE_ROUTINE, TYPE_NONE);
        if (name){
            name->routine.formals=formalsList;
            decl = makeAstNode(AST_ROUTINE_DECL, name, formalsList,TYPE_NONE ,block);
            semanticChecker->checkStatement(decl,TYPE_NONE);
        }
        else
            fatalError(fileDescriptor, "name already definded");
        decl->aRoutineDecl.num=offset;
        decl->aRoutineDecl.numOfFormal=numberOfFormal;
        codeGenerator->generate(decl);
    }
    
    else
        fatalError(fileDescriptor, "name already definded");
    
    return decl;
}





J_TYPE Parser::parseType() {
    
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
        fatalError(fileDescriptor, "unknown type");
    
    return TYPE_NONE;
}


SteList * Parser::parseFormalList() {
    SteList * formalList = nullptr;
    SteList * formalListTail = nullptr;
    match(LX_LPAREN);
    formalListTail = parseFormalListTail();
    formalList = formalListTail;
    return formalList;
}


SteList * Parser::parseFormalListTail() {
    SteList * formalListTail = nullptr;
    
    if (token->type == LX_RPAREN) {
        match(LX_RPAREN);
        formalListTail =new SteList();
    }
    
    else {
        SteList* formals;
        formals = parseFormals();
        match(LX_RPAREN);
        formalListTail = formals;
    }
    
    return formalListTail;
}


SteList * Parser::parseFormals() {
    SteList * formals = new SteList();
    J_TYPE type;
    string id = token->stringValue;
    match(LX_IDENTIFIER);
    match(LX_COLON);
    type = parseType();
    formals->head = global->getHead()->putSymbol(id,STE_VAR, type);
    formals->head->offset=offset++;
    numberOfFormal++;
    formals->next = new SteList();
    parseFormalsTail(formals->next);
    return formals;
}


SteList * Parser::parseFormalsTail(SteList* formals) {
    SteList * formalsTail = nullptr;
    
    if (token->type == LX_COMMA) {
        formalsTail = formals;
        J_TYPE type;
        match(LX_COMMA);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON);
        type = parseType();
        formals->head=global->getHead()->putSymbol(id,STE_VAR, type);
        formals->head->offset=offset++;
        numberOfFormal++;
        formalsTail->next=new SteList();
        parseFormalsTail(formalsTail->next);
    }
    return formalsTail;
}

Ast * Parser::parseStmt() {
    Ast * stmt = nullptr;
    
    if (token->type == LX_IDENTIFIER) {
        string varName;
        varName = token->stringValue;
        match(LX_IDENTIFIER);
        SymbolTableEntry* entry;
        entry = global->getMySymbol(varName);
        if (!entry) fatalError(fileDescriptor, "use undefined var ");
        stmt = parseIdTail(entry);
    }
    
    else if (token->type == KW_IF) {
        Ast * expr, * stmtTail = nullptr;
        match(KW_IF);
        expr = parseExpr();
        if (!expr )fatalError(fileDescriptor, "invalid condition in if statement");
        match(KW_THEN);
        stmt = parseStmt();
        stmtTail = parseStmtTail();
        stmt = makeAstNode(AST_IF, expr, stmt, stmtTail);
        semanticChecker->checkStatement(stmt,TYPE_BOOLEAN);
    }
    
    
    else if (token->type == KW_WHILE) {
        Ast* expr = nullptr;
        match(KW_WHILE);
        expr = parseExpr();
        if (!expr )fatalError(fileDescriptor, "invalid condition in while statement");
        match(KW_DO);
        stmt = parseStmt();
        match(KW_OD);
        stmt = makeAstNode(AST_WHILE, expr, stmt);
        semanticChecker->checkStatement(stmt,TYPE_BOOLEAN);
    }
    
    else if (token->type == KW_FOR) {
        Ast* exprLowerBound = nullptr;
        Ast* exprUpperBound = nullptr;
        match(KW_FOR);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_COLON_EQ);
        SymbolTableEntry * var  = global->getMySymbol(id);
        if (!var)
            fatalError(fileDescriptor, "use undefined var ");
        exprLowerBound = parseExpr();
        if (!exprLowerBound )
            fatalError(fileDescriptor, "invalid lower bound of for statmnte");
        match(KW_TO);
        exprUpperBound = parseExpr();
        if (!exprUpperBound )
            fatalError(fileDescriptor, "invalid upper bound of for statmnte");
        match(KW_DO);
        stmt = parseStmt();
        match(KW_OD);
        stmt = makeAstNode(AST_FOR,var,exprLowerBound,exprUpperBound, stmt);
        semanticChecker->checkStatement(stmt,TYPE_INTEGER);
    }
    
    else if (token->type == KW_READ) {
        SymbolTableEntry* entry;
        match(KW_READ);
        match(LX_LPAREN);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_RPAREN);
        entry = global->getMySymbol(id);
        if (!entry)
            fatalError(fileDescriptor, "use undefined var ");
        else stmt = makeAstNode(AST_READ, entry);
        semanticChecker->checkStatement(stmt,TYPE_NONE);
    }
    
    else if (token->type == KW_WRITE) {
        SymbolTableEntry* entry;
        match(KW_WRITE);
        match(LX_LPAREN);
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        match(LX_RPAREN);
        entry = global->getMySymbol(id);
        if (!entry) fatalError(fileDescriptor, "use undefined var ");
        else stmt = makeAstNode(AST_WRITE, entry);
    }
    else if (token->type == KW_RETURN) {
        Ast* expr = nullptr;
        match(KW_RETURN);
        match(LX_LPAREN);
        expr = parseExpr();
        match(LX_RPAREN);
        stmt = makeAstNode(AST_RETURN, expr);
    }
    else if (token->type == KW_BEGIN) {
        Ast* block;
        global->enterScope();
        block = parseBlock();
        global->exitScope();
        stmt = block;
    }
    
    else
        fatalError(fileDescriptor, "invalid statement");
    
    return stmt;
}


Ast * Parser::parseIdTail(SymbolTableEntry* identifier) {
    
    Ast * idTail = nullptr;
    
    if (token->type == LX_COLON_EQ) {
        Ast* expr;
        match(LX_COLON_EQ);
        expr=parseExpr();
        if (!expr )fatalError(fileDescriptor, "invalid assign statement");
        J_TYPE expectedType =identifier->getType();
        idTail = makeAstNode(AST_ASSIGN, identifier, expr);
        semanticChecker->checkStatement(idTail,expectedType);
    }
    
    else {
        AstList * argList;
        argList = parseArgList();
        idTail = makeAstNode(AST_CALL, identifier, argList);
        J_TYPE expectedType= identifier->getType();
        semanticChecker->checkStatement(idTail,expectedType);
    }
    return idTail;
}



Ast * Parser::parseStmtTail() {
    Ast * stmtTail = nullptr;
    if (token->type == KW_FI)
        match(KW_FI);
    else if (token->type == KW_ELSE) {
        match(KW_ELSE);
        stmtTail = parseStmt();
        match(KW_FI);
    }
    
    return stmtTail;
}



Ast * Parser::parseBlock() {
    Ast* block = nullptr;
    SteList * varDeclList = new SteList();
    AstList * stmtList;
    match(KW_BEGIN);
    parseVarDeclList(varDeclList);
    stmtList =new AstList();
    parseStmtList(stmtList);
    match(KW_END);
    block = makeAstNode(AST_BLOCK, varDeclList,stmtList);
    return block;
}


SteList* Parser::parseVarDeclList(SteList* s) {
    SteList * varDeclList = nullptr;
    if (token->type == KW_VAR) {
        varDeclList = s;
        SymbolTableEntry* entry = new SymbolTableEntry();
        entry = parseVarDecl();
        varDeclList->head = entry;
        varDeclList->head->offset=offset++;
        varDeclList->next = new SteList();
        match(LX_SEMICOLON);
        parseVarDeclList(varDeclList->next);
    }
    
    return varDeclList;
}


SymbolTableEntry * Parser::parseVarDecl() {
    J_TYPE  type;
    SymbolTableEntry * varDecl = nullptr;
    string varName;
    match(KW_VAR);
    varName = token->stringValue;
    match(LX_IDENTIFIER);
    match(LX_COLON);
    type = parseType();
    varDecl = global->getHead()->putSymbol(varName, STE_VAR, type);
    if (!varDecl) fatalError(fileDescriptor, "already definded");
    return varDecl;
}


AstList* Parser::parseStmtList(AstList* s) {
    
    AstList * stmtList = nullptr;
    if (firstOfStmt()) {
        stmtList = s;
        Ast * stmt;
        stmt = parseStmt();
        match(LX_SEMICOLON);
        stmtList->head = stmt;
        stmtList->next=new AstList();
        parseStmtList(stmtList->next);
    }
    return stmtList;
}




AstList* Parser::parseArgList() {
    AstList * argList = nullptr;
    match(LX_LPAREN);
    argList = parseArgListTail();
    return argList;
}


AstList* Parser::parseArgListTail() {
    AstList * argListTail =new AstList();
    if (token->type == LX_RPAREN)
        match(LX_RPAREN);
    else {
        parseArgs(argListTail);
        match(LX_RPAREN);
    }
    
    return argListTail;
    
}


AstList * Parser::parseArgs(AstList * s) {
    AstList * args = s;
    Ast * expr;
    expr = parseExpr();
    if (!expr )fatalError(fileDescriptor, "invalid arg");
    args->head = expr;
    args->next=new AstList();
    parseArgsTail(args->next);
    return args;
}


Ast* Parser::parseArgsTail(AstList * s) {
    Ast * argsTail = nullptr;
    if (token->type == LX_COMMA) {
        match(LX_COMMA);
        parseArgs(s);
    }
    
    return argsTail;
}


Ast* Parser::parseExpr() {
    Ast * expr1 = nullptr;
    Ast * exprTail = nullptr;
    expr1 = parseExpr1();
    exprTail = parseExprTail(expr1);
    return exprTail;
}


Ast * Parser::parseExprTail(Ast * expr1) {
    Lexeme t=token->type;
    if (t == KW_AND or t == KW_OR) {
        Ast* relConj, * exprTail;
        relConj = parseRelConj();
        exprTail = parseExpr1();
        if (!expr1 or !exprTail)
            fatalError(fileDescriptor, "invalid expretion");
        if(t==KW_AND)
            expr1=makeAstNode(AST_AND, expr1,exprTail);
        else
            expr1=makeAstNode(AST_OR, expr1,exprTail);
        
        expr1=parseExprTail(expr1);
    }
    return expr1;
}


Ast* Parser::parseExpr1() {
    Ast * expr2 = nullptr;
    Ast * expr1Tail = nullptr;
    expr2 = parseExpr2();
    expr1Tail = parseExpr1Tail(expr2);
    return expr1Tail;
}



Ast * Parser::parseExpr1Tail(Ast * expr2) {
    Ast * expr1Tail = nullptr;
    Lexeme t = token->type;
    if (isRelOp()) {
        Ast * relOp;
        relOp = parseRelOp();
        expr1Tail = parseExpr2();
        if (!expr2 or !expr1Tail)fatalError(fileDescriptor, "invalid expretion");
        if (t == LX_EQ)expr2=makeAstNode(AST_EQ, expr2,expr1Tail);
        else if (t == LX_NEQ)
            expr2=makeAstNode(AST_NEQ, expr2,expr1Tail);
        else if (t == LX_LT)
            expr2=makeAstNode(AST_LT, expr2,expr1Tail);
        else if (t == LX_LE)
            expr2=makeAstNode(AST_LE, expr2,expr1Tail);
        else if (t == LX_GT)
            expr2=makeAstNode(AST_GT, expr2,expr1Tail);
        else if (t == LX_GE)
            expr2=makeAstNode(AST_GE, expr2,expr1Tail);
        
        expr2=parseExpr1Tail(expr2);
    }
    
    return expr2;
}


Ast* Parser::parseExpr2() {
    Ast * expr3;
    Ast * expr2Tail;
    expr3 = parseExpr3();
    expr2Tail = parseExpr2Tail(expr3);
    return expr2Tail;
}


Ast* Parser::parseExpr2Tail(Ast* expr3) {
    Ast * expr2Tail = nullptr;
    Lexeme t = token->type;
    
    if (t == LX_PLUS or t == LX_MINUS) {
        Ast * arithTail= nullptr;
        arithTail = parseArithOpTail1();
        expr2Tail = parseExpr3();
        if (!expr3 or !expr2Tail)fatalError(fileDescriptor, "invalid expretion");
        if (t == LX_PLUS)expr3=makeAstNode(AST_PLUS, expr3,expr2Tail);
        else expr3=makeAstNode(AST_MINUS, expr3,expr2Tail);
        expr3 =parseExpr2Tail(expr3);
    }
    
    return expr3;
}




Ast* Parser::parseExpr3() {
    Ast * expr4 = nullptr;
    Ast * expr3Tail = nullptr;
    expr4 = parseExpr4();
    expr3Tail = parseExpr3Tail(expr4);
    return expr3Tail;
}


Ast * Parser::parseExpr3Tail(Ast * expr4) {
    Ast * expr3Tail = nullptr;
    Ast * arithTail2=nullptr;
    Lexeme t = token->type;
    if (t == LX_STAR or t == LX_SLASH) {
        arithTail2 = parseArithOpTail2();
        expr3Tail = parseExpr4();
        if (!expr4 or !expr3Tail)fatalError(fileDescriptor, "invalid expretion");
        if (t == LX_STAR)expr4=makeAstNode(AST_TIMES, expr4,expr3Tail);
        else expr4=makeAstNode(AST_DIVIDE, expr4,expr3Tail);
        expr4=parseExpr3Tail(expr4);
    }
    return expr4;
}



Ast * Parser::parseExpr4() {
    Ast * expr4 = nullptr;
    Lexeme t = token->type;
    Ast * expr5;
    if (token->type == LX_MINUS || token->type == KW_NOT) {
        Ast* unary;
        unary = parseUnaryOp();
        expr4 = parseExpr4();
        if (!expr4 )fatalError(fileDescriptor, "invalid expretion");
        if (t == LX_MINUS)expr4=makeAstNode(AST_UMINUS, expr4);
        else expr4=makeAstNode(AST_NOT, expr4);
    }
    else {
        expr5 = parseExpr5();
        expr4=expr5;
    }
    
    return expr4;
}


Ast * Parser::parseExpr5() {
    Ast * expr5 = nullptr;
    if (token->type == LX_IDENTIFIER) {
        AstList * exprIdTail;
        string id = token->stringValue;
        match(LX_IDENTIFIER);
        SymbolTableEntry* entry;
        entry = global->getMySymbol(id);
        if (!entry) fatalError(fileDescriptor, "use undefined var ");
        else {
            exprIdTail = parseExprIdTail();
            
            if (!exprIdTail) {
                if(entry->type == STE_ROUTINE )
                    fatalError(fileDescriptor,entry->name+ "use undefined var ");
                expr5 = makeAstNode(AST_VAR, entry);
            }
            else {
                
                if(entry->type==STE_VAR or entry->type==STE_CONST )
                    fatalError(fileDescriptor,entry->name+ " is not a fuction ");
                
                if(entry->type==STE_ROUTINE  and entry->getType()==TYPE_NONE)
                    fatalError(fileDescriptor, "procedure not return a value");
                
                expr5 = makeAstNode(AST_CALL, entry, exprIdTail);
                
                if(entry->type==STE_ROUTINE )
                    semanticChecker->checkStatement(expr5,TYPE_NONE);
            }
        }
    }
    
    else if (token->type == LX_INTEGER) {
        int value = token->intValue;
        match(LX_INTEGER);
        expr5 = makeAstNode(AST_INTEGER, value);
    }
    else if (token->type == LX_FLOAT) {
        float value = token->floatValue;
        match(LX_FLOAT);
        expr5 = makeAstNode(AST_FLOAT, value);
    }
    
    else if (token->type == LX_STRING) {
        string value = token->stringValue;
        match(LX_STRING);
        expr5 = makeAstNode(AST_STRING, value.c_str());
    }
    else if (token->type == KW_TRUE) {
        match(KW_TRUE);
        expr5 = makeAstNode(AST_BOOLEAN, true);
    }
    else if (token->type == KW_FALSE) {
        match(KW_FALSE);
        expr5 = makeAstNode(AST_BOOLEAN, false);
    }
    else if (token->type == LX_LPAREN) {
        match(LX_LPAREN);
        expr5 = parseExpr();
        match(LX_RPAREN);
    }
    
    return expr5;
}



AstList * Parser::parseExprIdTail() {
    AstList * exprIdTail = nullptr;
    if (token->type == LX_LPAREN) {
        exprIdTail = parseArgList();
    }
    return exprIdTail;
}


Ast * Parser::parseArithOpTail1() {
    Ast* arithOpTail1 = nullptr;
    if (token->type == LX_PLUS) match(LX_PLUS);
    else if (token->type == LX_MINUS) match(LX_MINUS);
    
    return arithOpTail1;
}


Ast * Parser::parseArithOpTail2() {
    Ast * arithOpTail2 = nullptr;
    if (token->type == LX_STAR) match(LX_STAR);
    else if (token->type == LX_SLASH) match(LX_SLASH);
    
    return arithOpTail2;
    
}


Ast * Parser::parseRelOp() {
    Ast * relOp = nullptr;
    if (token->type == LX_EQ) match(LX_EQ);
    else if (token->type == LX_NEQ) match(LX_NEQ);
    else if (token->type == LX_LT) match(LX_LT);
    else if (token->type == LX_LE) match(LX_LE);
    else if (token->type == LX_GT) match(LX_GT);
    else if (token->type == LX_GE) match(LX_GE);
    
    return relOp;
}


Ast * Parser::parseRelConj() {
    Ast * relConj = nullptr;
    if (token->type == KW_AND) match(KW_AND);
    else if (token->type == KW_OR) match(KW_OR);
    
    return relConj;
}


Ast * Parser::parseUnaryOp() {
    Ast * unaryOp = nullptr;
    if (token->type == LX_MINUS) match(LX_MINUS);
    else if (token->type == KW_NOT) match(KW_NOT);
    
    return unaryOp;
}


void Parser::print(AstList * s) {
    while (s->next) {
        printAstNode(fout, s->head);
        s = s->next;
    }
}


bool Parser::firstOfStmt() {
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

bool Parser::isRelOp() {
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
    delete semanticChecker;
    delete codeGenerator;
}













