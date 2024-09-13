%{
#include "myparser.tab.h"
%}

%option noyywrap 
/* we deal with EOF later */

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
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
[0-9]+      { return NUMBER; }

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

<<EOF>>     { return END_OF_FILE; }

[ \t\n]+    ;  // ignore whitespace
"//".*      ;  // ignore single-line comments
"/*"([^*]|\*[^/])*"*/" ;  // Ignore multi-line comments
.           { return UNKNOWN; }
%%