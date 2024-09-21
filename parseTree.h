#ifndef PARSE_TREE_H
#define PARSE_TREE_H

/* define the structure for nodes */
typedef struct Node {
    enum {
        NODE_PROGRAM, NODE_STATEMENT_SEQUENCE, NODE_STATEMENT, NODE_ASSIGN, NODE_IF,
        NODE_REPEAT, NODE_READ, NODE_WRITE, NODE_FACTOR, NODE_TERM, NODE_SIMPLE_EXPRESSION, NODE_RELATIONAL_EXPRESSION, NODE_EXPRESSION
    } type;

    union {
        struct { struct Node *statement_sequence; } program;
        struct { struct Node *statement_sequence; struct Node *statement; } statement_sequence;
        struct { char *identifier; struct Node *expression; } assign_statement;
        struct { struct Node *expression; struct Node *statement_sequence1; struct Node *statement_sequence2; } if_statement;
        struct { struct Node *statement_sequence; struct Node *expression; } repeat_statement;
        struct { char *identifier; } read_statement;
        struct { char *identifier; } write_statement;
        struct { struct Node *relational_expression; } expression;
        struct { struct Node *relational_expression; int operation; struct Node *simple_expression; } relational_expression;
        struct { struct Node *simple_expression; int operation; struct Node *term; } simple_expression;
        struct { struct Node *term; int operation; struct Node *factor; } term;
        struct { struct Node *expression; char *identifier; int num; } factor;
    } data;
} Node;

void parseTreePrint(Node *node, int indent);
void treePrint(Node *node, int indent);
const char* getNodeTypeString(int type);

#endif
