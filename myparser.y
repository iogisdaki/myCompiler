%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


/* define the structure for AST nodes */
typedef struct ASTNode {
    enum {
        NODE_PROGRAM, NODE_STATEMENT_SEQUENCE, NODE_STATEMENT, NODE_ASSIGN, NODE_IF,
        NODE_REPEAT, NODE_READ, NODE_WRITE, NODE_FACTOR, NODE_TERM, NODE_RELATIONAL_OPERATION, NODE_EXPRESSION
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
        struct { struct ASTNode *simple_expression; int operation; struct ASTNode *term; } relative_operation;
        struct { struct ASTNode *factor; int operation; struct ASTNode *term; } term;
        struct { struct ASTNode *expression; char *identifier; int num; } factor;
    } data;
} ASTNode;

ASTNode *createNode(int type) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

const char* getNodeTypeString(int type) {
    switch(type) {
        case NODE_PROGRAM: return "Program";
        case NODE_STATEMENT_SEQUENCE: return "Statement Sequence";
        case NODE_STATEMENT: return "Statement";
        case NODE_ASSIGN: return "Assign Statement";
        case NODE_IF: return "If Statement";
        case NODE_REPEAT: return "Repeat Statement";
        case NODE_READ: return "Read Statement";
        case NODE_WRITE: return "Write Statement";
        case NODE_FACTOR: return "Factor";
        case NODE_TERM: return "Term";
        case NODE_RELATIONAL_OPERATION: return "Relational Operation";
        case NODE_EXPRESSION: return "Expression";
        default: return "Unknown";
    }
}

/* Recursive function to print the AST */
void AST_print(ASTNode *node, int indent) {
    if (node == NULL) return;

    for (int i = 0; i < indent; i++) {
        printf("  ");  // Indentation for better readability
    }
    printf("%s\n", getNodeTypeString(node->type));

    /* Recursively print children based on the node type */
    switch (node->type) {
        case NODE_PROGRAM:
            AST_print(node->data.program.statement_sequence, indent + 1);
            break;
        case NODE_STATEMENT_SEQUENCE:
            AST_print(node->data.statement_sequence.statement_sequence, indent + 1);
            AST_print(node->data.statement_sequence.statement, indent + 1);
            break;
        case NODE_ASSIGN:
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Identifier: %s\n", node->data.assign_statement.identifier);
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
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Identifier: %s\n", node->data.read_statement.identifier);
            break;
        case NODE_WRITE:
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Identifier: %s\n", node->data.write_statement.identifier);
            break;
        case NODE_FACTOR:
            if (node->data.factor.identifier) {
                for (int i = 0; i < indent + 1; i++) printf("  ");
                printf("Identifier: %s\n", node->data.factor.identifier);
            } else {
                for (int i = 0; i < indent + 1; i++) printf("  ");
                printf("Number: %d\n", node->data.factor.num);
            }
            break;
        case NODE_TERM:
            AST_print(node->data.term.term, indent + 1);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Operation: %c\n", node->data.term.operation);
            AST_print(node->data.term.factor, indent + 1);
            break;
        case NODE_RELATIONAL_OPERATION:
            AST_print(node->data.relative_operation.term, indent + 1);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Operation: %c\n", node->data.relative_operation.operation);
            AST_print(node->data.relative_operation.simple_expression, indent + 1);
            break;
        case NODE_EXPRESSION:
            AST_print(node->data.expression.relational_expression, indent + 1);
            break;
        default:
            printf("Unknown node type\n");
            break;
    }
}
%}

/* Bison union for token values */
%union {
    int num;
    char* string;
    struct ASTNode* node;
}

/* Token declarations */
%token <string> IDENTIFIER
%token <num> NUMBER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE MULTIPLY DIVIDE PLUS MINUS ASSIGN LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON LESS EQUALS UNKNOWN
%type <node> program statement_sequence statement assign_statement if_statement repeat_statement read_statement write_statement factor term simple_expression relational_expression expression

%start program

%%

/* Grammar rules */

program:
    statement_sequence {
        $$ = createNode(NODE_PROGRAM);
        $$->data.program.statement_sequence = $1;
        AST_print($$, 0); 
    };

statement_sequence:
    statement_sequence SEMICOLON statement {
        $$ = createNode(NODE_STATEMENT_SEQUENCE);
        $$->data.statement_sequence.statement_sequence = $1;
        $$->data.statement_sequence.statement = $3;
    }
    | statement { $$ = $1; };

statement:
    assign_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | repeat_statement { $$ = $1; }
    | read_statement { $$ = $1; }
    | write_statement { $$ = $1; };

assign_statement:
    IDENTIFIER ASSIGN expression {
        $$ = createNode(NODE_ASSIGN);
        $$->data.assign_statement.identifier = $1;  // $1 is a string (IDENTIFIER)
        $$->data.assign_statement.expression = $3;
    };

if_statement:
    IF expression THEN statement_sequence END {
        $$ = createNode(NODE_IF);
        $$->data.if_statement.expression = $2;
        $$->data.if_statement.statement_sequence1 = $4;
        $$->data.if_statement.statement_sequence2 = NULL;
    }
    | IF expression THEN statement_sequence ELSE statement_sequence END {
        $$ = createNode(NODE_IF);
        $$->data.if_statement.expression = $2;
        $$->data.if_statement.statement_sequence1 = $4;
        $$->data.if_statement.statement_sequence2 = $6;
    };

repeat_statement:
    REPEAT statement_sequence UNTIL expression {
        $$ = createNode(NODE_REPEAT);
        $$->data.repeat_statement.statement_sequence = $2;
        $$->data.repeat_statement.expression = $4;
    };

read_statement:
    READ IDENTIFIER {
        $$ = createNode(NODE_READ);
        $$->data.read_statement.identifier = $2;  // $2 is a string (IDENTIFIER)
    };

write_statement:
    WRITE IDENTIFIER {
        $$ = createNode(NODE_WRITE);
        $$->data.write_statement.identifier = $2;  // $2 is a string (IDENTIFIER)
    };

factor:
    LEFT_PARENTHESIS expression RIGHT_PARENTHESIS { $$ = $2; }
    | NUMBER {
        $$ = createNode(NODE_FACTOR);
        $$->data.factor.num = $1;  // $1 is a number (NUMBER)
    }
    | IDENTIFIER {
        $$ = createNode(NODE_FACTOR);
        $$->data.factor.identifier = $1;  // $1 is a string (IDENTIFIER)
    };

term:
    factor { $$ = $1; }
    | term MULTIPLY factor {
        $$ = createNode(NODE_TERM);
        $$->data.term.operation = '*';
        $$->data.term.term = $1;
        $$->data.term.factor = $3;
    }
    | term DIVIDE factor {
        $$ = createNode(NODE_TERM);
        $$->data.term.operation = '/';
        $$->data.term.term = $1;
        $$->data.term.factor = $3;
    };

simple_expression:
    term { $$ = $1; }
    | simple_expression PLUS term {
        $$ = createNode(NODE_TERM);
        $$->data.term.operation = '+';
        $$->data.term.term = $1;
        $$->data.term.factor = $3;
    }
    | simple_expression MINUS term {
        $$ = createNode(NODE_TERM);
        $$->data.term.operation = '-';
        $$->data.term.term = $1;
        $$->data.term.factor = $3;
    };

relational_expression:
    simple_expression { $$ = $1; }
    | relational_expression LESS simple_expression {
        $$ = createNode(NODE_RELATIONAL_OPERATION);
        $$->data.relative_operation.operation = '<';
        $$->data.relative_operation.simple_expression = $3;
    }
    | relational_expression EQUALS simple_expression {
        $$ = createNode(NODE_RELATIONAL_OPERATION);
        $$->data.relative_operation.operation = '=';
        $$->data.relative_operation.simple_expression = $3;
    };

expression:
    relational_expression { $$ = $1; };

%%

int main() {
    yyparse();  
    return 0;
}