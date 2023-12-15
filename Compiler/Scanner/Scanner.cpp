//
//  Scanner.cpp
//  Parser
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#include "Scanner.h"

Scanner :: Scanner(FileDescriptor * fd){
    fileDescriptor = fd;
}

Token * Scanner ::scan(){
    Token * temp = new Token;
    Lexeme type;
    
    char ch = fileDescriptor->getChar();
    if (ch=='"')
        return getString(ch);
    
    else if (isalpha(ch) || ch == '_')
        return getIdentifier(ch);
    
    else if (isdigit(ch))
        return getNumber(ch);
    
    else if (ch=='#')
        return skipComments(ch);
    
    else if (isspace(ch))
        return scan();
    
    else if (isOperators(ch,type)){
        temp->type=type;
        return temp;
    }
    
    else if (ch == EOF){
        temp->type=LX_EOF;
        return temp;
    }
    
    else {
        fileDescriptor->reportError("Lexical Error: Illegal in COMP23 programs");
        exit(EXIT_FAILURE);
    }
}

Token * Scanner ::getIdentifier(char ch){
    Token* temp = new Token;
    string str="";
    str += ch;
    int index=0;
    ch= fileDescriptor->getChar();
    while(isalpha(ch) || isdigit(ch) || ch == '_'){
        str += ch;
        ch= fileDescriptor->getChar();
    }
    
    if(checkKeyword(str,index) )
        temp->type=KEY_TYPE[index];
    else {
        temp->type=LX_IDENTIFIER;
        temp->stringValue=str;
    }
    
    if(isDelimiter(ch)){
        fileDescriptor->ungetChar();
        return temp;
    }
    
    fileDescriptor->ungetChar();
    fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  identifier number");
    exit(EXIT_FAILURE);
}


Token * Scanner ::getNumber(char ch){
    Token* temp = new Token;
    string str="";
    
    if(isdigit(ch)){
        while (isdigit(ch)) {str += ch;ch= fileDescriptor->getChar();  }
        int checkOverflow;
        try {
            checkOverflow = stoi(str);
        }
        catch (const out_of_range& e) {
            fileDescriptor->ungetChar();
            fileDescriptor->reportWarning("Overflow occurred");
            exit(EXIT_FAILURE);
        }
        switch (ch){
            case '.':{
                str += ch;
                ch = fileDescriptor->getChar();
                if(isdigit(ch)){
                    while(isdigit(ch)){ str += ch;  ch = fileDescriptor->getChar(); }
                    if(isDelimiter(ch)){
                        fileDescriptor->ungetChar();
                        temp->type=LX_FLOAT;
                        temp->floatValue =   stof(str);
                        return temp;
                    }
                }
                
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  floating number");
                exit(EXIT_FAILURE);
            }
                
            default:{
                if(isDelimiter(ch)){
                    fileDescriptor->ungetChar();
                    temp->type=LX_INTEGER;
                    temp->intValue = stoi(str);
                    return temp;}
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  integer number");
                exit(EXIT_FAILURE);
            }
        }
    }
    return temp;
    
}


Token * Scanner ::getString(char ch){
    Token* temp = new Token;
    string str="";
    ch= fileDescriptor->getChar();
    while(ch != '"' && ch != '\n' ){
        str += ch;
        ch= fileDescriptor->getChar();}
    
    switch (ch){
        case '"':{
            ch= fileDescriptor->getChar();
            if(isDelimiter(ch)){
                fileDescriptor->ungetChar();
                temp->type=LX_STRING;
                temp->stringValue=str;
                return temp;}
            else{
                fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  string");
                exit(EXIT_FAILURE);
            }
        }
        default:{
            fileDescriptor->ungetChar();
            fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  string");
            exit(EXIT_FAILURE);
        }
    }
}

Token * Scanner ::skipComments(char ch){
    
    ch= fileDescriptor->getChar();
    if (ch=='#'){
        ch= fileDescriptor->getChar();
        while(ch != '#' && ch != '\n')
            ch= fileDescriptor->getChar();
        
        switch (ch){
            case '#':{
                ch= fileDescriptor->getChar();
                if (ch=='#')
                    return scan();
                else {
                    fileDescriptor->ungetChar();
                    fileDescriptor->reportError("Lexical Error: Incorrect comments");
                }
            }
            default:
                return scan();
        }
    }
    else {
        fileDescriptor->ungetChar();
        fileDescriptor->reportError("Lexical Error: Incorrect comments");
        exit(EXIT_FAILURE);
    }
}

bool Scanner ::checkKeyword(string str , int &index){
    
    for (int i=0;i <KEYS;i++)
        if(str == KEY_WORD[i]){
            index=i;
            return true;
        }
    return false;
}


bool Scanner ::isDelimiter(char ch) {
    if (isspace(ch))return true;
    switch (ch) {
        case '(': case ')': case '+': case '-': case '*': case '/': case '=':
        case '.': case ';': case '[': case ']': case ',': case '!': case ':':
        case '<': case '>': case '#': case '{': case '}':
            return true;
        default:
            return false;
    }
}


bool Scanner :: isOperators(char ch,Lexeme &type){
    
    switch (ch){
        case'(':{
            type=LX_LPAREN;
            return true;
        }
        case')':{
            type=LX_RPAREN;
            return true;
        }
        case'+':{
            type=LX_PLUS;
            return true;
        }
        case'-':{
            type=LX_MINUS;
            return true;
        }
        case'*':{
            type=LX_STAR;
            return true;
        }
        case'/':{
            type=LX_SLASH;
            return true;
        }
        case'=':{
            type=LX_EQ;
            return true;
        }
        case'.':{
            type=LX_DOT;
            return true;
        }
        case';':{
            type=LX_SEMICOLON;
            return true;
        }
        case'[':{
            type=LX_LBRACKET;
            return true;
        }
        case']':{
            type=LX_RBRACKET;
            return true;
        }
        case',':{
            type=LX_COMMA;
            return true;
        }
        case'{':{
            type=LX_LCURLY;
            return true;
        }
        case'}':{
            type=LX_RCURLY;
            return true;
        }
        case'!':{
            ch = fileDescriptor->getChar();
            if (ch == '='){
                type=LX_NEQ;
                return true;
            }
            else{
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect operators");
                return false;
            }
        }
        case':':{
            ch = fileDescriptor->getChar();
            if (ch == '=')
                type=LX_COLON_EQ;
            else {
                fileDescriptor->ungetChar();
                type=LX_COLON;
            }
            return true;
        }
        case'<':{
            ch = fileDescriptor->getChar();
            if (ch == '=')
                type=LX_LE;
            else {
                fileDescriptor->ungetChar();
                type=LX_LT;
            }
            return true;
        }
        case'>':{
            ch = fileDescriptor->getChar();
            if (ch == '=')
                type=LX_GE;
            else {
                fileDescriptor->ungetChar();
                type=LX_GT;
            }
            return true;
        }
    }
    return false;
}













