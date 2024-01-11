//
//  SemanticChecker.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 30/08/2023.
//

#ifndef SemanticChecker_h
#define SemanticChecker_h


#include "Ast.h"
#include "SymbolTableList.h"

class SemanticChecker {
    
private:
    
    FileDescriptor * fileDescriptor;
    void semanticError(FileDescriptor * , string );
    void semanticWarning(FileDescriptor * , string );
    J_TYPE expressionType (Ast *);
    bool routineContainEnoughReturn(Ast * );
    void cheakAllReturnType(Ast *,J_TYPE);

public:
    
    SemanticChecker(FileDescriptor *);
    void checkStatement (Ast * , J_TYPE);
    
};




#endif /* SemanticChecker_h */
