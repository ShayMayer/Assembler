/* this file represents a list of external labels */
#ifndef EXTERN_LIST_H
#define EXTERN_LIST_H

#include "globals.h"

/* represents an extern_table's element */
typedef struct extern_element{
    char *name; /* label's name */
    long place; /* label's place */
    struct extern_element *next; /* a pointer to the next element */
} extern_element;


/* represents an extern table */
typedef struct extern_table{
    struct extern_element *head; /* first element */
    struct extern_element *tail; /* last element */
} extern_table;

/* creates an empty list of externals (returns a dummy element) */
extern_table *create_extern_table(void);

/* adds a new element to the given table via the given details */
void add_extern_element(extern_table *table, char *name, long place);

/* frees the given table */
void free_extern_table(extern_table *table);

/* says whether the given table is empty */
bool extern_is_empty(extern_table *table);

#endif
