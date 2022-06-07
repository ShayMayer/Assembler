/* this file represents a memory table */
#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

/* this struct represents a memory table cell */
typedef struct memory_cell {
    unsigned int cell: 8; /* the value of the current cell */
    struct memory_cell *next; /* a pointer to the next cell */
} memory_cell;

/* this function creates an empty memory table (contains a garbage value) */
memory_cell *create_memory_table(void);

/* this function adds a new cell to the given memory table via given details */
void add_memory_cells(memory_cell *table, int cell_amount , long value);

/* this function takes a memory tables and frees its memory */
void free_memory_table(memory_cell *table);

#endif