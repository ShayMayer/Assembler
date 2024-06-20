/* this file represents a list of external labels */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extern_list.h"

/* creates an empty list of externals (returns a dummy element) */
extern_table *create_extern_table(){
    extern_table *table = (extern_table *) calloc(1, sizeof(extern_table));
    
    if (table == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }
    return table;
}

/* adds a new element to the given table via the given details */
void add_extern_element(extern_table *table, char *name, long place) {
    extern_element *new_element; /* the new element to add to the given list */
    char *name_temp = NULL; /* a temporary string for assigning the given name to the new element */   

    name_temp = (char *) malloc(strlen(name) + 1);
    if (name_temp == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }
    strcpy(name_temp, name);

    if(table->head == NULL) {
        table->head = (extern_element *) calloc(1, sizeof(extern_element));
        table->head->name = (char *) calloc(1, sizeof(char *));
        table->head->name = name_temp;
        table->head->place = place;

        table->tail = table->head;
        return;
    }

    new_element = (extern_element *) calloc(1, sizeof(extern_element));
    if (new_element == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    new_element->name = (char *) calloc(1, sizeof(char *));
    if (new_element->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    /* forms the new element */
    new_element->name = name_temp;
    new_element->place = place;

	/* updates the state of the symbol table */
    table->tail->next = new_element;
    table->tail = table->tail->next;
}

/* frees the given table */
void free_extern_table(extern_table *table) {
    extern_element *current;

    while (table->head != NULL) {
        current = table->head;
        table->head = table->head->next;

        free(current->name);
        free(current);
    }
    free(table);
}

/* says whether the given table is empty */
bool extern_is_empty (extern_table *table) {
    return (table->head == NULL);
}
