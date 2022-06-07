/* this file represents a list of external labels */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extern_list.h"

/* this function creates an empty list of externals (contains a garbage value) */
extern_node *create_extern_list(){
    extern_node *list = (extern_node *) calloc(1, sizeof(extern_node)); /* the new list to return */
	/* checking if the calloc function worked */
    if (list == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    list->name = (char *) calloc(1, sizeof(char *));
	/* checking if the calloc function worked */
    if (list->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

	/* setting the place */
    list->place = 0;

    return list;
}

/* this function adds a new element to a given list of externals via given details */
void add_extern_item(extern_node *list, char *name, long place) {
    extern_node *new_node /* the new element to add to the given list */, *current = list  /* a pointer to the given list */;
    char *name_temp; /* a temporary string for assigning the given name to the new element */

    new_node = (extern_node *) calloc(1, sizeof(extern_node));
	/* checking if the calloc function worked */
    if (new_node == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    new_node->name = (char *) calloc(1, sizeof(char *));
	/* checking if the calloc function worked */
    if (new_node->name == NULL) {
        fprintf(stderr, "c language error: calloc failed");
        exit(1);
    }

    name_temp = (char *) malloc(strlen(name) + 1);
	/* checking if the malloc function worked */
    if (name_temp == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }
    strcpy(name_temp, name);

	/* these 2 lines bellow are responsible for creating the new element */
    new_node->name = name_temp;
    new_node->place = place;

	/* reaching the end of the list */
    while (current->next != NULL) {
        current = current->next;
    }

	/* adding the new element to the list */
    current->next = new_node;
}

/* this function takes a list of externals and frees its memory */
void free_extern_list(extern_node *list) {
    extern_node *current; /* a pointer to the given list */

	/* iterating through the list and freeing its elements */
    while (list != NULL) {
        current = list;
        list = list->next; /* moving to the next element */

		/* freeing the variables */
        free(current->name);
        free(current);
    }
}