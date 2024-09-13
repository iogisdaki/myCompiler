%{
#include <iostream>
%}

%option c++
%option noyywrap 
/* we deal with EOF later */

%%
  /* keywords */
"if"        { std::cout << "IF" << std::endl; }
"then"      { std::cout << "THEN" << std::endl; }
"else"      { std::cout << "ELSE" << std::endl; }
"end"       { std::cout << "END" << std::endl; }
"repeat"    { std::cout << "REPEAT" << std::endl; }
"until"     { std::cout << "UNTIL" << std::endl; }
"read"      { std::cout << "READ" << std::endl; }
"write"     { std::cout << "WRITE" << std::endl; }

  /* identifiers and numbers*/
[a-zA-Z_][a-zA-Z0-9_]*  { std::cout << "IDENTIFIER(" << yytext << ")" << std::endl; }
[0-9]+      { std::cout  << "NUMBER(" << yytext << ")" << std::endl; }

  /* operators*/
"*"         { std::cout << "MULTIPLY" << std::endl; }
"/"         { std::cout << "DIVIDE" << std::endl; }
"+"         { std::cout << "PLUS" << std::endl; }
"-"         { std::cout << "MINUS" << std::endl; }

":="        { std::cout << "ASSIGN" << std::endl; }

"("         { std::cout << "LEFT_PARENTHESIS" << std::endl; }
")"         { std::cout << "RIGHT_PARENTHESIS" << std::endl; }

";"         { std::cout << "SEMICOLON" << std::endl; }

"<"         { std::cout << "LESS" << std::endl; }
"="         { std::cout << "EQUALS" << std::endl; }

<<EOF>>     { std::cout << "END_OF_FILE" << std::endl; return 0; }

[ \t\n]+    ;  // ignore whitespace
"//".*      ;  // ignore single-line comments
"/*"([^*]|\*[^/])*"*/" ;  // Ignore multi-line comments
.           { std::cout << "Unknown character: " << yytext << std::endl; }
%%