#ifndef AST_H
#define AST_H

/* define the structure for AST nodes */
typedef struct ASTNode {
    enum {
        NODE_PROGRAM, NODE_STATEMENT_SEQUENCE, NODE_STATEMENT, NODE_ASSIGN, NODE_IF,
        NODE_REPEAT, NODE_READ, NODE_WRITE, NODE_FACTOR, NODE_TERM, NODE_SIMPLE_EXPRESSION, NODE_RELATIONAL_EXPRESSION, NODE_EXPRESSION
    } type;

    union {
        struct { struct ASTNode *statement_sequence; } program;
        struct { struct ASTNode *statement_sequence; struct ASTNode *statement; } statement_sequence;
        struct { char *identifier; struct ASTNode *expression; } assign_statement;
        struct { struct ASTNode *expression; struct ASTNode *statement_sequence1; struct ASTNode *statement_sequence2; } if_statement;
        struct { struct ASTNode *statement_sequence; struct ASTNode *expression; } repeat_statement;
        struct { char *identifier; } read_statement;
        struct { char *identifier; } write_statement;
        struct { struct ASTNode *relational_expression; } expression;
        struct { struct ASTNode *relational_expression; int operation; struct ASTNode *simple_expression; } relational_expression;
        struct { struct ASTNode *simple_expression; int operation; struct ASTNode *term; } simple_expression;
        struct { struct ASTNode *term; int operation; struct ASTNode *factor; } term;
        struct { struct ASTNode *expression; char *identifier; int num; } factor;
    } data;
} ASTNode;

void astPrint(ASTNode *node, int indent);
void AST_print(ASTNode *node, int indent);
const char* getNodeTypeString(int type);

#endif
