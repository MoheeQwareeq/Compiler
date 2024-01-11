//
//  CodeGenerator.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 06/09/2023.
//

#include "CodeGenerator.h"

#define syscall gen("syscall");
#define line gen("");



CodeGenerator::CodeGenerator(){
    fout.open("assmbly.asm");
    lableCount = 0;
    flagData = 1;
    line
    gen(".data");
    line
    gen("__true__: .asciiz \"true\\n\"");
    gen("__false__:.asciiz \"false\\n\"");
    gen("__true__print: .asciiz \"true\"");
    gen("__false__print:.asciiz \"false\"");
    gen("__buffer__: .space 10");
    gen("__error__string__: .asciiz \"run time error\\n\"");
    line
    flagMain = 0;
    flageRead = 0;
    flageWrite = 0;
    flagePrintRead = 0;
    flagePrintWrite = 0;
}

string CodeGenerator::floatToIeeeHexString(float value){
    bitset<32> ieee754Representation;
    unsigned int intValue = *reinterpret_cast<unsigned int*>(&value);
    
    for (int i = 31; i >= 0; i--)
        ieee754Representation[i] = (intValue >> (31 - i)) & 1;
    
    stringstream hexStream;
    hexStream << hex << uppercase << setw(8) << setfill('0') << intValue;
    return hexStream.str();
}


void CodeGenerator::genVarDecl(Ast * n){
    if (flagData == 0){
        flagData = 1;//its .data
        line
        gen(".data");
        line
    }
    if(n->aVarDecl.type==TYPE_INTEGER) gen(n->aVarDecl.name->name +": .word\t0\t\t# integer");
    else if(n->aVarDecl.type==TYPE_BOOLEAN) gen(n->aVarDecl.name->name +": .word\t0\t\t# boolean");
    else if(n->aVarDecl.type==TYPE_STRING) gen(n->aVarDecl.name->name +": .space\t500\t\t# string");
    else if(n->aVarDecl.type==TYPE_FLOAT) gen(n->aVarDecl.name->name +": .float\t0.0\t\t# float");
}

void CodeGenerator::genConstant(Ast * n){
    if (flagData == 0){
        flagData = 1;//its .data
        line
        gen(".data");
        line
    }
    string alignment = "\t\t";
    if (n->aConstDecl.value > 10000000)alignment = "\t";
    gen(n->aConstDecl.name->name + ": .word\t"+
        to_string((n->aConstDecl.value))+alignment + "# integer constant");
}

void CodeGenerator::genRoutine(Ast * n){
    line
    if(n->aRoutineDecl.resultType != TYPE_NONE)
        gen("# function  " + n->aRoutineDecl.name->name);
    else
        gen("# procedure  " + n->aRoutineDecl.name->name);
    gen(".globl " + n->aRoutineDecl.name->name);
    line
    gen(n->aRoutineDecl.name->name +":");
    gen("# save $s0-$s7 and $sp and $fp");
    if (n->aRoutineDecl.name->name!= "main"){
        gen("move $t8, $sp");
    }
    line
    gen("addiu $sp, $sp, -36");
    gen("sw $fp, 36($sp)");
    gen("sw $s0, 32($sp)");
    gen("sw $s1, 28($sp)");
    gen("sw $s2, 24($sp)");
    gen("sw $s3, 20($sp)");
    gen("sw $s4, 16($sp)");
    gen("sw $s5, 12($sp)");
    gen("sw $s6, 8($sp)");
    gen("sw $s7, 4($sp)");
    gen("move $fp, $sp");
    int num = n->aRoutineDecl.num;
    int num_formal = n->aRoutineDecl.numOfFormal;
    
    gen("addiu $sp, $sp, " + to_string(-4*num));
    if (n->aRoutineDecl.name->name!= "main"){
        SteList * fomal = n->aRoutineDecl.formals;
        while (fomal->head) {
            gen("addiu $t8, $t8, 4");
            gen("lw $t0, 0($t8)");
            gen("sw $t0,"+to_string(4*(num_formal-1)) + "($fp)");
            fomal=fomal->next;
            num_formal--;
        }
    }
    line
    gen("# body");
    generate(n->aRoutineDecl.body);
    
    if(flagePrintWrite == 0 and flageWrite == 1) {
        flagePrintWrite++;
        line
        gen("write_____boolean :");
        gen("beq $t0, $zero, _F_a_l_s_e_");
        gen("la $a0, __true__print");
        gen("j p_r_i_n_t");
        gen("_F_a_l_s_e_:");
        gen("la $a0, __false__print");
        gen("p_r_i_n_t:");
        gen("li $v0, 4");
        syscall
        gen("jr $ra");
    }
    
    if(flagePrintRead == 0 and flageRead == 1){
        flagePrintRead++;
        line
        gen("read____boolean:");
        gen("li $v0, 8");
        gen("la $a0, __buffer__");
        gen("li $a1, 10");
        syscall
        gen("la $t0, __true__");
        gen("li $t3,1");
        gen("compare____true:");
        gen("lb $t1, ($t0)");
        gen("lb $t2, ($a0)");
        gen("beqz $t1, i_t_s_t_r_u_e");
        gen("beqz $t2, nـoـt_tـrـuـe");
        gen("beq $t1, $t2, continue___compare___true");
        gen("j nـoـt_tـrـuـe");
        gen("continue___compare___true:");
        gen("addi $t3, $t3,1");
        gen("addi $t0, $t0, 1");
        gen("addi $a0, $a0, 1");
        gen("j compare____true");
        gen("nـoـt_tـrـuـe:");
        gen("la $t0, __false__");
        gen("compare____false:");
        gen("lb $t1, ($t0)");
        gen("lb $t2, ($a0)");
        gen("beqz $t1, i_t_s_f_a_l_s_e");
        gen("beqz $t2, _e_r_r_o_r_");
        gen("beq $t1, $t2, continue___compare___false");
        gen("j _e_r_r_o_r_");
        gen("continue___compare___false:");
        gen("addi $t0, $t0, 1");
        gen("addi $a0, $a0, 1");
        gen("j compare____false");
        gen("i_t_s_t_r_u_e:");
        gen("beq $t3,5, nـoـt_tـrـuـe");
        gen("li $v0, 1");
        gen("jr $ra");
        gen("i_t_s_f_a_l_s_e:");
        gen("li $v0,0");
        gen("jr $ra");
        line
        gen("_e_r_r_o_r_:");
        gen("la $a0 , __error__string__");
        gen("li $v0, 4");
        syscall
        gen("li $v0, 10");
        syscall
    }
}

void CodeGenerator::genBlock(Ast * n){
    AstList * stmt = n->aBlock.stmts;
    while (stmt) {
        generate(stmt->head);
        stmt = stmt->next;
    }
}


void CodeGenerator::genOpreation(Ast * n){
    generate(n->aBinaryOp.larg);
    generate(n->aBinaryOp.rarg);
    J_TYPE left  = n->aBinaryOp.l_type;
    J_TYPE right = n->aBinaryOp.r_type;
    J_TYPE rel = n->aBinaryOp.rel_type;
    string str = ".s $f2,$f0,$f1";
    bool flage1 = (left == TYPE_INTEGER and right == TYPE_INTEGER);
    bool flage2 = (left == TYPE_BOOLEAN and right == TYPE_BOOLEAN);
    
    if (flage1 or flage2){
        gen("lw $t0, 8($sp)");
        gen("lw $t1, 4($sp)");
        str = " $t2, $t0, $t1";
    }
    
    else if(left == TYPE_FLOAT and right== TYPE_FLOAT ){
        gen("l.s $f0, 8($sp)");
        gen("l.s $f1, 4($sp)");
    }
    
    else if(left == TYPE_INTEGER and right== TYPE_FLOAT ){
        gen("lw $t0, 8($sp)");
        gen("mtc1 $t0, $f0");
        gen("cvt.s.w $f0, $f0");
        gen("l.s $f1, 4($sp)");
    }
    
    else if(left == TYPE_FLOAT and right== TYPE_INTEGER ){
        gen("l.s $f0, 8($sp)");
        gen("lw $t1, 4($sp)");
        gen("mtc1 $t0, $f1");
        gen("cvt.s.w $f1, $f1");
    }
    gen("addiu $sp, $sp, 8");
    
    switch (n->type ) {
            
        case AST_PLUS:
            gen("add"+str);
            break;
            
        case AST_MINUS:
            gen("sub"+str);
            break;
            
        case AST_TIMES:
            gen("mul"+str);
            break;
            
        case AST_DIVIDE:
            gen("div"+str);
            break;
            
        case AST_AND:
            gen("and"+str);
            break;
            
        case AST_OR:
            gen("or"+str);
            break;
            
        case AST_EQ:
            if(flage1)
                gen("seq"+str);
            else{
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.eq.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_NEQ:
            if(flage1)
                gen("sne"+str);
            else{
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.eq.s $f0, $f1");
                gen("movf $t2,$t3");
            }
            break;
            
        case AST_LT:
            if(flage1)
                gen("slt"+str);
            else {
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.lt.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_LE:
            if(flage1)
                gen("sle"+str);
            else{
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.le.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_GT:
            if(flage1)
                gen("sgt"+str);
            else {
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.lt.s $f1, $f0");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_GE:
            if(flage1)
                gen("sgt"+str);
            else{
                gen("li $t2, 0");
                gen("li $t3, 1");
                gen("c.lt.s $f0, $f1");
                gen("movf $t2,$t3");
            }
            break;
            
        default:
            break;
            
    }
    
    gen("addiu $sp,$sp,-4");
    
    if (rel == TYPE_INTEGER or rel == TYPE_BOOLEAN)
        gen("sw $t2, 4($sp)");
    else if (rel == TYPE_FLOAT)
        gen("s.s $f2, 4($sp)");
}

void CodeGenerator::genUnaryOpreation(Ast * n){
    
    generate(n->aUnaryOp.arg);
    gen("lw $t0, 4($sp)");
    gen("addiu $sp, $sp, 4");
    
    
    if(n->type == AST_UMINUS){
        if(n->aUnaryOp.type == TYPE_INTEGER)
            gen("mul $t1, $t0,-1");
        
        else if (n->aUnaryOp.type == TYPE_FLOAT)
            gen("xori $t1,$t0,0x80000000");
    }
    else if (n->type == AST_NOT)
        gen("xori $t1, $t0, 1");
    
    gen("addiu $sp,$sp,-4");
    gen("sw $t1, 4($sp)");
}



void CodeGenerator::genLit(Ast * n){
  
    if (n->type == AST_INTEGER)
        gen ("li $t0,"+to_string(n->aInteger.int_value));
    
    else if (n->type == AST_BOOLEAN)
        gen ("li $t0,"+to_string(n->aBoolean.value));
    
    else if (n->type == AST_FLOAT){
        
        string hexString = floatToIeeeHexString(n->aFloat.floatValue);
        string upper4Digits = hexString.substr(0, 4);
        string lower4Digits = hexString.substr(4, 4);
        gen("lui $t0, 0x" + upper4Digits);
        gen("ori $t0, $t0, 0x"+ lower4Digits);
    }
    
    gen("addiu $sp,$sp, -4");
    gen("sw $t0,4($sp)");
}


void CodeGenerator::genAssign(Ast * n){
    
    generate(n->aAssign.rhs);
    gen("addiu $sp, $sp, 4");
    
    J_TYPE left =n->aAssign.lhs->getType();
    J_TYPE right =n->aAssign.rightType;
    
    if ((left == TYPE_BOOLEAN) or
        (left == TYPE_INTEGER and right == TYPE_INTEGER) or
        (left == TYPE_FLOAT and right == TYPE_FLOAT))
        gen("lw $t0, 0($sp)");
    
    
    else if ((left == TYPE_INTEGER and right == TYPE_FLOAT)){
        gen("l.s $f0, 0($sp)");
        gen("cvt.w.s $f0, $f0");
        gen("mfc1 $t0, $f0");
    }
    else if ((left == TYPE_FLOAT and right == TYPE_INTEGER  )){
        gen("lw $t0, 0($sp)");
        gen("mtc1 $t0, $f0");
        gen("cvt.s.w $f0, $f0");
        gen("mfc1 $t0, $f0");
    }
    
    if(n->aAssign.lhs->offset ==-1){
        gen("la $t1, " + n->aAssign.lhs->name);
        gen("sw $t0, 0($t1)");
    }
    else
        gen("sw $t0," + to_string(n->aAssign.lhs->offset * 4)+"($fp)");
}


void CodeGenerator::genVar(Ast * n){
    if(n->aVar.var->getType() == TYPE_INTEGER or n->aVar.var->getType() == TYPE_BOOLEAN){
        if(n->aVar.var->offset == -1)
            gen("lw $t0, " + n->aVar.var->name);
        else
            gen("lw $t0, " + to_string(n->aVar.var->offset * 4)+"($fp) ");
        
        gen("addiu $sp,$sp, -4 ");
        gen("sw $t0, 4($sp)");
    }
    
    else if (n->aVar.var->getType()  == TYPE_FLOAT){
        if(n->aVar.var->offset == -1)
            gen("l.s $f0, "+n->aVar.var->name);
        else
            gen("l.s $f0, " + to_string(n->aVar.var->offset * 4)+"($fp) ");
        
        gen("addiu $sp,$sp, -4 ");
        gen("s.s $f0, 4($sp)");
    }
}

void CodeGenerator::genIntegerToFloat(Ast * n){
    gen("# convert integer to float");
    
    if(n->aVar.var->offset == -1)
        gen("lw $t0, "+n->aVar.var->name);
    else
        gen("lw $t0, "+to_string(n->aVar.var->offset*4)+"($fp) ");
    
    gen("mtc1 $t0, $f0");
    gen("cvt.s.w $f0, $f0");
    gen("addiu $sp,$sp, -4 ");
    gen("s.s $f0, 4($sp)");
}


void CodeGenerator::genFloatToInteger(Ast * n){
    gen("# convert float to integer");
    
    if(n->aVar.var->offset == -1)
        gen("l.s $f0, "+n->aVar.var->name);
    else
        gen("l.s $f0, "+to_string(n->aVar.var->offset*4)+"($fp) ");
    
    gen("cvt.w.s $f0, $f0");
    gen("mfc1 $t0, $f0");
    gen("addiu $sp,$sp, -4 ");
    gen("sw $t0, 4($sp)");
}



void CodeGenerator::genRead(Ast * n){
    line
    if(n->aRead.var->getType()==TYPE_INTEGER){
        if(n->aRead.var->offset == -1){
            gen("# read global integer "+n->aRead.var->name);
            gen("li $v0, 5 ");
            syscall
            gen("sw $v0, "+n->aRead.var->name);
            line
        }
        else {
            gen("# read local integer "+n->aRead.var->name);
            gen("li $v0, 5 ");
            syscall
            gen("sw $v0, "+to_string(n->aRead.var->offset*4)+"($fp)");
            line
        }
    }
    
    else if(n->aRead.var->getType() == TYPE_BOOLEAN){
        flageRead=1;
        if(n->aRead.var->offset==-1){
            gen("# read global boolean "+n->aRead.var->name);
            gen("addiu $sp, $sp , -4");
            gen("sw $ra, 4($sp)");
            line
            gen("jal read____boolean");
            line
            gen("sw $v0, "+n->aRead.var->name);
            gen("addiu $sp, $sp , 4");
            gen("lw $ra, 0($sp)");
            line
        }
        else {
            gen("# read local boolean "+n->aRead.var->name);
            gen("addiu $sp, $sp , -4");
            gen("sw $ra, 4($sp)");
            line
            gen("jal read____boolean");
            line
            gen("sw $v0, "+to_string(n->aRead.var->offset*4)+"($fp)");
            gen("addiu $sp, $sp , 4");
            gen("lw $ra, 0($sp)");
            line
            line
        }
    }
    
    else if (n->aRead.var->getType() == TYPE_FLOAT){
        
        if(n->aRead.var->offset == -1){
            gen("# read global float "+n->aRead.var->name);
            gen("li $v0, 6 ");
            syscall
            gen("s.s $f0, "+n->aRead.var->name);
            line
        }
        else {
            gen("# read local float "+n->aRead.var->name);
            gen("li $v0, 6 ");
            syscall
            gen("s.s $f0, "+to_string(n->aRead.var->offset*4)+"($fp)");
            line
        }
    }
}


void CodeGenerator:: genWrite(Ast * n){
    line
    if(n->aWrite.var->getType()==TYPE_INTEGER){
        if(n->aWrite.var->offset==-1  ){
            gen("# write global integer "+n->aWrite.var->name);
            gen("lw $a0, "+n->aWrite.var->name);
        }
        else{
            gen("# write local integer "+n->aWrite.var->name);
            gen("lw $a0,"+to_string(4*n->aWrite.var->offset)+"($fp)");
        }
        gen("li $v0, 1");
        syscall
    }
    
    else if(n->aWrite.var->getType()==TYPE_BOOLEAN){
        flageWrite=1;
        if(n->aWrite.var->offset==-1){
            gen("# write global boolean "+n->aWrite.var->name);
            gen("lw $t0, "+n->aWrite.var->name);
        }
        else {
            gen("# write local boolean "+n->aWrite.var->name);
            gen("lw $t0,"+to_string(4*n->aWrite.var->offset)+"($fp)");
        }
        gen("addiu $sp, $sp , -4");
        gen("sw $ra, 4($sp)");
        line
        gen("jal write_____boolean");
        gen("addiu $sp, $sp , 4");
        gen("lw $ra, 0($sp)");
        line
    }
    
    else if(n->aWrite.var->getType()==TYPE_FLOAT){
        
        if(n->aWrite.var->offset == -1  ){
            gen("# write global float "+n->aWrite.var->name);
            gen("l.s $f12, "+n->aWrite.var->name);
        }
        else{
            gen("# write local float "+n->aWrite.var->name);
            gen("l.s $f12,"+to_string(4*n->aWrite.var->offset)+"($fp)");
        }
        gen("li $v0, 2");
        syscall
    }
    
}

void CodeGenerator::genIf(Ast * n){
    line
    gen("# if statment");
    string altern_label = "if__"+makeLabel();
    string end_label = "end__if__"+makeLabel();
    generate(n->aIf.predicate);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("beqz $t0, "+altern_label);
    generate(n->aIf.conseq);
    gen ("j " +end_label);
    gen (altern_label+":");
    generate(n->aIf.altern);
    gen (end_label+":");
}

void CodeGenerator::genWhile(Ast *n){
    line
    gen("# while statment");
    string while_label ="while__"+makeLabel();
    string end_label ="end__while__"+makeLabel();
    gen (while_label+":");
    generate (n->aWhile.predicate);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("beqz $t0,"+end_label);
    generate (n->aWhile.body);
    gen ("j "+while_label);
    gen (end_label+":");
}

void CodeGenerator::genFor(Ast *n){
    line
    gen("# for statment");
    string start_label= "for__"+makeLabel();
    string for_label  = "statr__for__"+makeLabel();
    string end_label  = "end__for__"+makeLabel();
    generate(n->aFor.lowerBound);
    gen("addiu $sp, $sp, 4");
    gen("lw $t1, 0($sp)");
    
    if(n->aFor.var->offset ==-1){
        gen("la $t0, "+n->aFor.var->name);
        gen("sw $t1, 0($t0)");
    }
    else
        gen("sw $t1,"+to_string(n->aFor.var->offset*4)+"($fp)");
    
    gen(start_label+" :");
    generate(n->aFor.upperBound);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("ble $t1,$t0,"+for_label);
    gen("j "+end_label);
    gen(for_label+" :");
    generate(n->aFor.body);
    
    if(n->aFor.var->offset == -1){
        gen("la $t0, "+n->aFor.var->name);
        gen("lw $t1, 0($t0)");
        gen("addiu $t1,$t1,1");
        gen("sw $t1, 0($t0)");
    }
    else{
        gen("lw $t1,"+to_string(n->aFor.var->offset*4)+"($fp)");
        gen("addiu $t1,$t1,1");
        gen("sw $t1,"+to_string(n->aFor.var->offset*4)+"($fp)");
    }
    gen("j "+start_label);
    gen(end_label+" :");
}


void CodeGenerator::genCall(Ast *n){
    
    gen("# save $ra and $t0-$t9");
    gen("addiu $sp, $sp, -44");
    gen("sw $ra, 44($sp)");
    gen("sw $t0, 40($sp)");
    gen("sw $t1, 36($sp)");
    gen("sw $t2, 32($sp)");
    gen("sw $t3, 28($sp)");
    gen("sw $t4, 24($sp)");
    gen("sw $t5, 20($sp)");
    gen("sw $t6, 16($sp)");
    gen("sw $t7, 12($sp)");
    gen("sw $t8, 8($sp)");
    gen("sw $t9, 4($sp)");
    line
    int c = 0;
    AstList * arg = n->aCall.argList;
    SteList * formal = n->aCall.callee->routine.formals;
    
    while (arg->head) {
        c++;
        J_TYPE real   = arg->type;
        J_TYPE expect = formal->head->getType();
        
        if(real == expect)
            generate(arg->head);
        
        else
            if (real == TYPE_FLOAT and expect == TYPE_INTEGER )
                genFloatToInteger(arg->head);
            
            else if(real == TYPE_INTEGER and expect== TYPE_FLOAT )
                genIntegerToFloat(arg->head);
        
        
        arg = arg->next;
        formal = formal->next;
    }
    
    gen ("jal " + n->aCall.callee->name);
    line
    gen ("addiu $sp, $sp,"+to_string(c*4));
    gen("# restore $ra and $t0 - $t9");
    gen("addiu $sp, $sp, 44");
    gen("lw $t9, -40($sp)");
    gen("lw $t8, -36($sp)");
    gen("lw $t7, -32($sp)");
    gen("lw $t6, -28($sp)");
    gen("lw $t5, -24($sp)");
    gen("lw $t4, -20($sp)");
    gen("lw $t3, -16($sp)");
    gen("lw $t2, -12($sp)");
    gen("lw $t1, -8($sp)");
    gen("lw $t0, -4($sp)");
    gen("lw $ra, 0($sp)");
    gen("addiu $sp, $sp, -4");
    gen("sw $v1, 4($sp)");
}


void CodeGenerator::genReturn(Ast * n){
    line
    gen("# return");
    generate(n->aReturn.expr);
    gen("addiu $sp, $sp, 4");
    
    J_TYPE expect = n->aReturn.expectReturnType;
    J_TYPE real = n->aReturn.realReturnType;
    
    if((real == TYPE_INTEGER and expect == TYPE_INTEGER )
       or real == TYPE_BOOLEAN)
        gen("lw $v1, 0($sp)");
    
    else if (real == TYPE_INTEGER and expect == TYPE_FLOAT ){
        gen("lw $v1, 0($sp)");
        gen("mtc1 $v1, $f0");
        gen("cvt.s.w $f0, $f0");
        gen("mfc1 $v1, $f0");
    }
    
    else if (real == TYPE_FLOAT and expect == TYPE_INTEGER ){
        gen("l.s $f0, 0($sp)");
        gen("cvt.w.s $f0, $f0");
        gen("mfc1 $v1, $f0");
    }
    
    else if (real == TYPE_FLOAT and expect == TYPE_FLOAT ){
        gen("l.s $f0, 0($sp)");
        gen("mfc1 $v1, $f0");
    }
    gen("move $sp, $fp");
    gen("# restore $fp and %sp and $s0 - $s7");
    gen("addiu $sp, $sp, 36");
    gen("lw $s7, -32($sp)");
    gen("lw $s6, -28($sp)");
    gen("lw $s5, -24($sp)");
    gen("lw $s4, -20($sp)");
    gen("lw $s3, -16($sp)");
    gen("lw $s2, -12($sp)");
    gen("lw $s1, -8($sp)");
    gen("lw $s0, -4($sp)");
    gen("lw $fp, 0($sp)");
    gen("jr $ra");
}


void CodeGenerator::generate(Ast * n){
    if(!n)return;
    
    AST_type type = n->type;
    if (flagData == 1 and type != AST_CONST_DECL and  type != AST_VAR_DECL) {
        flagData = 0;
        flagMain++;
        line
        gen(".text");
        line
        if(flagMain == 1){
            gen("jal main");
            line
            gen("exit:");
            gen("li $v0, 10");
            syscall
            line
        }
    }
    
    switch (type) {
            
        case AST_VAR_DECL:
            genVarDecl(n);
            break;
            
        case AST_CONST_DECL:
            genConstant(n);
            break;
            
        case AST_ROUTINE_DECL:
            genRoutine(n);
            break;
            
        case AST_BLOCK:
            genBlock(n);
            break;
            
        case AST_VAR:
            genVar(n);
            break;
            
        case AST_INTEGER:
        case AST_BOOLEAN:
        case AST_FLOAT:
            genLit(n);
            break;
            
        case AST_PLUS:
        case AST_MINUS:
        case AST_TIMES:
        case AST_DIVIDE:
        case AST_EQ:
        case AST_NEQ:
        case AST_GT:
        case AST_GE:
        case AST_LE:
        case AST_LT:
        case AST_AND:
        case AST_OR:
            genOpreation(n);
            break;
            
        case AST_UMINUS:
        case AST_NOT:
            genUnaryOpreation(n);
            break;
            
        case AST_IF:
            genIf(n);
            break;
            
        case AST_ASSIGN:
            genAssign(n);
            break;
            
        case AST_WHILE:
            genWhile(n);
            break;
            
        case AST_FOR:
            genFor(n);
            break;
            
        case AST_READ:
            genRead(n);
            break;
            
        case AST_WRITE:
            genWrite(n);
            break;
            
        case AST_CALL:
            genCall(n);
            break;
            
        case AST_RETURN:
            genReturn(n);
            break;
            
        default:
            break;
    }
}


string CodeGenerator:: makeLabel(){
    string label = "label__" + to_string(lableCount++);
    return label;
}

void CodeGenerator::gen(string  massage){
    fout<<massage<<endl;
}

CodeGenerator::~CodeGenerator() {
    fout.close();
}







