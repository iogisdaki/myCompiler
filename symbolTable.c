#include "symbolTable.h"

struct SymbolTable table = { .size = 0 };

void insertSymbol(char *name) {
    for (int i = 0; i < table.size; i++) {
        if (strcmp(table.symbols[i].name, name) == 0) 
            return;
    }
    table.symbols[table.size].name = strdup(name);
    table.size++;
}

int lookupSymbol(char *name) {
    for (int i = 0; i < table.size; i++) {
        if (strcmp(table.symbols[i].name, name) == 0) 
            return 1;
    }
    printf("Error: Variable '%s' not found.\n", name);
    return 0;
}

void printSymbolTable() {
    FILE *file2 = fopen("table.out", "w");
    if (file2 == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    for (int i = 0; i < table.size; i++) 
            fprintf(file2, "%s", table.symbols[i].name);   
    fclose(file2);
}
