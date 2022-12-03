/* this file represents a memory table */
#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include "globals.h"

/* this struct represents a memory table cell */
typedef struct memory_element {
    unsigned int cell: 8; /* the value of the current cell */
    struct memory_element *next; /* a pointer to the next cell */
} memory_element;

typedef struct memory_table {    
    struct memory_element *head; /* a pointer to beginning of the table */
	struct memory_element *tail; /* a pointer to end of the table */
} memory_table;

/* this function creates an empty memory table (contains a garbage value) */
memory_table *create_memory_table(void);

/* this function adds a new cell to the given memory table via given details */
void add_memory_elements(memory_table *table, int cell_amount , long value);

/* this function takes a memory tables and frees its memory */
void free_memory_table(memory_table *table);

bool mem_is_empty(memory_table *table);

#endif
