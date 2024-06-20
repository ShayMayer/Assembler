/* the file contains functions that are respinsible for creating the output file of the assembler */
#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "memory_table.h"
#include "symbol_table.h"
#include "extern_list.h"

/* creates an object file via the file_name parameter */
void write_object_file(char *file_name , memory_table *ic_memory_table, memory_table *dc_memory_table, long icf, long dcf);

/*
    creates an empty "ob" file via the file_name parameter the content of this 
	file is: " 0  0 ", the function is useful when we encouner an extreme case
*/
void write_blank_object_file(char *file_name);

/* creates a file of entries via the file_name parameter */
void write_entry_file(char *file_name, symbol_table *table);

/* creates a file of externals via the file_name parameter */
void write_extern_file(char *file_name, extern_table *table);

#endif
