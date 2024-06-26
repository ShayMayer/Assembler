/* this file contains functions for the second pass */
#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "code.h"
#include "memory_table.h"
#include "symbol_table.h"
#include "globals.h"
#include "extern_list.h"

/* takes a line and tries to assemble it */
bool assemble_command(file_info f_info, memory_table *ic_memory_table, memory_table *dc_memory_table, symbol_table *s_table, extern_table *extern_list, long *ic, long *dc);

#endif
