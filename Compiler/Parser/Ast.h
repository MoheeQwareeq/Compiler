//
//  AST.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 08/08/2023.
//

#ifndef AST_h
#define AST_h
#include "SymbolTable.h"
#include "FileDescriptor.h"

/* Definitions of list datatypes */
/* List of AST nodes */

struct Ast;

struct AstList      //node to builde ast trees
{
    Ast * head;        // pointer to  a tree
    AstList   * next; // this is the next pointer that you use typically in the list
    J_TYPE type;
};



struct SteList  /* List of symbol table entries */
{
    SymbolTableEntry * head;    // pointer to a symbol table entry
    SteList  * next;   //  this is the next pointer that you use typically in the list
};


enum AST_type{
    AST_VAR_DECL,        /* Top-level variable declaration */
    AST_CONST_DECL,      /*constant declaration */
    AST_ROUTINE_DECL,    /*procedure or function decl. */
    AST_ASSIGN,          /* Assignment statement */
    AST_IF,              /* If-then-else statement */
    AST_WHILE,           /* While statement */
    AST_FOR,             /* For statement */
    AST_READ,            /* Read statement */
    AST_WRITE,           /* Write statement */
    AST_CALL,            /* Procedure call */
    AST_BLOCK,           /* Begin-end block */
    AST_RETURN,          /* Return statement */
    AST_VAR,             /* Variable reference */
    AST_INTEGER,         /* Integer literal */
    AST_STRING,          /* String literal */
    AST_BOOLEAN,         /* Boolean literal */
    AST_TIMES,           /*  operator */
    AST_DIVIDE,          /* / operator */
    AST_PLUS,            /* + operator */
    AST_MINUS,           /* -operator */
    AST_EQ,              /* = operator */
    AST_NEQ,             /* != operator */
    AST_LT,              /* < operator */
    AST_LE,              /* <= operator */
    AST_GT,              /* > operator */
    AST_GE,              /* >= operator */
    AST_AND,             /* and operator */
    AST_OR,              /* or operator */
    AST_NOT,             /* not operator */
    AST_UMINUS,          /* unary -operator */
    AST_EOF,             /* End of File */
    AST_FLOAT,           /* float */
};




struct Ast{
    
    AST_type  type; /* Type of the AST node */
    
    union {  /* The fields depend on a node's type */
        
        struct{
            SymbolTableEntry * name; /* Variable's symbol table entry */
            J_TYPE type; /* Declared type of var*iable */
        } aVarDecl;
        
        
        struct{
            SymbolTableEntry * name; /* Constant's symbol table entry */
            int value; /* Evaluated value of constant */
        } aConstDecl;
        
        
        struct{
            SymbolTableEntry * name; /* Routine's symbol table entry */
            SteList * formals; /* List of formal parameters */
            J_TYPE resultType; /* Type of result (none_type for procedures) */
            Ast * body; /* Body of routine */
            int num;
            int numOfFormal;
        } aRoutineDecl;
        
        
        struct{
            SymbolTableEntry  * lhs; /* Target of assignment */
            Ast * rhs; /* Right side of assignment */
            J_TYPE rightType; //its real type that in right
        } aAssign;
        
        
        struct{
            Ast * predicate; /* Predicate */
            Ast * conseq; /* Consequent */
            Ast * altern; /* Alternative (nullptr for if-then stmt) */
        } aIf;
        
        
        struct{
            Ast * predicate; /* Predicate */
            Ast * body; /* Body */
        } aWhile;
        
        struct {
            SymbolTableEntry * var; /* Index variable */
            Ast * lowerBound; /* Lower iteration bound */
            Ast * upperBound; /* Upper iteration bound */
            Ast * body; /* Body */
        }aFor;
        
        
        struct{
            SymbolTableEntry * var; /* Target variable */
        } aRead;
        
        
        struct{
            SymbolTableEntry * var; /* Source variable */
        } aWrite;
        
        
        struct{
            SymbolTableEntry * callee; /* Symbol table entry for function */
            AstList * argList; /* List of actual arguments */
        } aCall;
        
        
        struct{
            SteList * vars; /* Symbol table entries of local variables */
            AstList * stmts; /* Statements in block */
        } aBlock;
        
        
        struct{
            Ast * expr; /* Return value */
            J_TYPE realReturnType; //its real type that return
            J_TYPE expectReturnType; //its real type that return
        } aReturn;
        
        
        struct{
            SymbolTableEntry * var; /* Symbol table entry for variable */
        } aVar;
        
        
        struct{
            int int_value; /* Integer value */
        } aInteger;
        
        
        struct{
            string stringValue; /* String value */
        } aString;
        
        
        struct{
            bool value; /* Boolean value */
        }aBoolean;
        
        
        struct{
            Ast * arg; /* Argument */
            J_TYPE type;
        } aUnaryOp;
        
        
        struct{
            Ast * larg; /* Argument 1 */
            Ast * rarg; /* Argument 2 */
            J_TYPE l_type;
            J_TYPE r_type;
            J_TYPE rel_type;
        } aBinaryOp;
        
        
        struct{
            float floatValue;
        } aFloat;
        
    };// union
    
}; // AST structure



/* Externally-visible functions: */
Ast * makeAstNode (AST_type type, ...);
void printAstNode (ofstream &fout, Ast *);
AstList * consAst (Ast *, AstList *);
SteList * consSte (SymbolTableEntry *, SteList *);
float evalAstExpr (ofstream & fout,FileDescriptor*,Ast *);
void fatalError(FileDescriptor *, string ) ;




#endif /* AST_h */

