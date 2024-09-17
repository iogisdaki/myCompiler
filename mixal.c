#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mixal.h"

/* recursivelly generate mixal code from the ast */
void generateMIXAL(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:
            generateMIXAL(node->data.program.statement_sequence);
            printf("HLT\n");  // end of program
            break;

        case NODE_STATEMENT_SEQUENCE:
            generateMIXAL(node->data.statement_sequence.statement_sequence);
            generateMIXAL(node->data.statement_sequence.statement);
            break;

        case NODE_ASSIGN:
            generateMIXAL(node->data.assign_statement.expression);
            printf("STA %s\n", node->data.assign_statement.identifier);  // store identifier in accumulator
            break;

        case NODE_IF: {
            char *elseLabel = "L1";
            char *endLabel = "L2";

            // code for the if condition
            generateMIXAL(node->data.if_statement.expression);
            printf("JZ %s\n", elseLabel);  // if the condition is 0(false), jump to else

            // code for the then part
            generateMIXAL(node->data.if_statement.statement_sequence1);
            printf("JMP %s\n", endLabel);  // after then, jump to the end

            // code for the else part
            printf("%s:\n", elseLabel);
            if (node->data.if_statement.statement_sequence2 != NULL)
                generateMIXAL(node->data.if_statement.statement_sequence2);

            // end 
            printf("%s:\n", endLabel);
            break;
        }

        case NODE_REPEAT: {
            char *startLabel = "L3";
            char *endLabel = "L4";

            printf("%s:\n", startLabel);  // start of repeat loop
            generateMIXAL(node->data.repeat_statement.statement_sequence);  

            generateMIXAL(node->data.repeat_statement.expression);
            printf("JZ %s\n", startLabel);  // repeat if contition is (0)false

            printf("%s:\n", endLabel);  // end
            break;
        }

        case NODE_READ:
            printf("IN %s,1\n", node->data.read_statement.identifier);  // read input from device 1 (keyboard)
            break;

        case NODE_WRITE:
            printf("OUT %s,3\n", node->data.write_statement.identifier);  // write output to device 3 (console)
            break;

        case NODE_FACTOR:
            if (node->data.factor.identifier) {
                printf("LDA %s\n", node->data.factor.identifier);  // Load the variable
            } else {
                printf("LDA =%d\n", node->data.factor.num);  // Load the constant number
            }
            break;

        case NODE_TERM:
            generateMIXAL(node->data.term.term);
            switch (node->data.term.operation) {
                case '*':
                    generateMIXAL(node->data.term.factor);
                    printf("MUL\n");  
                    break;
                case '/':
                    generateMIXAL(node->data.term.factor);
                    printf("DIV\n"); 
                    break;
            }
            break;

        case NODE_SIMPLE_EXPRESSION:
            if (node->data.term.operation == '+') {
                generateMIXAL(node->data.term.term); 
                generateMIXAL(node->data.term.factor);  
                printf("ADD\n");
            } else if (node->data.term.operation == '-') {
                generateMIXAL(node->data.term.term);  
                generateMIXAL(node->data.term.factor); 
                printf("SUB\n");  
            } else {
                generateMIXAL(node->data.term.term);
            }
            break;

        case NODE_RELATIONAL_OPERATION:
            generateMIXAL(node->data.relative_operation.term);
            generateMIXAL(node->data.relative_operation.simple_expression);
            if (node->data.relative_operation.operation == '<') {
                printf("CMP\n"); 
                printf("JL \n"); 
            } else if (node->data.relative_operation.operation == '=') {
                printf("CMP\n"); 
                printf("JE \n");  
            }
            break;

        case NODE_EXPRESSION:
            generateMIXAL(node->data.expression.relational_expression);
            break;

        default:
            fprintf(stderr, "Error: Unknown node type %d\n", node->type);
            break;
    }
}
