/* this file represents an assembler */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory_table.h"
#include "symbol_table.h"
#include "globals.h"
#include "file_writer.h"
#include "utils.h"
#include "second_pass.h"
#include "first_pass.h"

/* takes a file name and tries to assemble its source */
void assemble(char *file_name) {
    int cur_char; /* the current char in the current line to be assembled */
    char cur_line[MAX_LINE_LENGTH] /* the current line to be assembled */, *clear_file_name; /* the file name without the extension */
    long ic = IC_INIT_VALUE /* instruction couter */, dc = 0 /* data couter */, icf /* final instruction couter */, dcf /* final instruction couter */;
    FILE *asm_file; /* the assembly file */
    bool is_success = TRUE; /* status that says if the line to be assembled is valid or not */
    symbol_table *sym_table = create_symbol_table() /* the symbol table */, *entry_table;
    symbol_element *tmp; /* the entry table */
    memory_table *ic_memory_table /* the instruction memory table */, *dc_memory_table /* the data memory table */;
    extern_table *extern_list; /* the list of externals labels */
    file_info cur_file_info; /* contains information about the current file */

    /* checks whether the given file name is valid */
    if (!extension_of_as(file_name)) {
        fprintf(stderr,"the format of %s is not correct(should be <file name>.as) \n", file_name);
        return;
    }

    asm_file = fopen(file_name, "r");
    if (asm_file == NULL) {
        fprintf(stderr, "can't open file: %s\n", file_name);
        return;
    }

    /* sets info about the file */
    cur_file_info.name = file_name;
    cur_file_info.cur_line_content = cur_line;

    /* iterates through the lines of the assembler and checks whether they are valid instructions */
    for (cur_file_info.cur_line_number = 1; !feof(asm_file) && fgets(cur_line, MAX_LINE_LENGTH, asm_file) != NULL; cur_file_info.cur_line_number++) {
        if (strchr(cur_line, '\n') == NULL && !feof(asm_file)) { /* new line char not found, means line contains more chars than allowd */
            fprintf(stderr, "%s:%ld: line too long(contains more than 80 chars)\n", file_name, cur_file_info.cur_line_number);
            is_success = FALSE;

            do {
                cur_char = fgetc(asm_file); 
            } while (cur_char != '\n' && cur_char != EOF); /* proceeds to then next instruction(line) */
        } else {
            if (!validate_command(cur_file_info, sym_table, &ic, &dc)) { /* checks whether the instruction is valid */
                if (is_success)
                    is_success = FALSE;
            }

            if(((ic - 100) + dc) > MEMORY_SIZE) { /* checks whether the memory required is too much */
                fprintf(stderr, "%s:%ld: not enough memory in the processor\n", file_name, cur_file_info.cur_line_number);
                return;
            }
        }
    }

    icf = ic; /* saving the amount of instructions*/
    dcf = dc; /* saving the amount of data */

    if (is_success) { /* checks whether there wasn't any issue with the first pass */
        ic_memory_table = create_memory_table(); /* creates the instruction memory table */
        dc_memory_table = create_memory_table(); /* creates the data memory table */
        extern_list = create_extern_table(); /* creates a list of externals */

        dc = 0; /* resets the value of data counter for later use */

        tmp = sym_table->head;
        while (tmp != NULL) {
            if(exists(sym_table, tmp->name, DATA_SYMBOL))
                (tmp->address) += ic;
            tmp = tmp->next;
        }

	/* goes back to the beginnig of the file(for beggining the second pass) */
        rewind(asm_file);

        ic = IC_INIT_VALUE; /* resets the value of data counter for later use */
		
	/* iterates through the lines and trying to assemble them */
        for (cur_file_info.cur_line_number = 1; !feof(asm_file) && fgets(cur_line, MAX_LINE_LENGTH, asm_file) != NULL; cur_file_info.cur_line_number++) {
            /* checks whether an error occurred */
            if(!assemble_command(cur_file_info, ic_memory_table, dc_memory_table, sym_table, extern_list, &ic, &dc)) {
                if (is_success)
                    is_success = FALSE;
            }
        }

        if(is_success) {
	    /* gets the name of the file(with out the extension) */
            clear_file_name = get_clear_file_name(cur_file_info.name);

	    /* cheks whether the memory tables are empty(it's for cosmetic stuff) */
            if(!mem_is_empty(ic_memory_table) || !mem_is_empty(dc_memory_table)) write_object_file(clear_file_name, ic_memory_table, dc_memory_table, icf, dcf);
            else write_blank_object_file(clear_file_name);

	    /* gets all the labels which are entrys */
            entry_table = table_of_attributes(sym_table, ENTRY_SYMBOL);
	    /* checks whether the entry table is empty(it's for cosmetic stuff) */
            if(!sym_is_empty(entry_table)) write_entry_file(clear_file_name, entry_table);

	    /* checks whether the extern table is empty(it's for cosmetic stuff) */
            if(!extern_is_empty(extern_list)) write_extern_file(clear_file_name, extern_list);

	    /* frees the variables */
            free_symbol_table(entry_table);
            free(clear_file_name);
        }

	/* frees the variables */
        free_memory_table(ic_memory_table);
        free_memory_table(dc_memory_table);
        free_extern_table(extern_list);
    }

	/* frees the variables */
    fclose(asm_file);
    free_symbol_table(sym_table);
}

/* takes the command line argument and passes it to the assembler */
int main(int argc, char *argv[]) {
    int i;
    if(argc == 1) /* checks whether any file name has been provided */
        fprintf(stderr, "you didn't provide enough files(less than 1)");

    /* passes the file names to the assembler */
    for(i = 1; i < argc; i++)
        assemble(argv[i]); 

    return 0;
}

