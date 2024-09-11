%{
#include <iostream>
%}

%option c++
%option noyywrap

%%
  /*rules*/
"if"        { std::cout << "IF" << std::endl; }
"else"      { std::cout << "ELSE" << std::endl; }
"while"     { std::cout << "WHILE" << std::endl; }
"return"    { std::cout << "RETURN" << std::endl; }
"and"       { std::cout << "AND" << std::endl; }
"not"       { std::cout << "NOT" << std::endl; }
"or"        { std::cout << "OR" << std::endl; }
"int"       { std::cout << "INT" << std::endl; }
"print"     { std::cout << "PRINT" << std::endl; }
"read"      { std::cout << "READ" << std::endl; }

[a-zA-Z_][a-zA-Z0-9_]*  { std::cout << "IDENTIFIER(" << yytext << ")" << std::endl; }
[0-9]+      { std::cout  << "NUMBER(" << yytext << ")" << std::endl; }

"+"         { std::cout << "PLUS" << std::endl; }
"-"         { std::cout << "MINUS" << std::endl; }
"*"         { std::cout << "MULTIPLY" << std::endl; }
"/"         { std::cout << "DIVIDE" << std::endl; }

"("         { std::cout << "LPAREN" << std::endl; }
")"         { std::cout << "RPAREN" << std::endl; }
"{"         { std::cout << "LBRACE" << std::endl; }
"}"         { std::cout << "RBRACE" << std::endl; }
"["         { std::cout << "LBRACKET" << std::endl; }
"]"         { std::cout << "RBRACKET" << std::endl; }

";"         { std::cout << "SEMICOLON" << std::endl; }
","         { std::cout << "COMMA" << std::endl; }

"="         { std::cout << "ASSIGN" << std::endl; }
"!="        { std::cout << "NOT_EQUALS" << std::endl; }
">"         { std::cout << "MORE" << std::endl; }
">="        { std::cout << "MORE_EQUALS" << std::endl; }
"<"         { std::cout << "LESS" << std::endl; }
"<="        { std::cout << "LESS_EQUALS" << std::endl; }
"=="        { std::cout << "EQUALS" << std::endl; }

<<EOF>>     { std::cout << "END_OF_FILE" << std::endl; return 0; }

[ \t\n]+    ;  // Ignore whitespace
"//".*      ;  // Ignore single-line comments
"/*"([^*]|\*[^/])*"*/" ;  // Ignore multi-line comments
.           { std::cout << "Unknown character: " << yytext << std::endl; }
%%
