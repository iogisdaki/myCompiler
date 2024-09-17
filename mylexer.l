%{
#include "myparser.tab.h"
extern int yywrap(void) {}
%}


%%
  /* keywords */
"if"        { return IF; }
"then"      { return THEN; }
"else"      { return ELSE; }
"end"       { return END; }
"repeat"    { return REPEAT; }
"until"     { return UNTIL; }
"read"      { return READ; }
"write"     { return WRITE; }

  /* identifiers and numbers*/
[a-zA-Z_][a-zA-Z0-9_]*  {
    yylval.string = strdup(yytext); // store the identifier
    return IDENTIFIER;
}
[0-9]+      {
    yylval.num = atoi(yytext); // store the number
    return NUMBER;
}


  /* operators*/
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"+"         { return PLUS; }
"-"         { return MINUS; }

":="        { return ASSIGN; }

"("         { return LEFT_PARENTHESIS; }
")"         { return RIGHT_PARENTHESIS; }

";"         { return SEMICOLON; }

"<"         { return LESS; }
"="         { return EQUALS; }

[ \t\n]+    ;  // ignore whitespace
"//".*      ;  // ignore single-line comments
"/*"([^*]|\*[^/])*"*/" ;  // Ignore multi-line comments
.           { return UNKNOWN; }
%%