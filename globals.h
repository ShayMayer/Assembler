/* this file contains general information about the assembler */
#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_LINE_LENGTH 80 /* maximum length of a line in an aseembly line to be allowd */
#define IC_INIT_VALUE 100 /* the initial value of the instruction counter according to the task */
#define MEMORY_SIZE 33554432 /* amount of cells that the computer has (according to the task) */

/* an enum of booleans */
typedef enum booleans {
    FALSE = 0, TRUE = 1
} bool;

/* contains information about the file */
typedef struct file_info {
    char *name; /* the name of the file */
    long cur_line_number; /* the current number of the line to be read in the assembly file */
    char *cur_line_content; /* the current line to be read in the assembly file */
} file_info;

#endif