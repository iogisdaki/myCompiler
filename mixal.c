#include <stdio.h>
#include "mixal.h"

FILE *file1;

void generateMixal(ASTNode *node){
    file1 = fopen("result.txt", "w");

    if (file1 == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    printf("ORIG 1000\nSTART"); // start the memory location at 1000
    genMixal(node);
    printf("END  START");
    fclose(file1);
}

void genMixal(ASTNode *node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PROGRAM:
            genMixal(node->data.program.statement_sequence);
            break;
        case NODE_STATEMENT_SEQUENCE:
            genMixal(node->data.statement_sequence.statement_sequence);
            genMixal(node->data.statement_sequence.statement);
            break;
        case NODE_ASSIGN:
            printf("\tLDA ");
            genMixal(node->data.assign_statement.expression);
            printf("\tSTA %s\n", node->data.assign_statement.identifier);  // store the result from accumulator to the identifier
            break;
        case NODE_IF:
            genMixal(node->data.if_statement.expression);
            printf("CMPA 0\n");  // compare result to 0(false)
            printf("JGE ELSE_%p\n", (void *)node);  // if false jump to else
            genMixal(node->data.if_statement.statement_sequence1); // if true do the statement sequence of the if block
            printf("JMP ENDIF_%p\n", (void *)node);  // and skip else block
            printf("ELSE_%p:\n", (void *)node); // do the else code if it exists
            if (node->data.if_statement.statement_sequence2 != NULL) {
                genMixal(node->data.if_statement.statement_sequence2);
            }
            printf("ENDIF_%p:\n", (void *)node);
            break;
        case NODE_REPEAT:
            printf("REPEAT_%p:\n", (void *)node);
            genMixal(node->data.repeat_statement.statement_sequence);
            genMixal(node->data.repeat_statement.expression);
            printf("CMPA 0\n");  // compare result to 0(false)
            printf("JNE REPEAT_%p\n", (void *)node);  // if until condition is false loop
            break;
        case NODE_READ:
            printf("IN %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            printf("OUT %s\n", node->data.write_statement.identifier);
            break;
        case NODE_FACTOR:
            genMixal(node->data.factor.expression);
            if (node->data.factor.identifier != NULL) {
                printf("%s\n", node->data.factor.identifier); 
            } else {
                printf("=%d=\n", node->data.factor.num); // constant into the accumulator
            }
            break;
        case NODE_TERM:
            genMixal(node->data.term.term);
            if (node->data.term.operation == '*') 
                printf("\tMUL ");
            else if (node->data.term.operation == '/') 
                printf("\tDIV ");
            genMixal(node->data.term.factor);
            break;
        case NODE_SIMPLE_EXPRESSION:
            genMixal(node->data.simple_expression.simple_expression);
            if (node->data.simple_expression.operation == '+') 
                printf("\tADD ");
            else if (node->data.simple_expression.operation == '-') 
                printf("\tSUB ");
            genMixal(node->data.simple_expression.term);
            break;
        case NODE_RELATIONAL_EXPRESSION:
            genMixal(node->data.relational_expression.relational_expression);
            if (node->data.relational_expression.operation == '<' || node->data.relational_expression.operation == '=') 
                printf("\tCMPA %s\n", node->data.relational_expression.simple_expression);
            genMixal(node->data.relational_expression.simple_expression);
            break;
        default:
            fprintf(stderr, "Unknown node type in MIXAL generator.\n");
    }
}