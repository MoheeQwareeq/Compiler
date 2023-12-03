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
    void gen_var_decl(AST *);
    void gen_constant(AST *);
    void gen_routine(AST *);
    void gen_block(AST *);
    void gen_stmt(AST * );
    void gen_opreation(AST *);
    void gen_unary_opreation(AST *);
    void gen_assign(AST *);
    void gen_var(AST *);
    void gen_lit(AST *);
    void gen_if(AST *);
    void gen_while(AST *);
    void gen_for(AST *);
    void gen_call(AST *);
    void gen_read(AST *);
    void gen_write(AST *);
    void gen_return(AST *);
    void gen_integer_to_float(AST *);
    void gen_float_to_integer(AST *);
    string makeLabel();
    string floatToIEEE754HexString(float value);
    
public:
    
    CodeGenerator();
    void generate(AST *);
    ~CodeGenerator();
};


#endif /* CodeGenerator_h */
