#ifndef MIXAL_H
#define MIXAL_H

#include "parseTree.h"

void generateMixal(Node *node);
void genMixal(Node *node);
void generateExpression(Node *node);
void generateFactor(Node *node);
void generateTerm(Node *node);
void generateSimpleExpression(Node *node);
void generateRelationalExpression(Node *node);


#endif