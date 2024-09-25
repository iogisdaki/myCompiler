#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 100  

struct Symbol {
    char *name;
};

struct SymbolTable {
    struct Symbol symbols[MAX_SYMBOLS];
    int size;
};

extern struct SymbolTable table;

void insertSymbol(char *name);
int lookupSymbol(char *name);
void printSymbolTable();

#endif
