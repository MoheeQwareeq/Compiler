//
//  Scanner.h
//  Parser
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#ifndef Scanner_h
#define Scanner_h


#include "FileDescriptor.h"

using namespace std;

typedef enum
{
    /* Literals */
    LX_IDENTIFIER, LX_INTEGER, LX_STRING, LX_FLOAT,
    /* Keywords */
    KW_PROGRAM,
    KW_VAR, KW_CONSTANT, KW_INTEGER, KW_BOOLEAN, KW_STRING, KW_FLOAT,
    KW_TRUE, KW_FALSE, KW_IF, KW_FI, KW_THEN, KW_ELSE,
    KW_WHILE, KW_DO, KW_OD,
    KW_AND, KW_OR,
    KW_READ, KW_WRITE,
    KW_FOR, KW_FROM, KW_TO, KW_BY,
    KW_FUNCTION, KW_PROCEDURE, KW_RETURN, KW_NOT, KW_BEGIN, KW_END,
    /* Operators */
    LX_LPAREN, LX_RPAREN, LX_LBRACKET, LX_RBRACKET,LX_LCURLY,LX_RCURLY,
    LX_COLON, LX_DOT, LX_SEMICOLON, LX_COMMA, LX_COLON_EQ,
    LX_PLUS, LX_MINUS, LX_STAR, LX_SLASH,
    LX_EQ, LX_NEQ, LX_LT, LX_LE, LX_GT, LX_GE, LX_EOF
} LEXEME_TYPE;

const int keys = 32; /* number of keywords */

const string keyword[] = {
    "and","begin","boolean","by","constant",
    "do", "else", "end", "false", "fi", "float", "for", "from",
    "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true",
    "var", "while", "write"
};


static LEXEME_TYPE key_type[] = {
    KW_AND, KW_BEGIN, KW_BOOLEAN, KW_BY, KW_CONSTANT,
    KW_DO, KW_ELSE, KW_END, KW_FALSE, KW_FI, KW_FLOAT,
    KW_FOR, KW_FROM, KW_FUNCTION, KW_IF, KW_INTEGER, KW_NOT,
    KW_OD, KW_OR, KW_PROCEDURE, KW_PROGRAM, KW_READ, KW_RETURN,
    KW_STRING, KW_THEN, KW_TO, KW_TRUE, KW_VAR, KW_WHILE, KW_WRITE
};


struct  TOKEN
{
    LEXEME_TYPE type;
    int    intValue;
    float  floatValue;
    string stringValue;
};

class Scanner{
private:
    FileDescriptor *fileDescriptor;
    TOKEN * getIdentifier(char ch);
    TOKEN * getNumber(char ch);
    TOKEN * getString(char ch);
    TOKEN * skipComments(char ch);//
    bool checkKeyword(string identifier ,int &index);
    bool isOperators(char ch,LEXEME_TYPE &t);
    bool isDelimiter(char ch);
    
public:
    Scanner();
    Scanner (FileDescriptor *fd);
    TOKEN * Scan();
};


#endif /* Scanner_h */
