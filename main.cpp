#include <FlexLexer.h>
#include <iostream>

int main(int argc, char** argv){
  FlexLexer* l = new yyFlexLexer(std::cin, std::cerr);
  l->yylex();
  delete l;
  return 0;
}