//
//  SemanticChecker.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 30/08/2023.
//

#include "SemanticChecker.h"


SemanticChecker::SemanticChecker(FileDescriptor * fileDescriptor){
    this->fileDescriptor = fileDescriptor;
}


void SemanticChecker::semanticError(FileDescriptor * fileDescriptor , string message) {
    fileDescriptor->reportError("Semantic Error: "+message);
    cout<<endl;
    remove("symbol tabel.txt");
    remove("out.txt");
    remove("assmbly.asm");
    exit(EXIT_FAILURE);
}

void SemanticChecker::semanticWarning(FileDescriptor * fileDescriptor , string message) {
    fileDescriptor->reportWarning(message);
    cout<<endl;
}


bool SemanticChecker::routineContainEnoughReturn(AST * n ){
    ast_list * statement = n->a_block.stmts;
    if(!statement->head)return false;
    bool flag =false;
    while (statement->next) {
        
        if (statement->head->type == AST_RETURN)
            return true;
        
        else if(statement->head->type == AST_BLOCK)
            flag = routineContainEnoughReturn(statement->head);
        
        else if(statement->head->type == AST_IF) {
            bool flag1 = false,flag2 = false;
            if(statement->head->a_if.altern){
                if(statement->head->a_if.altern->type == AST_RETURN)
                    flag1 = true;
                
                if(statement->head->a_if.altern->type == AST_BLOCK)
                    flag1 = routineContainEnoughReturn(statement->head->a_if.altern);
            }
              if(statement->head->a_if.conseq){
                if(statement->head->a_if.conseq->type == AST_RETURN)
                    flag2 = true;

                if (statement->head->a_if.conseq->type == AST_BLOCK)
                    flag2 = routineContainEnoughReturn(statement->head->a_if.conseq);
            }
            
            flag = flag1 and flag2;
        }
        
        if(flag)
            return true;
        else  statement = statement->next;
    }
    return false;
}




void SemanticChecker:: cheakAllReturnType(AST * n ,J_TYPE expectedType){
    if(n == nullptr)
        return;
    
    else if(n->type == AST_RETURN){
        J_TYPE return_type = expressionType(n->a_return.expr);
        n->a_return.realReturnType=return_type;
        n->a_return.expectReturnType=expectedType;
        
        if (expectedType != TYPE_NONE) {
            if (return_type != expectedType){
                if(expectedType == TYPE_INTEGER and return_type== TYPE_FLOAT)return;
                
                if(expectedType == TYPE_FLOAT and return_type == TYPE_INTEGER)return;
                
                semanticError(fileDescriptor, "return value mismatch type");}
        }
        else
            if (return_type != expectedType)
                semanticError(fileDescriptor, "procedure cannot return value");
    }
    
    else if(n->type == AST_BLOCK){
        ast_list * statement = n->a_block.stmts;
        if(statement == nullptr) return;
        while (statement->next) {
            
            if (statement->head->type == AST_RETURN or statement->head->type == AST_BLOCK) {
                cheakAllReturnType(statement->head,expectedType);
            }
            
            else if (statement->head->type == AST_WHILE){
                cheakAllReturnType(statement->head->a_while.body,expectedType);
                
            }
            
            else if (statement->head->type == AST_FOR){
                cheakAllReturnType(statement->head->a_for.body,expectedType);
            }
            
            else if (statement->head->type == AST_IF){
                cheakAllReturnType(statement->head->a_if.conseq,expectedType);
                cheakAllReturnType(statement->head->a_if.altern,expectedType);
            }
            
            statement = statement->next;
        }
    }
}


void  SemanticChecker::checkStatement (AST * n , J_TYPE expectedType){
    
    switch (n->type) {
            
        case AST_ROUTINE_DECL:{
            
            // its procedure
            if(expectedType == TYPE_NONE ){
                //add return if procedure not contain return statment
                if (!routineContainEnoughReturn(n->a_routine_decl.body)){
                    ast_list * statement = n->a_routine_decl.body->a_block.stmts;
                    
                    if (statement->head)
                        while (statement->next) {
                            statement = statement->next;
                        }
                    statement->head=makeAstNode(AST_RETURN,nullptr);
                    statement->next=new ast_list;
                    statement->next->head=nullptr;
                }
            }
            // its function
            else{
                if (!routineContainEnoughReturn(n->a_routine_decl.body))
                    semanticError(fileDescriptor, "function must return value");
            }
            cheakAllReturnType(n->a_routine_decl.body,expectedType);
            return;
        }
            
        case AST_ASSIGN:{
            
            if(n->a_assign.lhs->type == STE_CONST )
                semanticError(fileDescriptor, "constant cannot be redeclared");
            
            if(expectedType == expressionType(n->a_assign.rhs)){
                n->a_assign.rightType=expectedType;
                return;
            }
            if(expectedType == TYPE_INTEGER and expressionType(n->a_assign.rhs) == TYPE_FLOAT){
                n->a_assign.rightType = TYPE_FLOAT;
                return;
            }
            
            if(expectedType == TYPE_FLOAT and expressionType(n->a_assign.rhs) == TYPE_INTEGER){
                n->a_assign.rightType = TYPE_INTEGER;
                return;
            }
            semanticError(fileDescriptor, "Assign statement type mismatch");
        }
            
            
        case AST_IF:{
            if(expressionType(n->a_if.predicate) == expectedType)
                return;
            
            semanticError(fileDescriptor, "if statement predicate must be boolean type");
        }
            
            
        case AST_WHILE:{
            
            if(expressionType(n->a_while.predicate) == expectedType)
                return;
            
            semanticError(fileDescriptor, "while statement predicate must be boolean type");
        }
        case AST_READ:
            if(n->a_read.var->type==STE_CONST)
                semanticError(fileDescriptor, "constant cannot be redeclared");
            return;
            
            
        case AST_FOR:{
            if(n->a_for.var->type == STE_CONST)
                semanticError(fileDescriptor, "cannot use constant var");
            
            if(n->a_for.var->getType() == expectedType){
                if( expressionType( n->a_for.lowerBound) == TYPE_INTEGER and  expressionType( n->a_for.upperBound)==TYPE_INTEGER)return;
                
                semanticError(fileDescriptor, "lower and upper bound must be integers");
            }
            semanticError(fileDescriptor, "variable in a for statement must be integer");
            
        }
            
        case AST_CALL:{
            
            if(n->a_call.callee->getType() != TYPE_NONE and expectedType !=TYPE_NONE)
                semanticWarning(fileDescriptor, "function result is not used ");
            
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
            
            if(number_of_args != number_of_formals)semanticError(fileDescriptor, "invalid number of args");
            else{
                arg = n->a_call.arg_list;
                formal= n->a_call.callee->routine.formals;
                while (formal->head ) {
                    arg->type = expressionType(arg->head);
                    if(formal->head->getType()!=expressionType(arg->head)){
                        if (!((formal->head->getType()==TYPE_INTEGER and expressionType(arg->head) == TYPE_FLOAT ) or
                              (formal->head->getType()==TYPE_FLOAT and expressionType(arg->head) == TYPE_INTEGER )))
                            semanticError(fileDescriptor, "invalid type of args");
                    }
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


J_TYPE SemanticChecker::expressionType (AST *n){
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
        J_TYPE left_type  = expressionType (n->a_binary_op.larg);
        J_TYPE right_type = expressionType (n->a_binary_op.rarg);
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
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;
        }
        
        
        else semanticError(fileDescriptor, "type mismatch around + operater");
    }
    
    
    else if (n->type == AST_MINUS or n->type == AST_TIMES or n->type == AST_DIVIDE ){
        J_TYPE left_type  = expressionType (n->a_binary_op.larg);
        J_TYPE right_type = expressionType (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
            n->a_binary_op.rel_type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) {
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else {
            if (n->type == AST_MINUS) semanticError(fileDescriptor, "type mismatch around - operater");
            else if (n->type == AST_TIMES)semanticError(fileDescriptor, "type mismatch around * operater");
            else semanticError(fileDescriptor, "type mismatch around / operater");
        }
    }
    
    else if (n->type == AST_EQ  or n->type == AST_NEQ ){
        J_TYPE left_type  = expressionType (n->a_binary_op.larg);
        J_TYPE right_type = expressionType (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if((left_type == TYPE_INTEGER && right_type == TYPE_INTEGER)
           or (left_type == TYPE_STRING && right_type == TYPE_STRING)
           or (left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN)
           or(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) ){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN; }
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        
        else {
            if (n->type == AST_EQ )
                semanticError(fileDescriptor, "type mismatch around = operater");
            else
                semanticError(fileDescriptor, "type mismatch around != operater");
        }
    }
    
    else if (n->type == AST_LT  or n->type == AST_LE or n->type == AST_GT or n->type == AST_GE){
        J_TYPE left_type  = expressionType (n->a_binary_op.larg);
        J_TYPE right_type = expressionType (n->a_binary_op.rarg);
        
        n->a_binary_op.l_type=left_type;
        n->a_binary_op.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        else {
            if (n->type == AST_LT )
                semanticError(fileDescriptor, "type mismatch around < operater");
            else if (n->type == AST_LE )
                semanticError(fileDescriptor, "type mismatch around <= operater");
            else if (n->type == AST_GT )
                semanticError(fileDescriptor, "type mismatch around > operater");
            else
                semanticError(fileDescriptor, "type mismatch around >= operater");
        }
    }
    
    else if (n->type == AST_AND or n->type == AST_OR){
        
        J_TYPE left_type  = expressionType (n->a_binary_op.larg);
        J_TYPE right_type = expressionType (n->a_binary_op.rarg);
        
        if(left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN) {
            n->a_binary_op.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;
        }
        
        else {
            if (n->type == AST_AND)
                semanticError(fileDescriptor, "type mismatch around and operater");
            else
                semanticError(fileDescriptor, "type mismatch around or operater");
        }
    }
    
    else if(n->type ==AST_NOT) {
        J_TYPE type  = expressionType (n->a_unary_op.arg);
        
        if(type == TYPE_BOOLEAN ){
            n->a_unary_op.type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        else
            semanticError(fileDescriptor, "type mismatch around not operater");
    }
    
    else if(n->type ==AST_UMINUS) {
        J_TYPE type  = expressionType (n->a_unary_op.arg);
        
        if(type == TYPE_INTEGER ){
            n->a_unary_op.type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(type == TYPE_FLOAT ){
            n->a_unary_op.type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else
            semanticError(fileDescriptor, "type mismatch");
    }
    
    return TYPE_NONE;
}
