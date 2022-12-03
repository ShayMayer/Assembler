/* this file contains functions and enums that have a lot of usage in the code  */
#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

/* this enum contains types of errors about the register format, related to the first pass */
typedef enum register_format_error {
    REG_DOLLAR_SIGN_NOT_FOUND = 0, /* dollar sign doesn't exist */
    REG_NOTHING_AFTER_DOLLAR_SIGN, /* there is only the dollar sign */
    REG_NOT_NUMBER, /* what comes after the dollar sign is not a number */
    REG_NOT_IN_RANGE /* the number that comes after the dollar sign is too large ot to liitle */,
    REG_NONE /* the given expression is a valid register */
} register_format_error;

/* this enum contains types of errors about the label format, related to the first pass */
typedef enum label_format_error {
    LBL_FIRST_CHAR_NOT_ALPHA = 0, /* the first char of the given expresion in not an alpha */
    LBL_TOO_LONG, /* the length of the label is too long */
    LBL_CHAR_NOT_ALPHA_OR_DIGIT, /* the given expression contains a char which is not an alpha and not a digit */
    LBL_NONE /* the given expression is a valid label */
} label_format_error;

/* this function takes a string a replace the small letters with capital letters */
void to_upper_string(char *str);

/* this function returns TRUE if the extension of the given char is "as" and FALSE otherwise */
bool extension_of_as(char *file_name);

/* this function returns TRUE if the given line is empty or a comment(according to the task) and FALSE otherwise */
bool is_empty_line_or_comment_line(char* line, int i);

/* this function returns TRUE if the given index indicates on the end of the line and FALSE otherwise */
bool end_of_line(char *line, int i);

/* this function takes a file name with extension and returns the clear name(without the extension) */
char* get_clear_file_name(char* file_name_with_extension);

/* this function takes a file name without any extension and adds to it the given extension */
char* add_extension_to_file(char* clear_file_name, char* extension);

/* this function incrementing the given counter until we see a char which is not a white space */
int skip_spaces(char *str, int i);

/* this function takes an expression and convert it to long */
long get_num(char *expression);

/* this function returns the value of a given register */
int get_register(char *register_content);

/* the function takes an expression if return TRUE if it's a valid register and FALSE otherwise */
register_format_error is_valid_register(char *expression);

/* this function takes an expression return TRUE if it's an integer and FALSE otherwise */
bool is_int(char *expression, int form);

/* the function takes a string, gets a label from it, and stores it in buf, and returns the index after getting the label */
int get_label(char str[], char buf[], int i);

/*
	the function takes a string, gets a label from it, and stores it in buf, and returns the index after getting the label
	the function stops reading the label when it sees the given char(token)	
 */
int get_label_until(char str[], char buf[], int i, char token);

/* the function takes an expression if return TRUE if it's a valid label and FALSE otherwise */
label_format_error is_valid_label(char *expression);

#endif
