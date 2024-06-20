/* this file represents a memory table */
#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include "globals.h"

/* represents memory cell */
typedef struct memory_element {
    unsigned int cell: 8; /* the value of the current cell */
    struct memory_element *next; /* a pointer to the next cell */
} memory_element;

/* represents a table of memory cells */
typedef struct memory_table {    
    struct memory_element *head; /* first element */
    struct memory_element *tail; /* last element */
} memory_table;

/* creates an empty memory table (returns a dummy element) */
memory_table *create_memory_table(void);

/* adds a new cell to the given table via the given details */
void add_memory_elements(memory_table *table, int cell_amount , long value);

/* frees the table */
void free_memory_table(memory_table *table);

/* checks whether the given table is empty */
bool mem_is_empty(memory_table *table);

#endif
