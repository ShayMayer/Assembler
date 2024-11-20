/* this file contains general information about the assembler */
#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_LINE_LENGTH 80 /* line's max allowed length */
#define IC_INIT_VALUE 100 /* instruction counter's initial value */
#define MEMORY_SIZE 33554432 /* max memory cells amount */
#define COMMENT_SYMBOL ';' /* comment symbol */
#define QUOTE_SYMBOL '\"' /* quote symbol */
#define EXTERNAL_INSTRUCTION_ADDRESS 0 /* address of external instructions */

/* enum of booleans */
typedef enum booleans {
    FALSE = 0, TRUE = 1
} bool;

/* contains information about the file */
typedef struct file_info {
    char *name; /* file's name*/
    long cur_line_number; /* current line to process */
    char *cur_line_content; /* current line's content */
} file_info;

#endif
