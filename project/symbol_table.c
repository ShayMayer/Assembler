/* this file represents a symbol table */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include <stdio.h>  

/* creates an empty symbol table (returns a dummy element) */
symbol_table *create_symbol_table() {
    symbol_table *table = (symbol_table *) calloc(1, sizeof(symbol_table));

    if (table == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }
    return table;
}

/* adds a new element to a given symbol table via the given details */
void add_symbol_item(symbol_table *table, char *name, long address, unsigned int attributes) {
    symbol_element *new_element; /* the new element to add to the given table */
    char *name_temp; /* a temporary string for assigning the given name to the new element */
    
    name_temp = (char *) malloc(strlen(name) + 1);
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
    if (new_element == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    new_element->name = (char *) calloc(1, sizeof(char));
    if (new_element->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    /* forms the new element */
    new_element->name = name_temp;
    new_element->address = address;
    new_element->attributes |= attributes;

	/* updates the state of the symbol table */
    table->tail->next = new_element;
    table->tail = table->tail->next;
}

/* says whether the given table has an element with the given name and atrributes */
bool exists(symbol_table *table, char *name, unsigned int attributes) {
    symbol_element *current = table->head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && (current->attributes & attributes))
            return TRUE;
        current = current->next;
    }

    return FALSE;
}

/* adds attributes to the element which its name is the same as the given name */
void add_attributes(symbol_table *table, char *name, unsigned int attributes) {
    symbol_element *current = table->head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->attributes |= attributes;
            return;
        }
        current = current->next;
    }
}

/* changes the address of the element which its name is the same as the given */
void change_address(symbol_table *table, char *name, long address) {
    symbol_element *current = table->head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->address = address;
            return;
        }
        current = current->next;
    }
}

/* returns a new symbol table containing all the elements that share the same attributes given as a parameter */
symbol_table* table_of_attributes(symbol_table *table, unsigned int attributes) {
    symbol_table *new_table = create_symbol_table(); /* the new table to return */ 
    symbol_element *current = table->head;
 
    while (current != NULL) {
       if(current->attributes & attributes)
           add_symbol_item(new_table, current->name, current->address, current->attributes);
        current = current->next;
    }

    return new_table;
}

/* returns the address that belongs to the given name */
long get_address_of(symbol_table *table, char *name){
    symbol_element *current = table->head;

    while (current != NULL) {
        if(strcmp(current->name, name) == 0)
            return current->address;
        current = current->next;
    }
    return -1;
}

/* frees a symbol table */
void free_symbol_table(symbol_table *table) {
    symbol_element *current = table->head;

    while (table->head != NULL) {
        current = table->head;
        table->head = table->head->next;

        free(current->name);
        free(current);
    }
    free(table);
}

/* says whether the given symbol table is empty */
bool sym_is_empty(symbol_table *table) {
    return (table->head == NULL);
}
