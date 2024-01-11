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


bool SemanticChecker::routineContainEnoughReturn(Ast * n ){
    AstList * statement = n->aBlock.stmts;
    if(!statement->head)return false;
    bool flag =false;
    while (statement->next) {
        
        if (statement->head->type == AST_RETURN)
            return true;
        
        else if(statement->head->type == AST_BLOCK)
            flag = routineContainEnoughReturn(statement->head);
        
        else if(statement->head->type == AST_IF) {
            bool flag1 = false,flag2 = false;
            if(statement->head->aIf.altern){
                if(statement->head->aIf.altern->type == AST_RETURN)
                    flag1 = true;
                
                if(statement->head->aIf.altern->type == AST_BLOCK)
                    flag1 = routineContainEnoughReturn(statement->head->aIf.altern);
            }
              if(statement->head->aIf.conseq){
                if(statement->head->aIf.conseq->type == AST_RETURN)
                    flag2 = true;

                if (statement->head->aIf.conseq->type == AST_BLOCK)
                    flag2 = routineContainEnoughReturn(statement->head->aIf.conseq);
            }
            
            flag = flag1 and flag2;
        }
        
        if(flag)
            return true;
        else  statement = statement->next;
    }
    return false;
}




void SemanticChecker:: cheakAllReturnType(Ast * n ,J_TYPE expectedType){
    if(n == nullptr)
        return;
    
    else if(n->type == AST_RETURN){
        J_TYPE return_type = expressionType(n->aReturn.expr);
        n->aReturn.realReturnType=return_type;
        n->aReturn.expectReturnType=expectedType;
        
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
        AstList * statement = n->aBlock.stmts;
        if(statement == nullptr) return;
        while (statement->next) {
            
            if (statement->head->type == AST_RETURN or statement->head->type == AST_BLOCK) {
                cheakAllReturnType(statement->head,expectedType);
            }
            
            else if (statement->head->type == AST_WHILE){
                cheakAllReturnType(statement->head->aWhile.body,expectedType);
                
            }
            
            else if (statement->head->type == AST_FOR){
                cheakAllReturnType(statement->head->aFor.body,expectedType);
            }
            
            else if (statement->head->type == AST_IF){
                cheakAllReturnType(statement->head->aIf.conseq,expectedType);
                cheakAllReturnType(statement->head->aIf.altern,expectedType);
            }
            
            statement = statement->next;
        }
    }
}


void  SemanticChecker::checkStatement (Ast * n , J_TYPE expectedType){
    
    switch (n->type) {
            
        case AST_ROUTINE_DECL:{
            
            // its procedure
            if(expectedType == TYPE_NONE ){
                //add return if procedure not contain return statment
                if (!routineContainEnoughReturn(n->aRoutineDecl.body)){
                    AstList * statement = n->aRoutineDecl.body->aBlock.stmts;
                    
                    if (statement->head)
                        while (statement->next) {
                            statement = statement->next;
                        }
                    statement->head=makeAstNode(AST_RETURN,nullptr);
                    statement->next=new AstList;
                    statement->next->head=nullptr;
                }
            }
            // its function
            else{
                if (!routineContainEnoughReturn(n->aRoutineDecl.body))
                    semanticError(fileDescriptor, "function must return value");
            }
            cheakAllReturnType(n->aRoutineDecl.body,expectedType);
            return;
        }
            
        case AST_ASSIGN:{
            
            if(n->aAssign.lhs->type == STE_CONST )
                semanticError(fileDescriptor, "constant cannot be redeclared");
            
            if(expectedType == expressionType(n->aAssign.rhs)){
                n->aAssign.rightType=expectedType;
                return;
            }
            if(expectedType == TYPE_INTEGER and expressionType(n->aAssign.rhs) == TYPE_FLOAT){
                n->aAssign.rightType = TYPE_FLOAT;
                return;
            }
            
            if(expectedType == TYPE_FLOAT and expressionType(n->aAssign.rhs) == TYPE_INTEGER){
                n->aAssign.rightType = TYPE_INTEGER;
                return;
            }
            semanticError(fileDescriptor, "Assign statement type mismatch");
        }
            
            
        case AST_IF:{
            if(expressionType(n->aIf.predicate) == expectedType)
                return;
            
            semanticError(fileDescriptor, "if statement predicate must be boolean type");
        }
            
            
        case AST_WHILE:{
            
            if(expressionType(n->aWhile.predicate) == expectedType)
                return;
            
            semanticError(fileDescriptor, "while statement predicate must be boolean type");
        }
        case AST_READ:
            if(n->aRead.var->type==STE_CONST)
                semanticError(fileDescriptor, "constant cannot be redeclared");
            return;
            
            
        case AST_FOR:{
            if(n->aFor.var->type == STE_CONST)
                semanticError(fileDescriptor, "cannot use constant var");
            
            if(n->aFor.var->getType() == expectedType){
                if( expressionType( n->aFor.lowerBound) == TYPE_INTEGER and  expressionType( n->aFor.upperBound)==TYPE_INTEGER)return;
                
                semanticError(fileDescriptor, "lower and upper bound must be integers");
            }
            semanticError(fileDescriptor, "variable in a for statement must be integer");
            
        }
            
        case AST_CALL:{
            
            if(n->aCall.callee->getType() != TYPE_NONE and expectedType !=TYPE_NONE)
                semanticWarning(fileDescriptor, "function result is not used ");
            
            AstList * arg = n->aCall.argList;
            SteList * formal= n->aCall.callee->routine.formals;
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
                arg = n->aCall.argList;
                formal= n->aCall.callee->routine.formals;
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


J_TYPE SemanticChecker::expressionType (Ast *n){
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
    
    else if (n->type == AST_VAR and ( n->aVar.var->type == STE_VAR or n->aVar.var->type == STE_CONST))
        return n->aVar.var->getType();
    
    else if (n->type == AST_CALL)
        return n->aCall.callee->getType();
    
    else if (n->type == AST_PLUS ){
        J_TYPE left_type  = expressionType (n->aBinaryOp.larg);
        J_TYPE right_type = expressionType (n->aBinaryOp.rarg);
        n->aBinaryOp.l_type=left_type;
        n->aBinaryOp.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER){
            n->aBinaryOp.rel_type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        else if(left_type == TYPE_STRING && right_type == TYPE_STRING){
            n->aBinaryOp.rel_type=TYPE_STRING;
            return TYPE_STRING;}
        
        else if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT){
            n->aBinaryOp.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->aBinaryOp.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;
        }
        
        
        else semanticError(fileDescriptor, "type mismatch around + operater");
    }
    
    
    else if (n->type == AST_MINUS or n->type == AST_TIMES or n->type == AST_DIVIDE ){
        J_TYPE left_type  = expressionType (n->aBinaryOp.larg);
        J_TYPE right_type = expressionType (n->aBinaryOp.rarg);
        
        n->aBinaryOp.l_type=left_type;
        n->aBinaryOp.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
            n->aBinaryOp.rel_type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) {
            n->aBinaryOp.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->aBinaryOp.rel_type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else {
            if (n->type == AST_MINUS) semanticError(fileDescriptor, "type mismatch around - operater");
            else if (n->type == AST_TIMES)semanticError(fileDescriptor, "type mismatch around * operater");
            else semanticError(fileDescriptor, "type mismatch around / operater");
        }
    }
    
    else if (n->type == AST_EQ  or n->type == AST_NEQ ){
        J_TYPE left_type  = expressionType (n->aBinaryOp.larg);
        J_TYPE right_type = expressionType (n->aBinaryOp.rarg);
        
        n->aBinaryOp.l_type=left_type;
        n->aBinaryOp.r_type=right_type;
        
        if((left_type == TYPE_INTEGER && right_type == TYPE_INTEGER)
           or (left_type == TYPE_STRING && right_type == TYPE_STRING)
           or (left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN)
           or(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT) ){
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN; }
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        
        else {
            if (n->type == AST_EQ )
                semanticError(fileDescriptor, "type mismatch around = operater");
            else
                semanticError(fileDescriptor, "type mismatch around != operater");
        }
    }
    
    else if (n->type == AST_LT  or n->type == AST_LE or n->type == AST_GT or n->type == AST_GE){
        J_TYPE left_type  = expressionType (n->aBinaryOp.larg);
        J_TYPE right_type = expressionType (n->aBinaryOp.rarg);
        
        n->aBinaryOp.l_type=left_type;
        n->aBinaryOp.r_type=right_type;
        
        if(left_type == TYPE_INTEGER && right_type == TYPE_INTEGER){
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        if(left_type == TYPE_FLOAT && right_type == TYPE_FLOAT){
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        
        else if((left_type == TYPE_FLOAT && right_type == TYPE_INTEGER)
                or( left_type == TYPE_INTEGER && right_type == TYPE_FLOAT)){
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
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
        
        J_TYPE left_type  = expressionType (n->aBinaryOp.larg);
        J_TYPE right_type = expressionType (n->aBinaryOp.rarg);
        
        if(left_type == TYPE_BOOLEAN && right_type == TYPE_BOOLEAN) {
            n->aBinaryOp.rel_type=TYPE_BOOLEAN;
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
        J_TYPE type  = expressionType (n->aUnaryOp.arg);
        
        if(type == TYPE_BOOLEAN ){
            n->aUnaryOp.type=TYPE_BOOLEAN;
            return TYPE_BOOLEAN;}
        else
            semanticError(fileDescriptor, "type mismatch around not operater");
    }
    
    else if(n->type ==AST_UMINUS) {
        J_TYPE type  = expressionType (n->aUnaryOp.arg);
        
        if(type == TYPE_INTEGER ){
            n->aUnaryOp.type=TYPE_INTEGER;
            return TYPE_INTEGER;}
        
        if(type == TYPE_FLOAT ){
            n->aUnaryOp.type=TYPE_FLOAT;
            return TYPE_FLOAT;}
        
        else
            semanticError(fileDescriptor, "type mismatch");
    }
    
    return TYPE_NONE;
}
