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

struct AST;

struct ast_list      //node to builde ast trees
{
    AST * head;        // pointer to  a tree
    ast_list   * next; // this is the next pointer that you use typically in the list
    J_TYPE type;
};



struct ste_list  /* List of symbol table entries */
{
    SymbolTableEntry *head;    // pointer to a symbol table entry
    ste_list  * next;   //  this is the next pointer that you use typically in the list
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




struct AST{
    
    AST_type  type; /* Type of the AST node */
    
    union {  /* The fields depend on a node's type */
        
        struct{
            SymbolTableEntry * name; /* Variable's symbol table entry */
            J_TYPE type; /* Declared type of var*iable */
        } a_var_decl;
        
        
        struct{
            SymbolTableEntry * name; /* Constant's symbol table entry */
            int value; /* Evaluated value of constant */
        } a_const_decl;
        
        
        struct{
            SymbolTableEntry * name; /* Routine's symbol table entry */
            ste_list * formals; /* List of formal parameters */
            J_TYPE resultType; /* Type of result (none_type for procedures) */
            AST * body; /* Body of routine */
            int num;
            int numOfFormal;
        } a_routine_decl;
        
        
        struct{
            SymbolTableEntry  * lhs; /* Target of assignment */
            AST * rhs; /* Right side of assignment */
            J_TYPE rightType; //its real type that in right
        } a_assign;
        
        
        struct{
            AST * predicate; /* Predicate */
            AST * conseq; /* Consequent */
            AST * altern; /* Alternative (nullptr for if-then stmt) */
        } a_if;
        
        
        struct{
            AST * predicate; /* Predicate */
            AST * body; /* Body */
        } a_while;
        
        struct {
            SymbolTableEntry * var; /* Index variable */
            AST * lowerBound; /* Lower iteration bound */
            AST * upperBound; /* Upper iteration bound */
            AST * body; /* Body */
        }a_for;
        
        
        struct{
            SymbolTableEntry * var; /* Target variable */
        } a_read;
        
        
        struct{
            SymbolTableEntry * var; /* Source variable */
        } a_write;
        
        
        struct{
            SymbolTableEntry * callee; /* Symbol table entry for function */
            ast_list * arg_list; /* List of actual arguments */
        } a_call;
        
        
        struct{
            ste_list * vars; /* Symbol table entries of local variables */
            ast_list * stmts; /* Statements in block */
        } a_block;
        
        
        struct{
            AST * expr; /* Return value */
            J_TYPE realReturnType; //its real type that return
            J_TYPE expectReturnType; //its real type that return
        } a_return;
        
        
        struct{
            SymbolTableEntry * var; /* Symbol table entry for variable */
        } a_var;
        
        
        struct{
            int int_value; /* Integer value */
        } a_integer;
        
        
        struct{
            string string_value; /* String value */
        } a_string;
        
        
        struct{
            bool value; /* Boolean value */
        }a_boolean;
        
        
        struct{
            AST * arg; /* Argument */
            J_TYPE type;
        } a_unary_op;
        
        
        struct{
            AST * larg; /* Argument 1 */
            AST * rarg; /* Argument 2 */
            J_TYPE l_type;
            J_TYPE r_type;
            J_TYPE rel_type;
        } a_binary_op;
        
        
        struct{
            float float_value;
        } a_float;
        
    };// union
    
}; // AST structure



/* Externally-visible functions: */
AST * makeAstNode (AST_type type, ...);
void printAstNode (ofstream &fout, AST *);
ast_list * consAst (AST *, ast_list *);
ste_list * consSte (SymbolTableEntry *, ste_list *);
float evalAstExpr (ofstream & fout,FileDescriptor*,AST *);
void fatalError(FileDescriptor *, string ) ;




#endif /* AST_h */

