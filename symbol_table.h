/* this file represents a symbol table */
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "globals.h"

/* this enum contains types of symbols (according to the task) */
typedef enum symbol_types {
    CODE_SYMBOL = 1, /* code symbol */
    DATA_SYMBOL = 2, /* data symbol */
    EXTERNAL_SYMBOL = 4, /* extern symbol */ 
    ENTRY_SYMBOL = 8, /* entry symbol */
    ANY_SYMBOL = -1 /* the symbol doesn't matter */
} symbol_types;

/* this struct represents a symbol table element */
typedef struct symbol_table{
    char *name; /* the name of the symbol */
    long address; /* the address of the symbol */
    unsigned int attributes: 4; /* the attributes of the symbol */
    struct symbol_table *next; /* a pointer to the next element in the symbol table */
} symbol_table;

/* this function creates an empty symbol table (contains a garbage value) */
symbol_table *create_symbol_table(void);

/* this function adds a new element to a given symbol table via given details */
void add_symbol_item(symbol_table *table, char *name, long address, unsigned int attributes);

/* this function returns TRUE if the given symbol table contains a symbol table with the same name and attributes like the parameters amd FALSE other wise */
bool exists(symbol_table *table, char *name, unsigned int attributes);

/* this function adds attributes to an existing label via a given name */
void add_attributes(symbol_table *table, char *name, unsigned int attributes);

/* this function change the address of an existing label via a given address */
void change_address(symbol_table *table, char *name, long address);

/* this function returns a new symbol table contains all the label that have the attributes given as a parameter */
symbol_table* table_of_attributes(symbol_table *table, unsigned int attributes);

/* this function returns the address of the label that has the name of the given parameter */
long get_address_of(symbol_table *table, char *name);

/* this function takes a symbol tables and frees its memory */
void free_symbol_table(symbol_table *table);

#endif