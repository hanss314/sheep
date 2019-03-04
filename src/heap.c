#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "heap.h"
#include "expression.h"

#define HASHSIZE 1001
static struct Heap *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
Expr* lookup(char *s)
{
    struct Heap *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            return np->defn;
        }
    }
    return NULL;
}

struct Heap *lookup_h(char* s){
    struct Heap *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* heap_insert: put (name, defn) in hashtab */
struct Heap* heap_insert(char *name, Expr* defn)
{
    struct Heap *np;
    unsigned hashval;
    if ((np = lookup_h(name)) == NULL) { /* not found */
        np = (struct Heap *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else {
        free(np->defn);
    }
    np->defn = defn;
    return np;
}

void deleteHeap(){
    for(int i=0; i<HASHSIZE; i++){
        struct Heap* curr = hashtab[i];
        while (curr != NULL){
            free(curr->name);
            freeExpr(curr->defn);
            struct Heap* prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
}
