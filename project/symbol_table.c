/* this file represents a symbol table */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include <stdio.h>  

/* this function creates an empty symbol table (contains a garbage value) */
symbol_table *create_symbol_table() {
    symbol_table *table = (symbol_table *) calloc(1, sizeof(symbol_table)); /* the new table to return */

    /* checking if the calloc function worked */
    if (table == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    return table;
}

/* this function adds a new element to a given symbol table via given details */
void add_symbol_item(symbol_table *table, char *name, long address, unsigned int attributes) {
    symbol_element *new_element; /* the new element to add to the given table */
    char *name_temp; /* a temporary string for assigning the given name to the new element */
    
    /* allocating space in memory for the temporary name, used for assigning the name variable */
    name_temp = (char *) malloc(strlen(name) + 1);

    /* checking if the malloc function worked */
    if (name_temp == NULL) {
        printf("Error: there is not enough space");
        exit(1);
    }
    strcpy(name_temp, name);

	if(table->head == NULL) {
		table->head = (symbol_element *) calloc(1, sizeof(symbol_element));
		table->head->name = (char *) calloc(1, sizeof(char *));
	    table->head->name = name_temp;
		table->head->address = address;
		table->head->attributes |= attributes;

		table->tail = table->head;
		return;
	}
	new_element = (symbol_element *) calloc(1, sizeof(symbol_element));

    /* checking if the calloc function worked */
    if (new_element == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    /* allocating space in memory for the name */
    new_element->name = (char *) calloc(1, sizeof(char));

    /* checking if the calloc function worked */
    if (new_element->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    /* these 3 lines bellow are responsible for creating the new element */
    new_element->name = name_temp;
    new_element->address = address;
    new_element->attributes |= attributes;

    table->tail->next = new_element;
	table->tail = table->tail->next;
}

/* this function returns TRUE if the given symbol table contains a symbol table with the same name and attributes like the parameters amd FALSE other wise */
bool exists(symbol_table *table, char *name, unsigned int attributes) {
    symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the symbol table elements until we find the desired element */
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && (current->attributes & attributes)) /* means we found our element */
            return TRUE;
        current = current->next;
    }

    return FALSE;
}

/* this function adds attributes to an existing label via a given name */
void add_attributes(symbol_table *table, char *name, unsigned int attributes) {
    symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the symbol table elements until we find the desired element */
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->attributes |= attributes; /* adding the attributes the the desired element */
            return;
        }
        current = current->next;
    }
}

/* this function change the address of an existing label via a given address */
void change_address(symbol_table *table, char *name, long address) {
    symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the symbol table elements until we find the desired element */
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->address = address; /* changing the address of the desired element */
            return;
        }
        current = current->next;
    }
}

/* this function returns a new symbol table containing all the labels whcih conatin the given attributes */
symbol_table* table_of_attributes(symbol_table *table, unsigned int attributes) {
    symbol_table *new_table = create_symbol_table(); /* the new table to return */ 
	symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the table and adding the elements with the corresponding attributes to the new table */
    while (current != NULL) {
       if(current->attributes & attributes)
           add_symbol_item(new_table, current->name, current->address, current->attributes);
        current = current->next;
    }

    return new_table;
}

/* this function returns the address of the label that has the name of the given parameter */
long get_address_of(symbol_table *table, char *name){
    symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the symbol table elements until we find the desired element */
    while (current != NULL) {
        if(strcmp(current->name, name) == 0) /* found our element */
            return current->address; /* returning the address */
        current = current->next;
    }
    return -1;
}

/* this function takes a symbol tables and frees its memory */
void free_symbol_table(symbol_table *table) {
    symbol_element *current = table->head; /* a pointer to the given table */

    /* iterating through the table and freeing its elements */
    while (table->head != NULL) {
        current = table->head;
        table->head = table->head->next; /* moving to the next element */

		/* freeing the variables */
        free(current->name);
        free(current);
    }
	free(table);
}

bool sym_is_empty(symbol_table *table) {
	return (table->head == NULL);
}
