#ifndef MIXAL_H
#define MIXAL_H

#include "ast.h"

void generateMixal(ASTNode *node);
void genMixal(ASTNode *node);
void generateExpression(ASTNode *node);
void generateTerm(ASTNode *node);
void generateFactor(ASTNode *node);


#endif