#include <stdio.h>
#include "mixal.h"

FILE *file1;

void generateMixal(ASTNode *node){
    file1 = fopen("result.txt", "w");

    if (file1 == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    genMixal(node);
    fclose(file1);
}

/* recursivelly generate mixal code from the ast */
void genMixal(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:
            genMixal(node->data.program.statement_sequence);
            fprintf(file1, "HLT\n");  // end of program
            break;

        case NODE_STATEMENT_SEQUENCE:
            genMixal(node->data.statement_sequence.statement_sequence);
            genMixal(node->data.statement_sequence.statement);
            break;

        case NODE_ASSIGN:
            genMixal(node->data.assign_statement.expression);
            fprintf(file1, "STA %s\n", node->data.assign_statement.identifier);  // store identifier in accumulator
            break;

        case NODE_IF: {
            char *elseLabel = "L1";
            char *endLabel = "L2";

            // code for the if condition
            genMixal(node->data.if_statement.expression);
            fprintf(file1, "JZ %s\n", elseLabel);  // if the condition is 0(false), jump to else

            // code for the then part
            genMixal(node->data.if_statement.statement_sequence1);
            fprintf(file1, "JMP %s\n", endLabel);  // after then, jump to the end

            // code for the else part
            fprintf(file1, "%s:\n", elseLabel);
            if (node->data.if_statement.statement_sequence2 != NULL)
                genMixal(node->data.if_statement.statement_sequence2);

            // end 
            fprintf(file1, "%s:\n", endLabel);
            break;
        }

        case NODE_REPEAT: {
            char *startLabel = "L3";
            char *endLabel = "L4";

            fprintf(file1, "%s:\n", startLabel);  // start of repeat loop
            genMixal(node->data.repeat_statement.statement_sequence);  

            genMixal(node->data.repeat_statement.expression);
            fprintf(file1, "JZ %s\n", startLabel);  // repeat if contition is (0)false

            fprintf(file1, "%s:\n", endLabel);  // end
            break;
        }

        case NODE_READ:
            fprintf(file1, "IN %s,1\n", node->data.read_statement.identifier);  // read input from device 1 (keyboard)
            break;

        case NODE_WRITE:
            fprintf(file1, "OUT %s,3\n", node->data.write_statement.identifier);  // write output to device 3 (console)
            break;

        case NODE_FACTOR:
            if (node->data.factor.identifier) {
                fprintf(file1, "LDA %s\n", node->data.factor.identifier);  // Load the variable
            } else {
                fprintf(file1, "LDA =%d\n", node->data.factor.num);  // Load the constant number
            }
            break;

        case NODE_TERM:
            genMixal(node->data.term.term);
            switch (node->data.term.operation) {
                case '*':
                    genMixal(node->data.term.factor);
                    fprintf(file1, "MUL\n");  
                    break;
                case '/':
                    genMixal(node->data.term.factor);
                    fprintf(file1, "DIV\n"); 
                    break;
            }
            break;

        case NODE_SIMPLE_EXPRESSION:
            if (node->data.term.operation == '+') {
                genMixal(node->data.term.term); 
                genMixal(node->data.term.factor);  
                fprintf(file1, "ADD\n");
            } else if (node->data.term.operation == '-') {
                genMixal(node->data.term.term);  
                genMixal(node->data.term.factor); 
                fprintf(file1, "SUB\n");  
            } else {
                genMixal(node->data.term.term);
            }
            break;

        case NODE_RELATIONAL_OPERATION:
            genMixal(node->data.relative_operation.term);
            genMixal(node->data.relative_operation.simple_expression);
            if (node->data.relative_operation.operation == '<') {
                fprintf(file1, "CMP\n"); 
                fprintf(file1, "JL \n"); 
            } else if (node->data.relative_operation.operation == '=') {
                fprintf(file1, "CMP\n"); 
                fprintf(file1, "JE \n");  
            }
            break;

        case NODE_EXPRESSION:
            genMixal(node->data.expression.relational_expression);
            break;

        default:
            fprintf(stderr, "Error: Unknown node type %d\n", node->type);
            break;
    }
}
