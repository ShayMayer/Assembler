/* this file contains functions and enums that have a lot of usage in the code  */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* this function takes a string a replace the small letters with capital letters */
void to_upper_string(char *str) {
    int i = 0;

	/* iterates through the given string */
    while (!end_of_line(str, i)) {
        str[i] = toupper(str[i]); /* replace the current letter with a capital letter */
        i++;
    }
}

/* this function returns TRUE if the extension of the given char is "as" and FALSE otherwise */
bool extension_of_as(char *file_name) {
    char extension[3] = {'.', 'a', 's'}; /* the as extension */
    int i;
    int j;

    /* if the condition is TRUE than it means the string is too short */
    if (strlen(file_name) <= 3)
        return FALSE;

    /* starting from the end of the string and comparing to the as extension */
    for (i = strlen(file_name) - 1, j = 2; j >= 0; i--, j--)
        if (file_name[i] != extension[j]) /* means the string has an invalid extension */
            return FALSE;
    return TRUE;
}

/* this function returns TRUE if the given line is empty or a comment(according to the task) and FALSE otherwise */
bool is_empty_line_or_comment_line(char* line, int i){
    if(!end_of_line(line, i))
        return (line[i] == ';');
    return TRUE;
}

/* this function returns TRUE if the given index indicates on the end of the line and FALSE otherwise */
bool end_of_line(char *line, int i){
    if(i >= strlen(line) || line[i] == '\n' || line[i] == EOF)
        return TRUE;
    return FALSE;
}

/* this function takes a file name with extension and returns the clear name(without the extension) */
char* get_clear_file_name(char* file_name_with_extension){
    char *clear_file_name = (char *) (malloc((strlen(file_name_with_extension) - 2) * (sizeof(char))));
    int i = 0, ending_index = strlen(file_name_with_extension) - 1;

    /* checking if the malloc function worked */
    if (clear_file_name == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* decrementing the ending index until we see a dot(we actually look for the last dot in the string) */
    while (ending_index >= 0 && file_name_with_extension[ending_index] != '.')
        ending_index--;

    /*
        iterating through the given file name and copying it to the clear file name
        until the last dot
     */
    while (i < ending_index) {
        clear_file_name[i] = file_name_with_extension[i];
        i++;
    }
    clear_file_name[i] = '\0'; /* string terminator */

    return clear_file_name;
}

/* this function takes a file name without any extension and adds to it the given extension */
    char* add_extension_to_file(char* clear_file_name, char* extension){
    char* file_name = (char*)(malloc((strlen(clear_file_name) + strlen(extension) + 1)*(sizeof(char))));

    /* checking if the malloc function worked */
    if(file_name == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    strcpy(file_name, clear_file_name);
    strcat(file_name, extension);
    return file_name;
}

/* this function incrementing the given counter until we see a char which is not a white space */
int skip_spaces(char *str, int i){
    while(!end_of_line(str, i) && isspace(str[i]))
        i++;
    return i;
}

/* this function takes an expression and convert it to long */
long get_num(char *expression){
    char *temp_ptr;
    long value = strtol(expression, &temp_ptr, 10);
    return value;
}

/* the function takes an expression if return TRUE if it's a valid register and FALSE otherwise */
register_format_error is_valid_register(char *expression){
    int i = 1;
    int reg_value = 0; /* the value of the register(what comes after the dollar sign) */

    if(expression[0] != '$') /* means that the dollar sign doesn't exist */
        return REG_DOLLAR_SIGN_NOT_FOUND;

    if(strlen(expression) == 1) /* means taht there is only the dollar sign */
        return REG_NOTHING_AFTER_DOLLAR_SIGN;

    if(!is_int(expression, 1)) /* means what comes after the dollar sign is not a number */
        return REG_NOT_NUMBER;

    if(expression[i] == '-') /* means what comes after the dollar sign is a negative number(too little) */
        return REG_NOT_IN_RANGE;

    if(expression[i] == '+') /* a plus comes before the number is allowd */
        i++;

    /* reading the number from the string */
    for(; i < strlen(expression); i++){
		/* the two lines bellow are responsible for readingthe next digit of the number */
        reg_value *= 10;
        reg_value += (expression[i] - '0');

        if(reg_value > 31) /* means the number is too large */
            return REG_NOT_IN_RANGE;
    }

    return REG_NONE; /* means the expression is a valid register */
}

/* this function takes an expression return TRUE if it's an integer and FALSE otherwise */
bool is_int(char *expression, int from){
    if (expression[from] == '-' || expression[from] == '+') expression++; /* ignoring plus and minus in the beginning */
    for (; expression[from]; from++) { /* this loop checks if all the chars in this string are digits */
        if (!isdigit(expression[from])) {
            return FALSE;
        }
    }
    return from > 0; /* this means the string is empty */
}

/* the function takes a string, gets a label from it, and stores it in buf, and returns the index after getting the label */
int get_label(char str[], char buf[], int i){
    int j = 0;

    /* the reading of the label continues until we see a white space or we are at the end of the line */
    while(!end_of_line(str, i) && !isspace(str[i])) { 
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0'; /* string terminator */
    return i;
}

/*
	the function takes a string, gets a label from it, and stores it in buf, and returns the index after getting the label
	the function stops reading the label when it sees the given char(token)	
 */
int get_label_until(char str[], char buf[], int i, char token){
    int j = 0;

    /* the reading of the label continues until we see a white space or we see the given char(token), or we are at the end of the line */
    while(!end_of_line(str, i) && !isspace(str[i]) && str[i] != token) {  
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0'; /* string terminator */

    return i;
}

/* the function takes an expression if return TRUE if it's a valid label and FALSE otherwise */
label_format_error is_valid_label(char *expression){
    int i;
    if(strlen(expression) > 31) /* means the expression is too long */
        return LBL_TOO_LONG;

    if(!isalpha(expression[0])) /* means the first char of the expression is not an alpha */
        return LBL_FIRST_CHAR_NOT_ALPHA;

    for(i = 1; i < strlen(expression); i++)
        if(!isalpha(expression[i]) && !isdigit(expression[i])) /* means the one of the chars in the expression is not an alpha and not a digit */
            return LBL_CHAR_NOT_ALPHA_OR_DIGIT;

    return LBL_NONE; /* the expression is a valid label */
}
