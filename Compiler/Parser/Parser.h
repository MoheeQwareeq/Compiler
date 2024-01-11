//
//  Parser.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#ifndef Parser_h
#define Parser_h


#include "FileDescriptor.h"
#include "Ast.h"
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
    void match(Lexeme);
    AstList * parseProgram();
    AstList * parseDeclList(AstList *);
    Ast * parseDecl();
    J_TYPE parseType();
    SteList * parseFormalList();
    SteList * parseFormalListTail();
    SteList * parseFormals();
    SteList * parseFormalsTail(SteList *);
    Ast* parseStmt();
    Ast* parseStmtTail();
    Ast* parseBlock();
    SteList * parseVarDeclList(SteList *);
    SymbolTableEntry * parseVarDecl();
    AstList * parseStmtList(AstList *);
    Ast * parseIdTail(SymbolTableEntry *);
    AstList * parseArgList();
    AstList * parseArgListTail();
    AstList * parseArgs(AstList *);
    Ast * parseArgsTail(AstList *);
    Ast * parseExpr();
    Ast * parseExprTail(Ast *);
    Ast * parseExpr1();
    Ast * parseExpr1Tail(Ast *);
    Ast * parseExpr2();
    Ast * parseExpr2Tail(Ast *);
    Ast * parseExpr3();
    Ast * parseExpr3Tail(Ast *);
    Ast * parseExpr4();
    Ast * parseExpr5();
    Ast * parseArithOpTail1();
    Ast * parseArithOpTail2();
    Ast * parseRelOp();
    Ast * parseRelConj();
    Ast * parseUnaryOp();
    bool firstOfStmt();
    bool isRelOp();
    AstList * parseExprIdTail();

public:

    Parser(FileDescriptor & , SymbolTableList &);
    AstList * parse();
    void print(AstList *);
    ~Parser();
};


#endif /* Parser_h */






