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
            printf("LDA %s\n", node->data.assign_statement.identifier); // load the result from the accumulator
            genMixalExpression(node->data.assign_statement.expression);
            printf("STA %s\n", node->data.assign_statement.identifier);  // store the result to accumulator
            break;
        case NODE_IF:
            genMixalExpression(node->data.if_statement.expression);
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
            genMixalExpression(node->data.repeat_statement.expression);
            printf("CMPA 0\n");  // compare result to 0(false)
            printf("JNE REPEAT_%p\n", (void *)node);  // if until condition is false loop
            break;
        case NODE_READ:
            printf("IN %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            printf("OUT %s\n", node->data.write_statement.identifier);
            break;
        default:
            fprintf(stderr, "Unknown node type in MIXAL generator.\n");
    }
}

void genMixalExpression(ASTNode *node) {
    if (node == NULL) return;
    if (node->type == NODE_RELATIONAL_OPERATION) {
            genMixalExpression(node->data.relative_operation.term);
            genMixalExpression(node->data.relative_operation.simple_expression);
            if (node->data.relative_operation.operation == '<' || node->data.relative_operation.operation == '=') 
                printf("CMPA %s\n", node->data.relative_operation.simple_expression);
    } else
        genMixalTerm(node);
}

// Generate MIXAL code for a term
void genMixalTerm(ASTNode *node) {
    if (node == NULL) return;
    if (node->type == NODE_TERM){
            genMixalTerm(node->data.term.term);
            genMixalFactor(node->data.term.factor);
            if (node->data.term.operation == '*') {
                printf("MUL %s\n", node->data.term.factor);
            } else if (node->data.term.operation == '/') {
                printf("DIV %s\n", node->data.term.factor);
            }
    } else
            genMixalFactor(node);
}

// Generate MIXAL code for a factor
void genMixalFactor(ASTNode *node) {
    if (node == NULL) return;
    if (node->type == NODE_FACTOR){
            if (node->data.factor.identifier != NULL) {
                printf("LDA %s\n", node->data.factor.identifier); 
            } else {
                printf("LDA =%d\n", node->data.factor.num);
            }
    } else
        fprintf(stderr, "Unknown factor type in MIXAL generator.\n");
}
