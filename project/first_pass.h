/* this file contains functions for the first pass */
#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "symbol_table.h"
#include "memory_table.h"

/* takes a line and validates it */
bool validate_command(file_info f_info, symbol_table *table, long *ic, long *dc);

#endif
