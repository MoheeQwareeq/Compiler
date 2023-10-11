//
//  CodeGenerator.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 06/09/2023.
//

#include "CodeGenerator.h"

#define syscall gen("syscall");
#define newLine gen("");



CodeGenerator::CodeGenerator(){
    fout.open("assmbly.asm");
    lableCount=0;
    flag_data=1;
    newLine
    gen(".data");
    newLine
    gen("__true__: .asciiz \"true\\n\"");
    gen("__false__:.asciiz \"false\\n\"");
    gen("__true__print: .asciiz \"true\"");
    gen("__false__print:.asciiz \"false\"");
    gen("__buffer__: .space 10");
    gen("__error__string__: .asciiz \"run time error\\n\"");
    newLine
    flag_main=0;
    flage_read=0;
    flage_write=0;
    flage_print_read=0;
    flage_print_write=0;
}

string CodeGenerator::floatToIEEE754HexString(float value) {
    bitset<32> ieee754Representation;
    unsigned int intValue = *reinterpret_cast<unsigned int*>(&value);
    
    for (int i = 31; i >= 0; i--)
        ieee754Representation[i] = (intValue >> (31 - i)) & 1;
    
    stringstream hexStream;
    hexStream << hex << uppercase << setw(8) << setfill('0') << intValue;
    return hexStream.str();
}


void CodeGenerator::gen_var_decl(AST * n){
    if (flag_data == 0){
        flag_data = 1;//its .data
        newLine
        gen(".data");
        newLine
    }
    if(n->a_var_decl.type==TYPE_INTEGER) gen(n->a_var_decl.name->name +": .word\t0\t\t# integer");
    else if(n->a_var_decl.type==TYPE_BOOLEAN) gen(n->a_var_decl.name->name +": .word\t0\t\t# boolean");
    else if(n->a_var_decl.type==TYPE_STRING) gen(n->a_var_decl.name->name +": .space\t500\t\t# string");
    else if(n->a_var_decl.type==TYPE_FLOAT) gen(n->a_var_decl.name->name +": .float\t0.0\t\t# float");
}

void CodeGenerator::gen_constant(AST * n){
    if (flag_data == 0){
        flag_data = 1;//its .data
        newLine
        gen(".data");
        newLine
    }
    string alignment = "\t\t";
    if (n->a_const_decl.value > 10000000)alignment = "\t";
    gen(n->a_const_decl.name->name +": .word\t"+to_string((n->a_const_decl.value))+alignment+"# integer constant");
}

void CodeGenerator::gen_routine(AST * n){
    newLine
    if(n->a_routine_decl.result_type != TYPE_NONE)
        gen("# function  "+ n->a_routine_decl.name->name);
    else
        gen("# procedure  "+ n->a_routine_decl.name->name);
    gen(".globl "+n->a_routine_decl.name->name);
    newLine
    gen(n->a_routine_decl.name->name +":");
    gen("# save $s0-$s7 and $sp and $fp");
    if (n->a_routine_decl.name->name!= "main"){
        gen("move $t8, $sp");
    }
    newLine
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
    int num =n->a_routine_decl.num;
    int num_formal=n->a_routine_decl.num_of_formal;
    
    gen("addiu $sp, $sp, "+to_string(-4*num));
    if (n->a_routine_decl.name->name!= "main"){
        ste_list * fomal = n->a_routine_decl.formals;
        while (fomal->head) {
            gen("addiu $t8, $t8, 4");
            gen("lw $t0, 0($t8)");
            gen("sw $t0,"+to_string(4*(num_formal-1))+"($fp)");
            fomal=fomal->next;
            num_formal--;
        }
    }
    newLine
    gen("# body");
    generate(n->a_routine_decl.body);
    
    if(flage_print_write == 0 and flage_write==1) {
        flage_print_write++;
        newLine
        gen("write_____boolean :");
        gen("beq $t0, $zero, _F_a_l_s_e_");
        gen("la $a0, __true__print");
        gen("j p_r_i_n_t");
        gen("_F_a_l_s_e_:");
        gen("la $a0, __false__print");
        gen("p_r_i_n_t:");
        gen("li $v0, 4");
        syscall
        gen("jr $ra");}
    
    if(flage_print_read == 0 and flage_read == 1){
        flage_print_read++;
        newLine
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
        newLine
        gen("_e_r_r_o_r_:");
        gen("la $a0 , __error__string__");
        gen("li $v0, 4");
        syscall
        gen("li $v0, 10");
        syscall
        
    }
}

void CodeGenerator::gen_block(AST * n){
    ast_list * stmt = n->a_block.stmts;
    while (stmt) {
        generate(stmt->head);
        stmt=stmt->next;
    }
}


void CodeGenerator::gen_opreation(AST * n){
    generate(n->a_binary_op.larg);
    generate(n->a_binary_op.rarg);
    
    j_type left  = n->a_binary_op.l_type;
    j_type right = n->a_binary_op.r_type;
    j_type rel = n->a_binary_op.rel_type;
    string str ="";
    bool flage = (left == TYPE_INTEGER and right== TYPE_INTEGER);
    
    if (flage or (left == TYPE_BOOLEAN and right== TYPE_BOOLEAN)){
        gen("lw $t0, 8($sp)");
        gen("lw $t1, 4($sp)");
        str=" $t2, $t0, $t1";
    }
    
    else if(left == TYPE_FLOAT and right== TYPE_FLOAT ){
        gen("l.s $f0, 8($sp)");
        gen("l.s $f1, 4($sp)");
        str=".s $f2,$f0,$f1";
    }
    
    else if(left == TYPE_INTEGER and right== TYPE_FLOAT ){
        gen("lw $t0, 8($sp)");
        gen("mtc1 $t0, $f0");
        gen("cvt.s.w $f0, $f0");
        gen("l.s $f1, 4($sp)");
        str=".s $f2,$f0,$f1";
    }
    
    else if(left == TYPE_FLOAT and right== TYPE_INTEGER ){
        gen("l.s $f0, 8($sp)");
        gen("lw $t1, 4($sp)");
        gen("mtc1 $t0, $f1");
        gen("cvt.s.w $f1, $f1");
        str=".s $f2,$f0,$f1";
    }
    gen("li $t3, 1");
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
            if(flage)
                gen("seq"+str);
            else{
                gen("c.eq.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_NEQ:
            if(flage)
                gen("sne"+str);
            else{
                gen("c.eq.s $f0, $f1");
                gen("movf $t2,$t3");
            }
            break;
            
        case AST_LT:
            if(flage)
                gen("slt"+str);
            else {
                gen("c.lt.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_LE:
            if(flage)
                gen("sle"+str);
            else{
                gen("c.le.s $f0, $f1");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_GT:
            if(flage)
                gen("sgt"+str);
            else {
                gen("c.lt.s $f1, $f0");
                gen("movt $t2,$t3");
            }
            break;
            
        case AST_GE:
            if(flage)
                gen("sgt"+str);
            else{
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

void CodeGenerator::gen_unary_opreation(AST * n){
    
    generate(n->a_unary_op.arg);
    
    
    gen("lw $t0, 4($sp)");
    gen("addiu $sp, $sp, 4");
    
    
    if(n->type == AST_UMINUS){
        if(n->a_unary_op.type == TYPE_INTEGER)
            gen("mul $t1, $t0,-1");
        
        else if (n->a_unary_op.type == TYPE_FLOAT)
            gen("xori $t1,$t0,0x80000000");
    }
    else if (n->type == AST_NOT)
        gen("xori $t1, $t0, 1");
    
    gen("addiu $sp,$sp,-4");
    gen("sw $t1, 4($sp)");
}



void CodeGenerator::gen_lit(AST *n){
    if (n->type == AST_INTEGER)
        gen ("li $t0,"+to_string(n->a_integer.int_value));
    
    else if (n->type == AST_BOOLEAN)
        gen ("li $t0,"+to_string(n->a_boolean.value));
    
    else if (n->type == AST_FLOAT){
        
        string hexString = floatToIEEE754HexString(n->a_float.float_value);
        string upper4Digits = hexString.substr(0, 4);
        string lower4Digits = hexString.substr(4, 4);
        gen("lui $t0, 0x" + upper4Digits);
        gen("ori $t0, $t0, 0x"+ lower4Digits);
    }
    
    gen("addiu $sp,$sp, -4");
    gen("sw $t0,4($sp)");
}


void CodeGenerator::gen_assign(AST * n){
    
    generate(n->a_assign.rhs);
    gen("addiu $sp, $sp, 4");
    
    j_type left =n->a_assign.lhs->getType();
    j_type right =n->a_assign.rightType;
    
    if ((left == TYPE_BOOLEAN) or
        (left==TYPE_INTEGER and right == TYPE_INTEGER) or
        (left == TYPE_FLOAT and right == TYPE_FLOAT))
        gen("lw $t0, 0($sp)");
    
    
    else if ((left==TYPE_INTEGER and right == TYPE_FLOAT)){
        gen("l.s $f0, 0($sp)");
        gen("cvt.w.s $f0, $f0");
        gen("mfc1 $t0, $f0");
    }
    else if ((left==TYPE_FLOAT and right == TYPE_INTEGER  )){
        gen("lw $t0, 0($sp)");
        gen("mtc1 $t0, $f0");
        gen("cvt.s.w $f0, $f0");
        gen("mfc1 $t0, $f0");
    }
    
    if(n->a_assign.lhs->offset ==-1){
        gen("la $t1, "+n->a_assign.lhs->name);
        gen("sw $t0, 0($t1)");
    }
    else
        gen("sw $t0,"+to_string(n->a_assign.lhs->offset*4)+"($fp)");
}


void CodeGenerator::gen_var(AST * n){
    if(n->a_var.var->getType() == TYPE_INTEGER or n->a_var.var->getType() == TYPE_BOOLEAN){
        if(n->a_var.var->offset == -1)
            gen("lw $t0, "+n->a_var.var->name);
        else
            gen("lw $t0, "+to_string(n->a_var.var->offset*4)+"($fp) ");
        
        gen("addiu $sp,$sp, -4 ");
        gen("sw $t0, 4($sp)");
    }
    
    else if (n->a_var.var->getType()  == TYPE_FLOAT){
        if(n->a_var.var->offset == -1)
            gen("l.s $f0, "+n->a_var.var->name);
        else
            gen("l.s $f0, "+to_string(n->a_var.var->offset*4)+"($fp) ");
        
        gen("addiu $sp,$sp, -4 ");
        gen("s.s $f0, 4($sp)");
    }
}



void CodeGenerator::gen_read(AST * n){
    newLine
    if(n->a_read.var->getType()==TYPE_INTEGER){
        if(n->a_read.var->offset == -1){
            gen("# read global integer "+n->a_read.var->name);
            gen("li $v0, 5 ");
            syscall
            gen("sw $v0, "+n->a_read.var->name);
            newLine
        }
        else {
            gen("# read local integer "+n->a_read.var->name);
            gen("li $v0, 5 ");
            syscall
            gen("sw $v0, "+to_string(n->a_read.var->offset*4)+"($fp)");
            newLine
        }
    }
    
    else if(n->a_read.var->getType() == TYPE_BOOLEAN){
        flage_read=1;
        if(n->a_read.var->offset==-1){
            gen("# read global boolean "+n->a_read.var->name);
            gen("addiu $sp, $sp , -4");
            gen("sw $ra, 4($sp)");
            newLine
            gen("jal read____boolean");
            newLine
            gen("sw $v0, "+n->a_read.var->name);
            gen("addiu $sp, $sp , 4");
            gen("lw $ra, 0($sp)");
            newLine
        }
        else {
            gen("# read local boolean "+n->a_read.var->name);
            gen("addiu $sp, $sp , -4");
            gen("sw $ra, 4($sp)");
            newLine
            gen("jal read____boolean");
            newLine
            gen("sw $v0, "+to_string(n->a_read.var->offset*4)+"($fp)");
            gen("addiu $sp, $sp , 4");
            gen("lw $ra, 0($sp)");
            newLine
            newLine
        }
    }
    
    else if (n->a_read.var->getType() == TYPE_FLOAT){
        
        if(n->a_read.var->offset == -1){
            gen("# read global float "+n->a_read.var->name);
            gen("li $v0, 6 ");
            syscall
            gen("s.s $f0, "+n->a_read.var->name);
            newLine
        }
        else {
            gen("# read local float "+n->a_read.var->name);
            gen("li $v0, 6 ");
            syscall
            gen("s.s $f0, "+to_string(n->a_read.var->offset*4)+"($fp)");
            newLine
        }
    }
}


void CodeGenerator:: gen_write(AST * n){
    newLine
    if(n->a_write.var->getType()==TYPE_INTEGER){
        if(n->a_write.var->offset==-1  ){
            gen("# write global integer "+n->a_write.var->name);
            gen("lw $a0, "+n->a_write.var->name);
        }
        else{
            gen("# write local integer "+n->a_write.var->name);
            gen("lw $a0,"+to_string(4*n->a_write.var->offset)+"($fp)");
        }
        gen("li $v0, 1");
        syscall
    }
    
    else if(n->a_write.var->getType()==TYPE_BOOLEAN){
        flage_write=1;
        if(n->a_write.var->offset==-1){
            gen("# write global boolean "+n->a_write.var->name);
            gen("lw $t0, "+n->a_write.var->name);
        }
        else {
            gen("# write local boolean "+n->a_write.var->name);
            gen("lw $t0,"+to_string(4*n->a_write.var->offset)+"($fp)");
        }
        gen("addiu $sp, $sp , -4");
        gen("sw $ra, 4($sp)");
        newLine
        gen("jal write_____boolean");
        gen("addiu $sp, $sp , 4");
        gen("lw $ra, 0($sp)");
        newLine
    }
    
    else if(n->a_write.var->getType()==TYPE_FLOAT){
        
        if(n->a_write.var->offset == -1  ){
            gen("# write global float "+n->a_write.var->name);
            gen("l.s $f12, "+n->a_write.var->name);
        }
        else{
            gen("# write local float "+n->a_write.var->name);
            gen("l.s $f12,"+to_string(4*n->a_write.var->offset)+"($fp)");
        }
        gen("li $v0, 2");
        syscall
    }
    
}

void CodeGenerator::gen_if(AST * n){
    newLine
    gen("# if statment");
    string altern_label = "if__"+makeLabel();
    string end_label = "end__if__"+makeLabel();
    generate(n->a_if.predicate);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("beqz $t0, "+altern_label);
    generate(n->a_if.conseq);
    gen ("j " +end_label);
    gen (altern_label+":");
    generate(n->a_if.altern);
    gen (end_label+":");
}

void CodeGenerator::gen_while(AST *n){
    newLine
    gen("# while statment");
    string while_label ="while__"+makeLabel();
    string end_label ="end__while__"+makeLabel();
    gen (while_label+":");
    generate (n->a_while.predicate);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("beqz $t0,"+end_label);
    generate (n->a_while.body);
    gen ("j "+while_label);
    gen (end_label+":");
}

void CodeGenerator::gen_for(AST *n){
    newLine
    gen("# for statment");
    string start_label= "for__"+makeLabel();
    string for_label  = "statr__for__"+makeLabel();
    string end_label  = "end__for__"+makeLabel();
    generate(n->a_for.lower_bound);
    gen("addiu $sp, $sp, 4");
    gen("lw $t1, 0($sp)");
    
    if(n->a_for.var->offset ==-1){
        gen("la $t0, "+n->a_for.var->name);
        gen("sw $t1, 0($t0)");
    }
    else
        gen("sw $t1,"+to_string(n->a_for.var->offset*4)+"($fp)");
    
    gen(start_label+" :");
    generate(n->a_for.upper_bound);
    gen("addiu $sp, $sp, 4");
    gen("lw $t0, 0($sp)");
    gen("ble $t1,$t0,"+for_label);
    gen("j "+end_label);
    gen(for_label+" :");
    generate(n->a_for.body);
    
    if(n->a_for.var->offset == -1){
        gen("la $t0, "+n->a_for.var->name);
        gen("lw $t1, 0($t0)");
        gen("addiu $t1,$t1,1");
        gen("sw $t1, 0($t0)");
    }
    else{
        gen("lw $t1,"+to_string(n->a_for.var->offset*4)+"($fp)");
        gen("addiu $t1,$t1,1");
        gen("sw $t1,"+to_string(n->a_for.var->offset*4)+"($fp)");
    }
    gen("j "+start_label);
    gen(end_label+" :");
}


void CodeGenerator::gen_call(AST *n){
    
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
    newLine
    int c=0;
    ast_list * arg = n->a_call.arg_list;
    while (arg->head) {
        c++;
        generate(arg->head);
        arg = arg->next;
    }
    
    gen ("jal " +n->a_call.callee->name);
    newLine
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


void CodeGenerator::gen_return(AST * n){
    newLine
    gen("# return");
    generate(n->a_return.expr);
    gen("addiu $sp, $sp, 4");

    j_type expect=n->a_return.expectReturnType;
    j_type real = n->a_return.realReturnType;
    
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


void CodeGenerator::generate(AST * n){
    if(n==nullptr)return;
    
    AST_type type = n->type;
    if (flag_data == 1 and type!=AST_CONST_DECL and  type != AST_VAR_DECL) {
        flag_data =0;
        flag_main++;
        newLine
        gen(".text");
        newLine
        if(flag_main==1){
            gen("jal main");
            newLine
            gen("exit:");
            gen("li $v0, 10");
            syscall
            newLine
        }
    }
    
    switch (type) {
            
        case AST_VAR_DECL:
            gen_var_decl(n);
            break;
            
        case AST_CONST_DECL:
            gen_constant(n);
            break;
            
        case AST_ROUTINE_DECL:
            gen_routine(n);
            break;
            
        case AST_BLOCK:
            gen_block(n);
            break;
            
        case AST_VAR:
            gen_var(n);
            break;
            
        case AST_INTEGER:
        case AST_BOOLEAN:
        case AST_FLOAT:
            gen_lit(n);
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
            gen_opreation(n);
            break;
            
        case AST_UMINUS:
        case AST_NOT:
            gen_unary_opreation(n);
            break;
            
        case AST_IF:
            gen_if(n);
            break;
            
        case AST_ASSIGN:
            gen_assign(n);
            break;
            
        case AST_WHILE:
            gen_while(n);
            break;
            
        case AST_FOR:
            gen_for(n);
            break;
            
        case AST_READ:
            gen_read(n);
            break;
            
        case AST_WRITE:
            gen_write(n);
            break;
            
        case AST_CALL:
            gen_call(n);
            break;
            
        case AST_RETURN:
            gen_return(n);
            break;
            
        default:
            break;
    }
}


string CodeGenerator:: makeLabel(){
    string label="label__"+to_string(lableCount++);
    return label;
}

void CodeGenerator::gen(string  massage){
    fout<<massage<<endl;
}

CodeGenerator::~CodeGenerator() {
    fout.close();
}







