/* this file represents a symbol table */
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "globals.h"

/* enum containing all types of symbols */
typedef enum symbol_types {
    CODE_SYMBOL = 1, /* code symbol */
    DATA_SYMBOL = 2, /* data symbol */
    EXTERNAL_SYMBOL = 4, /* extern symbol */ 
    ENTRY_SYMBOL = 8, /* entry symbol */
    ANY_SYMBOL = -1 /* the symbol doesn't matter */
} symbol_types;

/* represents a symbol table element */
typedef struct symbol_element {
    char *name; /* the name of the symbol */
    long address; /* the address of the symbol */
    unsigned int attributes: 4; /* the attributes of the symbol */
    struct symbol_element *next; /* a pointer to the next element in the symbol table */
} symbol_element;

/* represents a symbol table */
typedef struct symbol_table {
    struct symbol_element *head; /* symbol table's first element */
    struct symbol_element *tail; /* symbol table's last element */
} symbol_table;

/* creates an empty symbol table (returns a dummy element) */
symbol_table *create_symbol_table(void);

/* adds a new element to the given table via the given details */
void add_symbol_item(symbol_table *table, char *name, long address, unsigned int attributes);

/* says whether the given table has an element with the given name and atrributes */
bool exists(symbol_table *table, char *name, unsigned int attributes);

/* adds attributes to the element which its name is the same as the given name */
void add_attributes(symbol_table *table, char *name, unsigned int attributes);

/* changes the address of the element which its name is the same as the given */
void change_address(symbol_table *table, char *name, long address);

/* returns a new symbol table containing all the elements that share the same attributes given as a parameter */
symbol_table* table_of_attributes(symbol_table *table, unsigned int attributes);

/* returns the address that belongs to the given name */
long get_address_of(symbol_table *table, char *name);

/* frees a symbol table */
void free_symbol_table(symbol_table *table);

/* says whether the given symbol table is empty */
bool sym_is_empty(symbol_table *table);

#endif
