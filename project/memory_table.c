/* this file represents a memory table */
#include <stdlib.h>
#include <stdio.h>
#include "memory_table.h"

static void add_single_element(memory_table *table, memory_element *new_element);

/* creates an empty memory table (returns a dummy element) */
memory_table *create_memory_table() {
    memory_table *table = (memory_table *) calloc(1, sizeof(memory_table));

    if (table == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }
    return table;
}

/* adds a single cell to the table */
static void add_single_element(memory_table *table, memory_element *new_element) {
    if(table->head == NULL) {
        table->head = (memory_element *) calloc(1, sizeof(memory_element)); 
        table->tail = table->head;
        table->tail->cell = new_element->cell;
        return;
    }
    
    table->tail->next = new_element;
    table->tail = table->tail->next;
}

/* adds a new cell to the given table via the given details */
void add_memory_elements(memory_table *table, int cell_amount, long value) {
    memory_element *new_node; /* the new cell to be added to the memory table */
    int i;	

    /* adds a new cells to the table according to the given cell amount */
    for (i = 0; i < cell_amount; i++) {
        new_node = (memory_element *) calloc(1, sizeof(memory_element));

        if (new_node == NULL) {
            fprintf(stderr, "c language error: malloc failed");
            exit(1);
        }

        new_node->cell |= value; /* sets the current cell */
        value >>= 8; /* procceds to the next cell by shifting 8 bits */        
	    
        add_single_element(table, new_node); /* adds the cell to the table */
    }
}

/* frees the table */
void free_memory_table(memory_table *table) {
    memory_element *current;

    while (table->head != NULL) {
        current = table->head;
        table->head = table->head->next;
        free(current);
    }
    free(table);
}

/* checks whether the given table is empty */
bool mem_is_empty(memory_table *table){
    return (table->head == NULL);
}
