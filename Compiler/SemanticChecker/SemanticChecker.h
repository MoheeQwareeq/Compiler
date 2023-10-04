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
    void semantic_error(FileDescriptor * , string );
    void semantic_warning(FileDescriptor * , string );
    
    
    
public:
    j_type expression_type (AST *);
    SemanticChecker(FileDescriptor* );
    void check_Statement (AST * , j_type );
    
};




#endif /* SemanticChecker_h */
