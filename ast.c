#include <stdio.h>
#include "ast.h"

FILE *file;

void astPrint(ASTNode *node, int indent){
    file = fopen("astTree.txt", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    AST_print(node, indent);
    fclose(file);
}

/* recursivelly print ast*/
void AST_print(ASTNode *node, int indent) {
    if (node == NULL) return;

    for (int i = 0; i < indent; i++) {
        fprintf(file, "  ");  // indentation for the tree
    }
    fprintf(file, "%s\n", getNodeTypeString(node->type));

    switch (node->type) {
        case NODE_PROGRAM:
            AST_print(node->data.program.statement_sequence, indent + 1);
            break;
        case NODE_STATEMENT_SEQUENCE:
            AST_print(node->data.statement_sequence.statement_sequence, indent + 1);
            AST_print(node->data.statement_sequence.statement, indent + 1);
            break;
        case NODE_ASSIGN:
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Identifier: %s\n", node->data.assign_statement.identifier);
            AST_print(node->data.assign_statement.expression, indent + 1);
            break;
        case NODE_IF:
            AST_print(node->data.if_statement.expression, indent + 1);
            AST_print(node->data.if_statement.statement_sequence1, indent + 1);
            if (node->data.if_statement.statement_sequence2 != NULL)
                AST_print(node->data.if_statement.statement_sequence2, indent + 1);
            break;
        case NODE_REPEAT:
            AST_print(node->data.repeat_statement.statement_sequence, indent + 1);
            AST_print(node->data.repeat_statement.expression, indent + 1);
            break;
        case NODE_READ:
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Identifier: %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Identifier: %s\n", node->data.write_statement.identifier);
            break;
        case NODE_FACTOR:
            AST_print(node->data.factor.expression, indent + 1);
            if (node->data.factor.identifier) {
                for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
                fprintf(file, "Identifier: %s\n", node->data.factor.identifier);
            } else if(node->data.factor.num){
                for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
                fprintf(file, "Number: %d\n", node->data.factor.num);
            }
            break;
        case NODE_TERM:
            AST_print(node->data.term.term, indent + 1);
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Operation: %c\n", node->data.term.operation);
            AST_print(node->data.term.factor, indent + 1);
            break;
        case NODE_SIMPLE_EXPRESSION:
            AST_print(node->data.simple_expression.simple_expression, indent + 1);
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Operation: %c\n", node->data.simple_expression.operation);
            AST_print(node->data.simple_expression.term, indent + 1);
            break;
        case NODE_RELATIONAL_EXPRESSION:
            AST_print(node->data.relational_expression.relational_expression, indent + 1);
            for (int i = 0; i < indent + 1; i++) fprintf(file, "  ");
            fprintf(file, "Operation: %c\n", node->data.relational_expression.operation);
            AST_print(node->data.relational_expression.simple_expression, indent + 1);
            break;
        case NODE_EXPRESSION:
            AST_print(node->data.expression.relational_expression, indent + 1);
            break;
        default:
            fprintf(stderr, "Error: Unknown node type %d\n", node->type);
            break;
    }
}

const char* getNodeTypeString(int type) {
    switch(type) {
        case NODE_PROGRAM: return "Program";
        case NODE_STATEMENT_SEQUENCE: return "Statement Sequence";
        case NODE_ASSIGN: return "Assign Statement";
        case NODE_IF: return "If Statement";
        case NODE_REPEAT: return "Repeat Statement";
        case NODE_READ: return "Read Statement";
        case NODE_WRITE: return "Write Statement";
        case NODE_FACTOR: return "Factor";
        case NODE_TERM: return "Term";
        case NODE_SIMPLE_EXPRESSION: return "Simple Expression";
        case NODE_RELATIONAL_EXPRESSION: return "Relational Expression";
        case NODE_EXPRESSION: return "Expression";
        default: return "Unknown";
    }
}
