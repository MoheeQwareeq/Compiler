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
    
    FileDescriptor * fileDescriptor;
    void semanticError(FileDescriptor * , string );
    void semanticWarning(FileDescriptor * , string );
    J_TYPE expressionType (AST *);
    bool routineContainEnoughReturn(AST * );
    void cheakAllReturnType(AST *,J_TYPE);

public:
    
    SemanticChecker(FileDescriptor *);
    void checkStatement (AST * , J_TYPE);
    
};




#endif /* SemanticChecker_h */
