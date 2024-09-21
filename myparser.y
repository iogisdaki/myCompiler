// TODO more detailed error messages

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"
#include "mixal.h"

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

Node *createNode(int type) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    return node;
}
%}


%union {
    int num;
    char* string;
    struct Node* node;
}

%token <string> IDENTIFIER
%token <num> NUMBER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE MULTIPLY DIVIDE PLUS MINUS ASSIGN LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON LESS EQUALS UNKNOWN
%type <node> program statement_sequence statement assign_statement if_statement repeat_statement read_statement write_statement factor term simple_expression relational_expression expression

%start program

%%

program:
    statement_sequence {
        $$ = createNode(NODE_PROGRAM);
        $$->data.program.statement_sequence = $1;
        parseTreePrint($$, 0); 
        generateMixal($$);
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
        $$->data.assign_statement.identifier = $1;  
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
        $$->data.read_statement.identifier = $2;  
    };

write_statement:
    WRITE IDENTIFIER {
        $$ = createNode(NODE_WRITE);
        $$->data.write_statement.identifier = $2;  
    };

factor:
    LEFT_PARENTHESIS expression RIGHT_PARENTHESIS { $$ = $2; }
    | NUMBER {
        $$ = createNode(NODE_FACTOR);
        $$->data.factor.num = $1;  
    }
    | IDENTIFIER {
        $$ = createNode(NODE_FACTOR);
        $$->data.factor.identifier = $1; 
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
        $$ = createNode(NODE_SIMPLE_EXPRESSION);
        $$->data.simple_expression.simple_expression = $1;
        $$->data.simple_expression.operation = '+';
        $$->data.simple_expression.term = $3;
    }
    | simple_expression MINUS term {
        $$ = createNode(NODE_SIMPLE_EXPRESSION);
        $$->data.simple_expression.simple_expression = $1;
        $$->data.simple_expression.operation = '-';
        $$->data.simple_expression.term = $3;
    };

relational_expression:
    simple_expression { $$ = $1; }
    | relational_expression LESS simple_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION);
        $$->data.relational_expression.relational_expression = $1;  
        $$->data.relational_expression.operation = '<';
        $$->data.relational_expression.simple_expression = $3; 
    }
    | relational_expression EQUALS simple_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION);
        $$->data.relational_expression.relational_expression = $1;  
        $$->data.relational_expression.operation = '=';
        $$->data.relational_expression.simple_expression = $3; 
    };

expression:
    relational_expression { $$ = $1; };

%%

int main() {
    yyparse();  
    return 0;
}