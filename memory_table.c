#include <stdlib.h>
#include <stdio.h>
#include "memory_table.h"

/* this function creates an empty memory table (contains a garbage value) */
memory_cell *create_memory_table() {
    memory_cell *table = (memory_cell *) calloc(1, sizeof(memory_cell)); /* the new memory table */

    /* checking if the malloc function worked */
    if (table == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    return table;
}

/* this function adds a new cell to the given memory table via given details */
void add_memory_cells(memory_cell *table, int cell_amount, long value) {
    memory_cell *new_node; /* the new cell to be added to the memory table */
    memory_cell *current = table; /* a pointer to the given table */
    int i;

    /* reaching the end of the list */
    while (current->next != NULL) {
        current = current->next;
    }

    /* adding new cell amount the the given memory table */
    for (i = 0; i < cell_amount; i++) {
        new_node = (memory_cell *) calloc(1, sizeof(memory_cell));

		/* checking if the malloc function worked */
        if (new_node == NULL) {
	        fprintf(stderr, "c language error: malloc failed");
            exit(1);
        }

        new_node->cell |= value; /* setting the current cell */
        value >>= 8; /* shifting 8 bits in order to set the next cell of the memory table */

        current->next = new_node; /* setting a new element in the memory table */
        current = current->next; /* moving to the next element */
    }
}

/* this function takes a memory tables and frees its memory */
void free_memory_table(memory_cell *table) {
    memory_cell *current; /* a pointer to the given table */

    /* iterating through the table and freeing its elements */
    while (table != NULL) {
        current = table; /* makes the variable points to the given memory table */
        table = table->next; /* moving to the next element */
        free(current); /* freeing the current element */
    }
}
