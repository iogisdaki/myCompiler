#include <stdio.h>
#include "mixal.h"

FILE *file1;

void generateMixal(Node *node){
    file1 = fopen("result.txt", "w");

    if (file1 == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    printf("ORIG 1000\nSTART\n"); // start the memory location at 1000
    genMixal(node);
    printf("END  START");
    fclose(file1);
}

void genMixal(Node *node) {
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
            generateExpression(node->data.assign_statement.expression);
            printf("\tSTA %s\n", node->data.assign_statement.identifier);  // store the result from accumulator to the identifier
            break;
        case NODE_IF:
            printf("\texpression\n"); //generateExpression(node->data.if_statement.expression);
            printf("\tCMPA 0\n");  // compare result to 0(false)
            if (node->data.if_statement.statement_sequence2 != NULL) {
                printf("\tJE ELSE\n");  // if equal to 0(false) jump to else
                genMixal(node->data.if_statement.statement_sequence1); // if true do the statement sequence of the if block
                printf("\tJMP END_IF\n");  // and skip else block
                printf("ELSE\n"); // do the else block code
                genMixal(node->data.if_statement.statement_sequence2);
                printf("END_IF\n");
            } else{
                printf("\tJE END_IF\n");  // if equal to 0(false) jump to end
                genMixal(node->data.if_statement.statement_sequence1); // if true do the statement sequence of the if block
                printf("END_IF\n");
            }
            break;
        case NODE_REPEAT:
            printf("REPEAT\n");
            genMixal(node->data.repeat_statement.statement_sequence);
            printf("\texpression\n");//generateExpression(node->data.repeat_statement.expression);
            printf("\tCMPA 0\n");  // compare result to 0(false)
            printf("\tJNE REPEAT\n");  // if until condition is false loop
            break;
        case NODE_READ:
            printf("\tIN 16\n"); // read from device 16 (terminal)
            printf("\tSTA %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            printf("\tLDA %s\n", node->data.read_statement.identifier);            
            printf("\tOUT 16\n");// write to device 16 (terminal)
            break;
        default:
            fprintf(stderr, "Unknown node type in MIXAL generator.\n");
    }
}


    void generateExpression(Node *node){
        if (node == NULL) return;
        switch (node->type) {
            case NODE_FACTOR:
                generateFactor(node);
                break;
            case NODE_TERM:
                generateTerm(node);
                break;
            case NODE_SIMPLE_EXPRESSION:
                generateSimpleExpression(node);
                break;
            case NODE_RELATIONAL_EXPRESSION:
                generateRelationalExpression(node);
                break;
        }
    }

    void generateFactor(Node *node){
        if (node == NULL) return;
        generateExpression(node->data.factor.expression);
        if (node->data.factor.identifier != NULL) {
            printf("%s\n", node->data.factor.identifier); 
        } else {
            printf("=%d=\n", node->data.factor.num); // constant
        }
    }

    void generateTerm(Node *node){
        if (node == NULL) return;
        generateExpression(node->data.term.term);
        if (node->data.term.operation == '*') 
            printf("\tMUL ");
        else if (node->data.term.operation == '/') 
            printf("\tDIV ");
        generateExpression(node->data.term.factor);
    }

    void generateSimpleExpression(Node *node){
        if (node == NULL) return;
        generateExpression(node->data.simple_expression.simple_expression);
        printf("\tSTA TEMP1\n");
        generateExpression(node->data.simple_expression.term);
        printf("\tLDA TEMP1\n");
        if (node->data.simple_expression.operation == '+') 
            printf("\tADD ");
        else if (node->data.simple_expression.operation == '-') 
            printf("\tSUB ");
        printf("TEMP2\n");
    }

    void generateRelationalExpression(Node *node){
        if (node == NULL) return;
        generateExpression(node->data.relational_expression.relational_expression);
        if (node->data.relational_expression.operation == '<' || node->data.relational_expression.operation == '=') 
            printf("\tCMPA ");
        generateExpression(node->data.relational_expression.simple_expression);
    }