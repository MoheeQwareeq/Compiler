//
//  CodeGenerator.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 06/09/2023.
//

#ifndef CodeGenerator_h
#define CodeGenerator_h



#include "AST.h"
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
    void genVarDecl(AST *);
    void genConstant(AST *);
    void genRoutine(AST *);
    void genBlock(AST *);
    void genStmt(AST * );
    void genOpreation(AST *);
    void genUnaryOpreation(AST *);
    void genAssign(AST *);
    void genVar(AST *);
    void genLit(AST *);
    void genIf(AST *);
    void genWhile(AST *);
    void genFor(AST *);
    void genCall(AST *);
    void genRead(AST *);
    void genWrite(AST *);
    void genReturn(AST *);
    void genIntegerToFloat(AST *);
    void genFloatToInteger(AST *);
    string makeLabel();
    string floatToIeeeHexString(float);
    
public:
    
    CodeGenerator();
    void generate(AST *);
    ~CodeGenerator();
};


#endif /* CodeGenerator_h */
