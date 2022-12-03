/* this file represents a list of external labels */
#ifndef EXTERN_LIST_H
#define EXTERN_LIST_H

#include "globals.h"

/* this struct represents an element in the list */
typedef struct extern_element{
    char *name; /* the name of the label */
    long place; /* the place of the label */
    struct extern_element *next; /* a pointer to the next element in the list*/
} extern_element;

typedef struct extern_table{
    struct extern_element *head;
    struct extern_element *tail;
} extern_table;

/* this function creates an empty list of externals (contains a garbage value) */
extern_table *create_extern_table(void);

/* this function adds a new element to a given list of externals via given details */
void add_extern_element(extern_table *table, char *name, long place);

/* this function takes a list of externals and frees its memory */
void free_extern_table(extern_table *table);

bool extern_is_empty(extern_table *table);

#endif
