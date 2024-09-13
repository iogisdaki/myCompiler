%{
#include <stdio.h>
#include <stdlib.h>

/* Forward declarations */
void yyerror(const char *s);
int yylex(void);

/* Implement yyerror */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

%}

%token IF THEN ELSE END REPEAT UNTIL READ WRITE IDENTIFIER NUMBER MULTIPLY DIVIDE PLUS MINUS ASSIGN LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON LESS EQUALS END_OF_FILE UNKNOWN

%%

/* rules */
program: statement_sequence;

statement_sequence: 
    statement_sequence SEMICOLON statement
    | statement;

statement:
    assign_statement
    | if_statement
    | repeat_statement
    | read_statement
    | write_statement;

assign_statement: 
    IDENTIFIER ASSIGN expression;

read_statement:
    READ IDENTIFIER;

write_statement:
    WRITE IDENTIFIER;

if_statement:
    IF expression THEN statement_sequence END
    | IF expression THEN statement_sequence ELSE statement_sequence END;

repeat_statement:
    REPEAT statement_sequence UNTIL expression;

factor:
    LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
    | NUMBER
    | IDENTIFIER;
    
term:
    factor
    | term MULTIPLY factor
    | term DIVIDE factor;

simple_expression:
    term
    | simple_expression PLUS term
    | simple_expression MINUS term;

relational_expression:
    simple_expression
    | relational_expression LESS simple_expression
    | relational_expression EQUALS simple_expression;

expression:
    relational_expression;

%%

// Main function
int main() {
    return yyparse();
}