/* this file represents a list of external labels */
#ifndef EXTERN_LIST_H
#define EXTERN_LIST_H

/* this struct represents an element in the list */
typedef struct extern_node{
    char *name; /* the name of the label */
    long place; /* the place of the label */
    struct extern_node *next; /* a pointer to the next element in the list*/
} extern_node;

/* this function creates an empty list of externals (contains a garbage value) */
extern_node *create_extern_list(void);

/* this function adds a new element to a given list of externals via given details */
void add_extern_item(extern_node *list, char *name, long place);

/* this function takes a list of externals and frees its memory */
void free_extern_list(extern_node *list);

#endif