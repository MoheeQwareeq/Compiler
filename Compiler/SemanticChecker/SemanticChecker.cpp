//
//  SemanticChecker.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 30/08/2023.
//

#include "SemanticChecker.h"


SemanticChecker::SemanticChecker(FileDescriptor* fd){
    fileDescriptor = fd;
}


void SemanticChecker::semantic_error(FileDescriptor *fd , string message) {
    fd->reportError("Semantic Error: "+message);
    cout<<endl;
    remove("out.txt");
    remove("assmbly.asm");
    exit(EXIT_FAILURE);
}

void SemanticChecker::semantic_warning(FileDescriptor *fd , string message) {
    fd->reportWarning(message);
    cout<<endl;
}


void  SemanticChecker::check_Statement (AST * n , j_type expectedType){
    
    switch (n->type) {
            
        case AST_ROUTINE_DECL:{
            ast_list *statement = n->a_routine_decl.body->a_block.stmts;
            while (statement->next) {
                if (statement->head->type == AST_RETURN) {
                    j_type return_type = expression_type(statement->head->a_return.expr);
                    statement->head->a_return.realReturnType=return_type;
                    statement->head->a_return.expectReturnType=expectedType;
                    
                    if (expectedType != TYPE_NONE) {
                        if (return_type != expectedType){
                            if(expectedType == TYPE_INTEGER and return_type== TYPE_FLOAT)
                                return;
                            
                            if(expectedType == TYPE_FLOAT and return_type == TYPE_INTEGER)
                                return;
                            semantic_error(fileDescriptor, "return value mismatch");}
                        return;
                    }
                    else
                        if (return_type != expectedType)
                            semantic_error(fileDescriptor, "procedure cannot return value");
                    
                }
                statement = statement->next;
            }
            
            if (expectedType != TYPE_NONE)
                semantic_error(fileDescriptor, "function must return value");
            
        }
            
        case AST_ASSIGN:{
            
            if(n->a_assign.lhs->type == STE_CONST )
                semantic_error(fileDescriptor, "constant cannot be redeclared");
            
            if(expectedType == expression_type(n->a_assign.rhs)){
                n->a_assign.rightType=expectedType;
                return;
            }
            if(expectedType == TYPE_INTEGER and expression_type(n->a_assign.rhs) == TYPE_FLOAT){
                n->a_assign.rightType = TYPE_FLOAT;
                return;
            }
            
            if(expectedType == TYPE_FLOAT and expression_type(n->a_assign.rhs) == TYPE_INTEGER){
                n->a_assign.rightType = TYPE_INTEGER;
                return;
            }
            semantic_error(fileDescriptor, "Assign statement type mismatch");
        }
            
            
        case AST_IF:{
            if(expression_type(n->a_if.predicate) == expectedType)
                return;
            
            semantic_error(fileDescriptor, "if statement predicate must be boolean type");
        }
            
            
        case AST_WHILE:{
            
            if(expression_type(n->a_while.predicate) == expectedType)
                return;
            
            semantic_error(fileDescriptor, "while statement predicate must be boolean type");
        }
        case AST_READ:
            if(n->a_read.var->type==STE_CONST)
                semantic_error(fileDescriptor, "constant cannot be redeclared");
            return;
            
            
        case AST_FOR:{
            if(n->a_for.var->type == STE_CONST)
                semantic_error(fileDescriptor, "cannot use constant var");
            
            if(n->a_for.var->getType() == expectedType){
                if( expression_type( n->a_for.lower_bound) == TYPE_INTEGER and  expression_type( n->a_for.upper_bound)==TYPE_INTEGER)return;
                
                semantic_error(fileDescriptor, "lower and upper bound must be integers");
            }
            semantic_error(fileDescriptor, "variable in a for statement must be integer");
            
        }
            
        case AST_CALL:{
            
            if(n->a_call.callee->getType() != TYPE_NONE && expectedType !=TYPE_NONE)//function and come frome statment
                semantic_warning(fileDescriptor, "function result is not used ");
            
            ast_list * arg = n->a_call.arg_list;
            ste_list * formal= n->a_call.callee->routine.formals;
            int number_of_args=0;
            int number_of_formals=0;
            
            while (formal->head ) {
                number_of_formals++;
                formal = formal->next;
            }
            
            while (arg->next ) {
                number_of_args++;
                arg = arg->next;
            }
            
            if(number_of_args != number_of_formals)semantic_error(fileDescriptor, "invalid number of args");
            else{
                arg = n->a_call.arg_list;
                formal= n->a_call.callee->routine.formals;
                while (formal->head ) {
                    
                    if(formal->head->getType()!=expression_type(arg->head)){
                        if (!((formal->head->getType()==TYPE_INTEGER and expression_type(arg->head) == TYPE_FLOAT ) or
                              (formal->head->getType()==TYPE_FLOAT and expression_type(arg->head) == TYPE_INTEGER )))
                            semantic_error(fileDescriptor, "invalid type of args");}
                    arg = arg->next;
                    formal = formal->next;
                }
            }
            return;
        }
            
        default:
            return;
    }
}


j_type SemanticChecker::expression_type (AST *n){
    if(!n)
        return TYPE_NONE;
    
    if(n->type == AST_INTEGER)
        return TYPE_INTEGER;
    
    else if (n->type == AST_BOOLEAN)
        return TYPE_BOOLEAN;
    
    else if (n->type == AST_STRING)
        return TYPE_STRING;
    
    else if(n->type == AST_FLOAT)
        return TYPE_FLOAT;
    
    else if (n->type == AST_VAR and ( n->a_var.var->type == STE_VAR or n->a_var.var->type == STE_CONST))
        return n->a_var.var->getType();
    
    else if (n->type == AST_CALL)
        return n->a_call.callee->getType();
    
    else if (n->type == AST_PLUS ){
        j_type left_type  = expression_type (n->a_binary_op.larg);
        j_type right_type = expression_type (n->a_binary_op.rarg);
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;

        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER){
            n->a_binary_op.rel_type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        else if(left_type == TYPE_STRING && right_type == TYPE_STRING){
            n->a_binary_op.rel_type=TYPE_STRING;
            return TYPE_STRING;}
        
        else if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT){
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        //here is convert int to float
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;
        }
        
        
        else semantic_error(fileDescriptor, "type mismatch around + operater");
    }
    
    
    else if (n->type == AST_MINUS or n->type == AST_TIMES or n->type == AST_DIVIDE ){
        j_type left_type  = expression_type (n->a_binary_op.larg);
        j_type right_type = expression_type (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
            n->a_binary_op.rel_type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) {
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        //here is convert int to float
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else {
            if (n->type == AST_MINUS) semantic_error(fileDescriptor, "type mismatch around - operater");
            else if (n->type == AST_TIMES)semantic_error(fileDescriptor, "type mismatch around * operater");
            else semantic_error(fileDescriptor, "type mismatch around / operater");
        }
    }
    
    else if (n->type == AST_EQ  or n->type == AST_NEQ ){
        j_type left_type  = expression_type (n->a_binary_op.larg);
        j_type right_type = expression_type (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if((left_type == TYPE_INTEGER && right_type == TYPE_INTEGER)
           or (left_type == TYPE_STRING && right_type == TYPE_STRING)
           or (left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN)
           or(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) ){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN; }
        
        //here is convert int to float
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        
        else {
            if (n->type == AST_EQ )
                semantic_error(fileDescriptor, "type mismatch around = operater");
            else
                semantic_error(fileDescriptor, "type mismatch around != operater");
        }
    }
    
    else if (n->type == AST_LT  or n->type == AST_LE or n->type == AST_GT or n->type == AST_GE){
        j_type left_type  = expression_type (n->a_binary_op.larg);
        j_type right_type = expression_type (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        //here is convert int to float
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        else {
            if (n->type == AST_LT )
                semantic_error(fileDescriptor, "type mismatch around < operater");
            else if (n->type == AST_LE )
                semantic_error(fileDescriptor, "type mismatch around <= operater");
            else if (n->type == AST_GT )
                semantic_error(fileDescriptor, "type mismatch around > operater");
            else
                semantic_error(fileDescriptor, "type mismatch around >= operater");
        }
    }
    
    else if (n->type == AST_AND or n->type == AST_OR){
        
        j_type left_type  = expression_type (n->a_binary_op.larg);
        j_type right_type = expression_type (n->a_binary_op.rarg);
        
        if(left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN) {
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;
        }
        
        else {
            if (n->type == AST_AND)
                semantic_error(fileDescriptor, "type mismatch around and operater");
            else
                semantic_error(fileDescriptor, "type mismatch around or operater");}
    }
    
    else if(n->type ==AST_NOT) {
        j_type type  = expression_type (n->a_unary_op.arg);
        
        if(type == TYPE_BOOLEAN ){
            n->a_unary_op.type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        else
            semantic_error(fileDescriptor, "type mismatch around not operater");
    }
    
    else if(n->type ==AST_UMINUS) {
        j_type type  = expression_type (n->a_unary_op.arg);
        
        if(type == TYPE_INTEGER ){
            n->a_unary_op.type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(type == TYPE_FLOAT ){
            n->a_unary_op.type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else
            semantic_error(fileDescriptor, "type mismatch");
    }
    
    return TYPE_NONE;
}
