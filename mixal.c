// TODO: mistake when input output

#include <stdio.h>
#include <ctype.h>
#include "mixal.h"

FILE *file1;

int tempVarCounter = 0;  // counter for generating unique temporary variables
int endifCounter = 0;
int repeatCounter = 0;
int trueCounter = 0;
int doneCounter = 0;

// function to get a new temporary variable for intermediate results
const char* getNewTempVar() {
    static char buffer[10];
    snprintf(buffer, sizeof(buffer), "T%d", tempVarCounter++);
    char *name = buffer;
    insertSymbol(buffer);
    return strdup(buffer);  // return a new unique temp variable name
}

void generateVariables(){
    for (int i = 0; i < table.size; i++) 
        fprintf(file1, "%s CON 0\n", table.symbols[i].name); 
}

void generateMixal(Node *node){
    file1 = fopen("result.mixal", "w");

    if (file1 == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    fprintf(file1, "\tORIG 1000\nSTART"); // start the memory location at 1000
    genMixal(node);
    generateVariables();
    fprintf(file1, "\tEND  START\n"); // newline at the end omg it doenst run without a newline at the end!!!!
    fclose(file1);
}

void genMixal(Node *node) {
    if (node == NULL) return;
    switch (node->type) {
        case NODE_SIMPLE_EXPRESSION:
            genMixal(node->data.simple_expression.term);  // evaluate the right hand
            if (node->data.simple_expression.operation == '+') {
                const char *tempVar = getNewTempVar();
                fprintf(file1, "\tSTA %s\n", tempVar);  // save the result of right hand
                genMixal(node->data.simple_expression.simple_expression);  // evaluate the left hand
                fprintf(file1, "\tADD %s\n", tempVar);  // add the saved right hand to the current result
            } else if (node->data.simple_expression.operation == '-') {
                const char *tempVar = getNewTempVar();
                fprintf(file1, "\tSTA %s\n", tempVar);  // save the result of right hand
                genMixal(node->data.simple_expression.simple_expression);  // evaluate the left hand
                fprintf(file1, "\tSUB %s\n", tempVar);  // subtract the saved right hand from the current result
            }
            break;
        case NODE_TERM:
            genMixal(node->data.term.factor);  // evaluate the right hand
            if (node->data.term.operation == '*') {
                const char *tempVar = getNewTempVar();
                fprintf(file1, "\tSTA %s\n", tempVar);  // save the result of right hand
                genMixal(node->data.term.term);  // evaluate the left hand
                fprintf(file1, "\tMUL %s\n", tempVar);  // multiply the current result by the saved right hand
                fprintf(file1, "\tSTX %s\n", tempVar); // the result of mul is stored in x so store it and load it back to a
                fprintf(file1, "\tLDA %s\n", tempVar); // and load it back to a
            } else if (node->data.term.operation == '/') {
                const char *tempVar = getNewTempVar();
                fprintf(file1, "\tSTA %s\n", tempVar);  // save the result of left hand 
                genMixal(node->data.term.term);  // evaluate the left hand
                const char *tempVar1 = getNewTempVar(); // save the result from the accumulator
                fprintf(file1, "\tSTA %s\n", tempVar1);  // store it 
                fprintf(file1, "\tLDX %s\n", tempVar1); // and load it to x so that division can be done
                fprintf(file1, "\tLDA =0=\n"); // make a register 0
                fprintf(file1, "\tDIV %s\n", tempVar);  // divide the current result by the saved right hand
            }
            break;
        case NODE_FACTOR:
            if (node->data.factor.identifier) {
                if(lookupSymbol(node->data.factor.identifier) == 0)
                    exit(0);
                fprintf(file1, "\tLDA %s\n", node->data.factor.identifier);  
            } else {
                fprintf(file1, "\tLDA =%d=\n", node->data.factor.num);  
            }
            break;
        case NODE_PROGRAM:
            genMixal(node->data.program.statement_sequence);
            break;
        case NODE_STATEMENT_SEQUENCE:
            genMixal(node->data.statement_sequence.statement_sequence);
            genMixal(node->data.statement_sequence.statement);
            break;
        case NODE_ASSIGN:
            genMixal(node->data.assign_statement.expression);
            fprintf(file1, "\tSTA %s\n", node->data.assign_statement.identifier);  // store the result from accumulator to the identifier
            break;
        case NODE_IF:
            genMixal(node->data.if_statement.expression);
            fprintf(file1, "\tCMPA 0\n");  // compare result to 0(false)
            if (node->data.if_statement.statement_sequence2 != NULL) { // if it has else
                fprintf(file1, "\tJE ELSE%d\n", endifCounter);  // jump to else if 0
                genMixal(node->data.if_statement.statement_sequence1); // if true do the statement sequence of the if block
                fprintf(file1, "\tJMP ENDIF%d\n", endifCounter);  // and skip else block
                fprintf(file1, "ELSE%d", endifCounter); // do the else block code
                genMixal(node->data.if_statement.statement_sequence2);
                fprintf(file1, "ENDIF%d NOP\n", endifCounter++);
            } else{
                fprintf(file1, "\tJE ENDIF%d\n", endifCounter);  // jump to end if 0
                genMixal(node->data.if_statement.statement_sequence1); // if true do the statement sequence of the if block
                fprintf(file1, "ENDIF%d NOP\n", endifCounter++);
            }
            break;
        case NODE_REPEAT:
            fprintf(file1, "REPEAT%d", repeatCounter);
            genMixal(node->data.repeat_statement.statement_sequence);
            genMixal(node->data.repeat_statement.expression);
            fprintf(file1, "\tCMPA 0\n");  // compare result to 0(false)
            fprintf(file1, "\tJE REPEAT%d\n", repeatCounter);  // if until condition is false loop (equal to 0 because we do cmpa 0)
            repeatCounter++;
            break;
        case NODE_READ:
            fprintf(file1, "\tIN 16\n"); // read from device 16 (card)
            fprintf(file1, "\tSTA %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            fprintf(file1, "\tLDA %s\n", node->data.read_statement.identifier);            
            fprintf(file1, "\tOUT 19\n");// write to device 16 (terminal)
            break;
        case NODE_RELATIONAL_EXPRESSION:
            genMixal(node->data.relational_expression.relational_expression);  // evaluate the left hand side
            const char *leftTempVar = getNewTempVar();
            fprintf(file1, "\tSTA %s\n", leftTempVar);  // store the result in a temporary variable
            genMixal(node->data.relational_expression.simple_expression);  // evaluate the right hand side
            const char *rightTempVar = getNewTempVar();
            fprintf(file1, "\tSTA %s\n", rightTempVar);  // store the result in another temporary variable
            fprintf(file1, "\tLDA %s\n", leftTempVar);  // load the left hand result into the accumulator
            if (node->data.relational_expression.operation == '<') {
                fprintf(file1, "\tCMPA %s\n", rightTempVar);  // compare accumulator with the right-hand result
                fprintf(file1, "\tJL TRUE%d\n", trueCounter); // if its less then jump to true
                fprintf(file1, "\tLDA  =0=\n"); 
                fprintf(file1, "\tJMP DONE%d\n", doneCounter);
                fprintf(file1, "TRUE%d LDA  =1=\n", trueCounter++);
                fprintf(file1, "DONE%d NOP\n", doneCounter++);
            }else if (node->data.relational_expression.operation == '='){
                fprintf(file1, "\tCMPA %s\n", rightTempVar);  // compare accumulator with the right-hand result
                fprintf(file1, "\tJE TRUE%d\n", trueCounter);  // if its equal jump to true
                fprintf(file1, "\tLDA  =0=\n"); 
                fprintf(file1, "\tJMP DONE%d\n", doneCounter);
                fprintf(file1, "TRUE%d LDA  =1=\n", trueCounter++);
                fprintf(file1, "DONE%d NOP\n", doneCounter++);
            }
            break;
        default:
            fprintf(stderr, "Unknown node type in MIXAL generator.\n");
    }
}