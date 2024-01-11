//
//  CodeGenerator.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 06/09/2023.
//

#ifndef CodeGenerator_h
#define CodeGenerator_h



#include "Ast.h"
#include <sstream>

class CodeGenerator {
    
private:
    
    ofstream fout;
    int lableCount;
    int flagData;
    int flagMain;
    int flagePrintRead;
    int flagePrintWrite;
    int flageRead;
    int flageWrite;
    void gen(string );
    void genVarDecl(Ast *);
    void genConstant(Ast *);
    void genRoutine(Ast *);
    void genBlock(Ast *);
    void genStmt(Ast * );
    void genOpreation(Ast *);
    void genUnaryOpreation(Ast *);
    void genAssign(Ast *);
    void genVar(Ast *);
    void genLit(Ast *);
    void genIf(Ast *);
    void genWhile(Ast *);
    void genFor(Ast *);
    void genCall(Ast *);
    void genRead(Ast *);
    void genWrite(Ast *);
    void genReturn(Ast *);
    void genIntegerToFloat(Ast *);
    void genFloatToInteger(Ast *);
    string makeLabel();
    string floatToIeeeHexString(float);
    
public:
    
    CodeGenerator();
    void generate(Ast *);
    ~CodeGenerator();
};


#endif /* CodeGenerator_h */
