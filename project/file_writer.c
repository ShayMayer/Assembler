/* the file contains functions that are respinsible for creating the output file of the assembler */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "memory_table.h"
#include "symbol_table.h"
#include "utils.h"
#include "extern_list.h"

/* this function creates an object file via the file_name parameter */
void write_object_file(char *file_name , memory_cell *ic_memory_table, memory_cell *dc_memory_table, long icf, long dcf) {
    char *final_filename = add_extension_to_file(file_name, ".ob"), hex_value[3];
    long address_counter = 100, counter = 0;
    unsigned int cur_cell;
    FILE *ob_file;

    ob_file = fopen(final_filename, "w");
	/* checking if the malloc function worked */
    if (ob_file == NULL) {
        fprintf(stderr, "c language error: can't create the file");
        free(final_filename);
        return;
    }

    /* writing amount of cells, of the data and of the insturction */
    fprintf(ob_file, "     %ld %ld\n", icf - 100, dcf);

    /* first element of the list is a garbage value */
    ic_memory_table = ic_memory_table->next;

    /* writing the cell of the instructions */
    while (ic_memory_table != NULL) {
        if (counter % 4 == 0 && ic_memory_table != NULL) { /* creating a new line when we are after writing 4 cells */
            if(counter != 0)
                fprintf(ob_file, "%c", '\n');
            fprintf(ob_file, "%04ld ", address_counter);
            address_counter += 4;
        }
        counter++;

        cur_cell = ic_memory_table->cell; /* the value of the current cell */

        sprintf(hex_value, "%02x", cur_cell); /* hex value of the cell */
        hex_value[2] = '\0'; /* string terminator */
        to_upper_string(hex_value); /* uppering the hex value to meet the requirements of the task */
        fprintf(ob_file, "%s ", hex_value); /* writing the value to the file */

        ic_memory_table = ic_memory_table->next; /* moving to next element */
    }

    /* first element of the list is a garbage value */
    dc_memory_table = dc_memory_table->next;

    /* writing the data to the file */
    while (dc_memory_table != NULL) {
        if (counter % 4 == 0 && dc_memory_table != NULL) { /* creating a new line when we are after writing 4 cells */
            if(counter != 0)
                fprintf(ob_file, "%c", '\n');
            fprintf(ob_file, "%04ld ", address_counter);
            address_counter += 4;
        }
        counter++;

        cur_cell = dc_memory_table->cell; /* the value of the current cell */

        sprintf(hex_value, "%02x", cur_cell); /* hex value of the cell */
        hex_value[2] = '\0'; /* string terminator */
        to_upper_string(hex_value); /* uppering the hex value to meet the requirements of the task */
        fprintf(ob_file, "%s ", hex_value); /* writing the value to the file */

        dc_memory_table = dc_memory_table->next; /* moving to next element */
    }

    /* freeing the variables */
    free(final_filename);
    fclose(ob_file);
}

/*
	this function creates an empty "ob" file via the file_name parameter 
	the content of this file is: " 0  0 ", the function is useful when we see a extreme case
*/
void write_blank_object_file(char *file_name){
    char *final_filename = add_extension_to_file(file_name, ".ob");
    FILE *ob_file;

    ob_file = fopen(final_filename, "w");
    /* checking if the malloc function worked */
    if (ob_file == NULL) {
        fprintf(stderr, "c language error: can't create the file");
        free(final_filename);
        return;
    }

    fprintf(ob_file, "     0 0"); /* writing the value to the file */

    /* freeing the variables */
    free(final_filename);
    fclose(ob_file);
}

void write_entry_file(char *file_name, symbol_table *table){
    FILE *ent_file;
    char *final_filename = add_extension_to_file(file_name, ".ent");

    ent_file = fopen(final_filename, "w");
    /* checking if the malloc function worked */
    if (ent_file == NULL) {
        fprintf(stderr, "c language error: can't create the file");
        free(final_filename);
        return;
    }

    /* first element of the list is a garbage value */
    table = table->next;
	
    /* writing the data to the file */
    while (table != NULL) {
        fprintf(ent_file, "%s ", table->name);
        fprintf(ent_file, "%04ld\n", table->address);
        table = table->next; /* moving to next element */
    }

    /* freeing the variables */
    free(final_filename);
    fclose(ent_file);
}

/* this function creates a file of externals via the file_name parameter */
void write_extern_file(char *file_name, extern_node *list){
    FILE *ext_file;
    char *final_filename = add_extension_to_file(file_name, ".ext");

    ext_file = fopen(final_filename, "w");
    if (ext_file == NULL) {
        printf("can't create file");
        return;
    }

    /* first element of the list is a garbage value */
    list = list->next;

    /* writing the data to the file */
    while (list != NULL) {
        fprintf(ext_file, "%s ", list->name);
        fprintf(ext_file, "%04ld\n", list->place);
        list = list->next; /* moving to next element */
    }

    /* freeing the variables */
    free(final_filename);
    fclose(ext_file);
}
