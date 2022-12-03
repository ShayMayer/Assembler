#include <stdlib.h>
#include <stdio.h>
#include "memory_table.h"

static void add_single_element(memory_table *table, memory_element *new_element);

/* this function creates an empty memory table (contains a garbage value) */
memory_table *create_memory_table() {
    memory_table *table = (memory_table *) calloc(1, sizeof(memory_table)); /* the new memory table */

    /* checking if the malloc function worked */
    if (table == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    return table;
}

static void add_single_element(memory_table *table, memory_element *new_element) {
	if(table->head == NULL) {
		table->head = (memory_element *) calloc(1, sizeof(memory_element)); 
		table->tail = table->head;
		table->tail->cell = new_element->cell;
		/*free(new_element);*/
		return;
	}

	table->tail->next = new_element;
	table->tail = table->tail->next;
}

/* this function adds a new cell to the given memory table via given details */
void add_memory_elements(memory_table *table, int cell_amount, long value) {
    memory_element *new_node; /* the new cell to be added to the memory table */
    int i;	

    /* adding new cell amount the the given memory table */
    for (i = 0; i < cell_amount; i++) {
        new_node = (memory_element *) calloc(1, sizeof(memory_element));

		/* checking if the malloc function worked */
        if (new_node == NULL) {
	        fprintf(stderr, "c language error: malloc failed");
            exit(1);
        }

        new_node->cell |= value; /* setting the current cell */
        value >>= 8; /* shifting 8 bits in order to set the next cell of the memory table */        

		add_single_element(table, new_node);
    }
}

/* this function takes a memory tables and frees its memory */
void free_memory_table(memory_table *table) {
    memory_element *current; /* a pointer to the given table */

    /* iterating through the table and freeing its elements */
    while (table->head != NULL) {
        current = table->head; /* makes the variable points to the given memory table */
        table->head = table->head->next; /* moving to the next element */
        free(current); /* freeing the current element */
    }
	free(table);
}


bool mem_is_empty(memory_table *table){
	return (table->head == NULL);
}
