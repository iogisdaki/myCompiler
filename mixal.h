#ifndef MIXAL_H
#define MIXAL_H

#include "ast.h"

void generateMixal(ASTNode *node);
void genMixal(ASTNode *node);
void generateMixalStatement(ASTNode *node);
void generateMixalExpression(ASTNode *node);
void generateMixalTerm(ASTNode *node);
void generateMixalFactor(ASTNode *node);


#endif