/* this file contains useful functions and enums */
#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

/* enum containing all types of register errors, related to the first pass */
typedef enum register_format_error {
    REG_DOLLAR_SIGN_NOT_FOUND = 0, /* dollar sign doesn't exist */
    REG_NOTHING_AFTER_DOLLAR_SIGN, /* there is only the dollar sign */
    REG_NOT_NUMBER, /* what comes after the dollar sign is not a number */
    REG_NOT_IN_RANGE /* the number that comes after the dollar sign is too large ot to liitle */,
    REG_NONE /* the given expression is a valid register */
} register_format_error;

/* enum containing all types of label errors, related to the first pass */
typedef enum label_format_error {
    LBL_FIRST_CHAR_NOT_ALPHA = 0, /* the first char of the given expresion in not an alpha */
    LBL_TOO_LONG, /* the length of the label is too long */
    LBL_CHAR_NOT_ALPHA_OR_DIGIT, /* the given expression contains a char which is not an alpha and not a digit */
    LBL_NONE /* the given expression is a valid label */
} label_format_error;

/* takes a string and makes its letters upper */
void to_upper_string(char *str);

/* says whether the given file name(string) ends with the .as extension */
bool extension_of_as(char *file_name);

/* says whether the given line should be processed by the assembler */
bool is_empty_line(char* line, int i);

/* says whether we've reached the end of the instruction */
bool end_of_line(char *line, int i);

/* returns the clear file name (the given file name without the extension) */
char* get_clear_file_name(char* file_name_with_extension);

/* takes a file name and adds to it the given extension */
char* add_extension_to_file(char* clear_file_name, char* extension);

/* skips white spaces(increments line's index) */
int skip_spaces(char *str, int i);

/* returns the int value of the given register(string) */
int get_register(char *register_content);

/* returns the given string as long */
long get_num(char *expression);

/* says whether the given string is an integer */
bool is_int(char *expression, int form);

/* says whether the given string is a valid register */
register_format_error is_valid_register(char *expression);

/* gets the instruction's label from the given string, returns the updated index */
int get_label(char str[], char buf[], int i);

/*
    gets the instruction's label from the given string
    it stops reading when it encounters the given symbol
    returns the updated index 
*/
int get_label_until(char str[], char buf[], int i, char token);

/* checks whether the given string is a valid label */
label_format_error is_valid_label(char *expression);

#endif
