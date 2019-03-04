#include "expression.h"

#ifndef SHEEP_HEAP_H
#define SHEEP_HEAP_H

struct Heap { /* table entry: */
    struct Heap *next; /* next entry in chain */
    char *name; /* defined name */
    Expr *defn; /* associated expression */
};
Expr* lookup(char*);
struct Heap* heap_insert(char*, Expr*);
void deleteHeap();


#endif //SHEEP_HEAP_H
