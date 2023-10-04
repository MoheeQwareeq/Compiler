//
//  Scanner.cpp
//  Parser
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#include "Scanner.h"

Scanner :: Scanner(FileDescriptor *fd){
    fileDescriptor = fd;
}


TOKEN * Scanner ::Scan(){
    TOKEN* temp = new TOKEN;
    LEXEME_TYPE t;
    
    char ch = fileDescriptor->getChar();    
    if (ch=='"')return get_string(ch);
    else if (isalpha(ch) || ch == '_')return get_identifier(ch);
    else if (isdigit(ch))return get_number(ch);
    else if (ch=='#')return skip_comments(ch);
    else if (isspace(ch))return Scan();
    else if (is_operators(ch,t)){temp->type=t; return temp;}
    else if (ch == EOF){temp->type=LX_EOF; return temp;}
    else {
        fileDescriptor->reportError("Lexical Error: Illegal in COMP23 programs");
        exit(EXIT_FAILURE);
        
    }
    
}



TOKEN * Scanner ::get_identifier(char ch){
    TOKEN* temp = new TOKEN;
    string str="";
    str += ch;
    int index=0;
    ch= fileDescriptor->getChar();
    while(isalpha(ch) || isdigit(ch) || ch == '_'){
        str += ch;
        ch= fileDescriptor->getChar();
    }
    
    if(check_keyword(str,index) )temp->type=key_type[index];
    else {
        temp->type=LX_IDENTIFIER;
        temp->string_value=str;
        
    }
    
    if(is_delimiter(ch)){
        fileDescriptor->ungetChar();
        return temp;}
    
    fileDescriptor->ungetChar();
    fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  identifier number");
    exit(EXIT_FAILURE);
}






TOKEN * Scanner ::get_number(char ch){
    TOKEN* temp = new TOKEN;
    string str="";
    
    if(isdigit(ch)){
        while (isdigit(ch)) {str += ch;ch= fileDescriptor->getChar();  }
        int checkOverflow;
        try {
            checkOverflow = stoi(str);
        } catch (const out_of_range& e) {
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
                    if(is_delimiter(ch)){
                        fileDescriptor->ungetChar();
                        temp->type=LX_FLOAT;
                        temp->float_value =   stof(str);
                        return temp;
                    }
                }
                
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  floating number");
                exit(EXIT_FAILURE);
            }
                
            default:{
                if(is_delimiter(ch)){
                    fileDescriptor->ungetChar();
                    temp->type=LX_INTEGER;
                    temp->int_value = stoi(str);
                    return temp;}
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect   definition  of  integer number");
                exit(EXIT_FAILURE);
                
                
            }}}
    return temp;
    
}








TOKEN * Scanner ::get_string(char ch){
    TOKEN* temp = new TOKEN;
    string str="";
    ch= fileDescriptor->getChar();
    while(ch != '"' && ch != '\n' ){
        str += ch;
        ch= fileDescriptor->getChar();}
    
    switch (ch){
        case '"':{
            ch= fileDescriptor->getChar();
            if(is_delimiter(ch)){
                fileDescriptor->ungetChar();
                temp->type=LX_STRING;
                temp->string_value=str;
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







TOKEN * Scanner ::skip_comments(char ch){
    
    ch= fileDescriptor->getChar();
    if (ch=='#'){
        ch= fileDescriptor->getChar();
        while(ch != '#' && ch != '\n')ch= fileDescriptor->getChar();
        
        switch (ch){
            case '#':{
                ch= fileDescriptor->getChar();
                if (ch=='#') return Scan();
                else {
                    fileDescriptor->ungetChar();
                    fileDescriptor->reportError("Lexical Error: Incorrect comments");
                }
            }
            default:return Scan();}}
    else {
        fileDescriptor->ungetChar();
        fileDescriptor->reportError("Lexical Error: Incorrect comments");
        exit(EXIT_FAILURE);
        
    }
}





bool Scanner ::check_keyword(string str , int &index){
    
    for (int i=0;i <keys;i++)
        if(str == keyword[i]){
            index=i;
            return true;
        }
    
    return false;
}



bool Scanner ::is_delimiter(char ch) {
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




bool Scanner :: is_operators(char ch,LEXEME_TYPE &t){
    switch (ch){
            
        case'(':{t=LX_LPAREN;return true;}
        case')':{t=LX_RPAREN;return true;}
        case'+':{t=LX_PLUS;return true;}
        case'-':{t=LX_MINUS; return true;}
        case'*':{t=LX_STAR;return true;}
        case'/':{t=LX_SLASH; return true;}
        case'=':{t=LX_EQ; return true;}
        case'.':{t=LX_DOT;return true;}
        case';':{t=LX_SEMICOLON;return true;}
        case'[':{t=LX_LBRACKET;return true;}
        case']':{t=LX_RBRACKET;return true;}
        case',':{t=LX_COMMA;return true;}
        case'{':{t=LX_LCURLY;return true;}
        case'}':{t=LX_RCURLY;return true;}
        case'!':{
            ch = fileDescriptor->getChar();
            if (ch == '='){t=LX_NEQ; return true;}
            else{
                fileDescriptor->ungetChar();
                fileDescriptor->reportError("Lexical Error: Incorrect operators");
                return false;
            }
        }
        case':':{
            ch = fileDescriptor->getChar();
            if (ch == '=')t=LX_COLON_EQ;
            else {
                fileDescriptor->ungetChar();
                t=LX_COLON;
            }
            return true;
        }
        case'<':{
            ch = fileDescriptor->getChar();
            if (ch == '=')t=LX_LE;
            else {
                fileDescriptor->ungetChar();
                t=LX_LT;
            }
            return true;}
        case'>':{
            ch = fileDescriptor->getChar();
            if (ch == '=')t=LX_GE;
            else {
                fileDescriptor->ungetChar();
                t=LX_GT;}
            return true;}
            
    }
    return false;
}













