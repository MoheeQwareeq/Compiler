//
//  SemanticChecker.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 30/08/2023.
//

#ifndef SemanticChecker_h
#define SemanticChecker_h


#include "AST.h"
#include "SymbolTableList.h"

class SemanticChecker {
    
private:
    FileDescriptor* fileDescriptor;
    void semanticError(FileDescriptor * , string );
    void semanticWarning(FileDescriptor * , string );
    j_type expression_type (AST *);
    bool routine_contain_enough_return(AST * );
    void cheak_all_return_type(AST *,j_type);

public:
    SemanticChecker(FileDescriptor* );
    void check_Statement (AST * , j_type );
    
};




#endif /* SemanticChecker_h */
