/* this file contains useful functions and enums */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* takes a string and makes its letters upper */
void to_upper_string(char *str) {
    int i = 0;

    while (!end_of_line(str, i)) {
        str[i] = toupper(str[i]);
        i++;
    }
}

/* says whether the given file name(string) ends with the .as extension */
bool extension_of_as(char *file_name) {
    char extension[3] = {'.', 'a', 's'}; /* the as extension */
    int i;
    int j;

    /* the file name's length must be atleast 3 */
    if (strlen(file_name) <= 3)
        return FALSE;

    /* a loop from end to start */
    for (i = strlen(file_name) - 1, j = 2; j >= 0; i--, j--)
        if (file_name[i] != extension[j]) /* cheking corresponding chars */
            return FALSE;
    return TRUE;
}

/* says whether the given line should be processed by the assembler */
bool is_empty_line(char* line, int i){
	i = skip_spaces(line, i); /* skips white spaces */
	return end_of_line(line, i); /* says whether it reached the end of the line */
}

/* says whether we've reached the end of the instruction */
bool end_of_line(char *line, int i){
    if(i >= strlen(line) || line[i] == '\n' || line[i] == COMMENT_SYMBOL || line[i] == EOF)
        return TRUE;
    return FALSE;
}

/* returns the clear file name (the given file name without the extension) */
char* get_clear_file_name(char* file_name_with_extension){
    char *clear_file_name = (char *) (malloc((strlen(file_name_with_extension) - 2) * (sizeof(char))));
    int i = 0, ending_index = strlen(file_name_with_extension) - 1;

    if (clear_file_name == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* decrements the ending index until we see a dot(looks for the last dot in the string) */
    while (ending_index >= 0 && file_name_with_extension[ending_index] != '.')
        ending_index--;

    /* iterates through the given file name and copying it to the clear file name until the last dot */
    while (i < ending_index) {
        clear_file_name[i] = file_name_with_extension[i];
        i++;
    }
    clear_file_name[i] = '\0';

    return clear_file_name;
}

/* takes a file name and adds to it the given extension */
    char* add_extension_to_file(char* clear_file_name, char* extension){
    char* file_name = (char*)(malloc((strlen(clear_file_name) + strlen(extension) + 1)*(sizeof(char))));

    if(file_name == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    strcpy(file_name, clear_file_name);
    strcat(file_name, extension);
    return file_name;
}

/* skips white spaces(increments line's index) */
int skip_spaces(char *str, int i){
    while(!end_of_line(str, i) && isspace(str[i]))
        i++;
    return i;
}

/* returns the int value of the given register(string) */
int get_register(char *register_content) {
    int i;
    char reg_value[3]; /* register's value as a string */

	/* skiping the num's sign */
    for(i = 1; !end_of_line(register_content, i) && (register_content[i] == '+' || register_content[i] == '-'); i++)
    	;
    
    reg_value[0] = register_content[i]; 
    reg_value[1] = register_content[i+1]; 
    reg_value[2] = '\0'; 
    return atoi(reg_value); /* converts the string to an int */
}

/* returns the given string as long */
long get_num(char *expression){
    char *temp_ptr, *value_as_string;
    long value;
    int sign = 1, i, j;
    
    for(i = 0; !end_of_line(expression, i) && (expression[i] == '+' || expression[i] == '-'); i++)
    	if(expression[i] == '-') sign *= -1;	
    
    value_as_string = (char*)(malloc((strlen(expression) - i)*(sizeof(char))));
    
    j = i;
    for(; !end_of_line(expression, i); i++)
    	value_as_string[i - j] = expression[i];
    
    value = strtol(value_as_string, &temp_ptr, 10);
    
    free(value_as_string);

    return value * sign;
}

/* says whether the given string is an integer */
bool is_int(char *expression, int from){
    for(; !end_of_line(expression, from) && (expression[from] == '+' || expression[from] == '-'); from++)
        ;

    /* checks whether all the string's char are digits */
    for (; !end_of_line(expression, from); from++)
        if (!isdigit(expression[from]))
            return FALSE;        
	    
    return from > 0; /* checks whether the string is empty */
}

/* says whether the given string is a valid register */
register_format_error is_valid_register(char *expression){
    int i, sign = 1, reg_value = 0;

    if(expression[0] != '$') /* checks whether the register symbol found */
        return REG_DOLLAR_SIGN_NOT_FOUND;

    if(strlen(expression) == 1) /* checks whether the length is valid */
        return REG_NOTHING_AFTER_DOLLAR_SIGN;

    if(!is_int(expression, 1)) /* checks whether the given string is an integer */
        return REG_NOT_NUMBER;

    for(i = 1; !end_of_line(expression, i) && (expression[i] == '+' || expression[i] == '-'); i++)
        if(expression[i] == '-') sign *= -1;	
    if(sign == -1){
        return REG_NOT_IN_RANGE;
    }

    /* converts the string to an integer(we can do it becasue we've already validated the string) */
    for(; i < strlen(expression); i++){
        reg_value *= 10;
        reg_value += (expression[i] - '0');

        if(reg_value > 31) /* checks whether the number is out of range */
            return REG_NOT_IN_RANGE;
    }

    return REG_NONE; /* the given register a valid register */
}

/* gets the instruction's label from the given string, returns the updated index */
int get_label(char str[], char buf[], int i){
    int j = 0;

    /* copies the string until it encounters a white space or reaches the end of the line */
    while(!end_of_line(str, i) && !isspace(str[i])) { 
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0';
    return i;
}

/*
    gets the instruction's label from the given string
    it stops reading when it encounters the given symbol
    returns the updated index 
*/
int get_label_until(char str[], char buf[], int i, char token){
    int j = 0;

    /* copies the string until it encounters the given token or a white space or reaches the end of the line */
    while(!end_of_line(str, i) && !isspace(str[i]) && str[i] != token) {  
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0';

    return i;
}

/* checks whether the given string is a valid label */
label_format_error is_valid_label(char *expression){
    int i;
    if(strlen(expression) > 31) /* checks whether the label is too long */
        return LBL_TOO_LONG;

    if(!isalpha(expression[0])) /* checks whether the label's first char is a letter */
        return LBL_FIRST_CHAR_NOT_ALPHA;

    for(i = 1; i < strlen(expression); i++)
        if(!isalpha(expression[i]) && !isdigit(expression[i])) /* checks whether the current char is a number or a letter */
            return LBL_CHAR_NOT_ALPHA_OR_DIGIT;

    return LBL_NONE; /* the given label isn't a valid label */
}
