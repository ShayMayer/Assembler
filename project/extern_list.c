/* this file represents a list of external labels */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extern_list.h"

/* this function creates an empty list of externals (contains a garbage value) */
extern_table *create_extern_table(){
    extern_table *table = (extern_table *) calloc(1, sizeof(extern_table)); /* the new list to return */
    /* checking if the calloc function worked */
    if (table == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }
    return table;
}

/* this function adds a new element to a given list of externals via given details */
void add_extern_element(extern_table *table, char *name, long place) {
    extern_element *new_element; /* the new element to add to the given list */
    char *name_temp = NULL; /* a temporary string for assigning the given name to the new element */   

    name_temp = (char *) malloc(strlen(name) + 1);
    /* checking if the malloc function worked */
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
    /* checking if the calloc function worked */
    if (new_element == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    new_element->name = (char *) calloc(1, sizeof(char *));
    /* checking if the calloc function worked */
    if (new_element->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    /* these 2 lines bellow are responsible for creating the new element */
    new_element->name = name_temp;
    new_element->place = place;

    table->tail->next = new_element;
    table->tail = table->tail->next;
}

/* this function takes a list of externals and frees its memory */
void free_extern_table(extern_table *table) {
    extern_element *current; /* a pointer to the given list */

    /* iterating through the list and freeing its elements */
    while (table->head != NULL) {
        current = table->head;
        table->head = table->head->next; /* moving to the next element */

	/* freeing the variables */
        free(current->name);
        free(current);
    }
    free(table);
}

bool extern_is_empty (extern_table *table) {
    return (table->head == NULL);
}




