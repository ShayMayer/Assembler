/* this file contains functions for the first pass */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "code.h"
#include "utils.h"
#include "first_pass.h"

/* takes a line and tries to find a label, if a valid label was found then it's added to the symbol table */
static int find_and_validate_label(file_info f_info, int i, char *label, symbol_table *table);

/* takes a r instruction(arithmetic and logic) and validates it */
static bool validate_r_instruction_group_1(file_info f_info, int i, long *ic);
/* takes a r instruction(copying) and validates it */
static bool validate_r_instruction_group_2(file_info f_info, int i, long *ic);
/* takes a r instruction and validates it */
static bool validate_r_command(file_info f_info, int i, long *ic, int args_amount_expected);

/* takes an i instruction(arithmetic and logic) and validates it */
static bool validate_i_instruction_group_1(file_info f_info, int i, long *ic);
/* takes an i instruction(jumping) and validates it */
static bool validate_i_instruction_group_2(file_info f_info, int i, long *ic);
/* takes an i instruction(loading and saving in memory) and validates it */
static bool validate_i_instruction_group_3(file_info f_info, int i, long *ic);
/* takes an i instruction(that only has one label) and validates it */
static bool validate_i_instruction_no_label(file_info f_info, int i, long *ic);

/* takes a jmp instruction and validates it */
static bool validate_jmp_instruction(file_info f_info, int i, long *ic);
/* takes a la instruction and validates it */
static bool validate_la_instruction(file_info f_info, int i, long *ic);
/* takes a call instruction and validates it */
static bool validate_call_instruction(file_info f_info, int i, long *ic);
/* takes a stop instruction and validates it */
static bool validate_stop_instruction(file_info f_info, int i, long *ic);
/* takes a j instruction and validates it */
static bool validate_j_instruction_label_only(file_info f_info, int i, long *ic);

/* takes a db instruction and validates it */
static bool validate_db_instruction(file_info f_info, int i, long *dc);
/* takes a dh instruction and validates it */
static bool validate_dh_instruction(file_info f_info, int i, long *dc);
/* takes a dw instruction and validates it */
static bool validate_dw_instruction(file_info f_info, int i, long *dc);
/* takes info about a data instruction and validates it */
static bool validate_num_creation_instruction(file_info f_info, int i, long *dc, int cells_amount);

/* takes an asciz instruction and validates it */
static bool validate_asciz_instruction(file_info f_info, int i, long *dc);

/* takes an entry instruction and validates it */
static bool validate_entry_instruction(file_info f_info, int i);
/* takes an extern instruction and validates it */
static bool validate_extern_instruction(file_info f_info, int i, symbol_table *table);

/* takes a line and validates it */
bool validate_command(file_info f_info, symbol_table *table, long *ic, long *dc){
    char label[MAX_LINE_LENGTH], instruction[MAX_LINE_LENGTH];
    int i = 0; /* current index */
    bool label_found = FALSE; /* says whether a label was found */
    instruction_info *instruct_info; /* information about the instruction */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    if(is_empty_line(f_info.cur_line_content, i)) return TRUE; /* checks whether the line should be processed */

    i = find_and_validate_label(f_info, i, label, table); /* looks for a label and updates the index */
    if(i == -1) return FALSE; /* an invalid label was found */

    if(i != 0) label_found = TRUE; /* a valid label was found */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    i = get_label_until(f_info.cur_line_content, instruction, i, ','); /* gets the instruction's name */

    /* gets info about the instruction */
    instruct_info = get_instruction_info(instruction);

    /* checks the insruction's type */
    if(instruct_info != NULL) {
        switch(instruct_info->id){
            case ADD:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_1(f_info, i, ic);
            case SUB:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_1(f_info, i, ic);
            case AND:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_1(f_info, i, ic);
            case OR:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_1(f_info, i, ic);
            case NOR:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_1(f_info, i, ic);
            case MOVE:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_2(f_info, i, ic);
            case MVHI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_2(f_info, i, ic);
            case MVLO:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_r_instruction_group_2(f_info, i, ic);
            case ADDI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_1(f_info, i, ic);
            case SUBI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_1(f_info, i, ic);
            case ANDI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_1(f_info, i, ic);
            case ORI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_1(f_info, i, ic);
            case NORI:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_1(f_info, i, ic);
            case BNE:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_2(f_info, i, ic);
            case BEQ:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_2(f_info, i, ic);
            case BLT:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_2(f_info, i, ic);
            case BGT:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_2(f_info, i, ic);
            case LB:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case SB:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case LW:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case SW:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case LH:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case SH:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_i_instruction_group_3(f_info, i, ic);
            case JMP:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_jmp_instruction(f_info, i, ic);
            case LA:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_la_instruction(f_info, i, ic);
            case CALL:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_call_instruction(f_info, i, ic);
            case STOP:
                if(label_found == TRUE) add_symbol_item(table, label, *ic, CODE_SYMBOL);
                    return validate_stop_instruction(f_info, i, ic);
            case DB:
                if(label_found == TRUE) add_symbol_item(table, label, *dc, DATA_SYMBOL);
                    return validate_db_instruction(f_info, i, dc);
            case DH:
                if(label_found == TRUE) add_symbol_item(table, label, *dc, DATA_SYMBOL);
                    return validate_dh_instruction(f_info, i, dc);
            case DW:
                if(label_found == TRUE) add_symbol_item(table, label, *dc, DATA_SYMBOL);
                    return validate_dw_instruction(f_info, i, dc);
            case ASCIZ:
                if(label_found == TRUE) add_symbol_item(table, label, *dc, DATA_SYMBOL);
                    return validate_asciz_instruction(f_info, i, dc);
            case ENTRY:
                if(label_found == TRUE){ /* label before an entry instruction is not allowd */
                    fprintf(stderr, "%s:%ld: the label \"%s\" can't be defined in entry instruction\n", f_info.name, f_info.cur_line_number, label);
                    return FALSE;
                }
                return validate_entry_instruction(f_info, i);
            case EXTERN:
                if(label_found == TRUE){ /* label before an extern instruction is not allowd */
                    fprintf(stderr, "%s:%ld: the label \"%s\" can't be defined in extern instruction\n", f_info.name, f_info.cur_line_number,label);
                    return FALSE;
                }
                return validate_extern_instruction(f_info, i, table);
            default: break;
        };
    }

    /* the lines below find out why the instruction isn't valid */
    if(f_info.cur_line_content[i] == ','){
        if(strcmp(instruction, "") == 0) fprintf(stderr, "%s:%ld: unexpected comma before instruction\n", f_info.name, f_info.cur_line_number);
        else fprintf(stderr, "%s:%ld: \"%s\" is not a defined instruction\n", f_info.name, f_info.cur_line_number, instruction);
        return FALSE;
    }

    if(strcmp(instruction, "") == 0) fprintf(stderr, "%s:%ld: no instruction after label\n", f_info.name, f_info.cur_line_number);
    else fprintf(stderr, "%s:%ld: \"%s\" is not a defined instruction\n", f_info.name, f_info.cur_line_number, instruction);

    return FALSE;
}

/* takes a line and tries to find a label, if a valid label was found then it's added to the symbol table */
static int find_and_validate_label(file_info f_info, int i, char *label, symbol_table *table){
    label_format_error label_error; /* info about label erros */

    i = get_label_until(f_info.cur_line_content, label, i, ':'); /* gets all the chars until it encounters a ':' */

    /* checks whether what it found is a label(must end with ':') */
    if(f_info.cur_line_content[i] != ':')
        return 0;

    /* a space should come after the ':', if it doesn't then the label isn't valid */
    if((i + 1 < strlen(f_info.cur_line_content)) && !isspace(f_info.cur_line_content[i + 1]))
        return 0;

    if(strlen(label) == 0){ /* label is empty */
        fprintf(stderr, "%s:%ld: \":\" is not a valid label\n", f_info.name, f_info.cur_line_number);
        return -1;
    }

    label_error = is_valid_label(label); /* looks for errors */

    if(label_error != LBL_NONE){ /* means the label is not valid */
        if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, label);
        else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, label);
        else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, label);

        return -1;
    }

    /* checks whether the label's name is a saved keyword */
    if(get_instruction_info(label) != NULL) {
        fprintf(stderr, "%s:%ld: the label \"%s\" can't have an instruction name\n", f_info.name, f_info.cur_line_number, label);
        return -1;
    }

    /* checks whether the label is already defined(only one definition is allowed) */
    if(exists(table, label, CODE_SYMBOL | DATA_SYMBOL)){
        fprintf(stderr, "%s:%ld: the label \"%s\" already exists\n", f_info.name, f_info.cur_line_number, label);
        return -1;
    }

    return i + 1; /* the label is valid, returns the current index */
}

/* takes a r instruction(arithmetic and logic) and validates it */
static bool validate_r_instruction_group_1(file_info f_info, int i, long *ic){
    return validate_r_command(f_info, i, ic, 3); /* this instruction has 3 operands */
}

/* takes a r instruction(copying) and validates it */
static bool validate_r_instruction_group_2(file_info f_info, int i, long *ic){
    return validate_r_command(f_info, i, ic, 2); /* this instruction has 2 operands */
}

/* takes a r instruction and validates it */
static bool validate_r_command(file_info f_info, int i, long *ic, int args_amount_expected) {
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether it enouctered a comma */
    register_format_error register_error; /* register errors status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    if (f_info.cur_line_content[i] == ',') { /* unexpected comma before operand detected */
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operands one by one and validates them, in addition it validates the syntax */
    while (!end_of_line(f_info.cur_line_content, i)) {
        comma_detected = FALSE; /* starts again, validating a new operand */

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

	/* first 3 operands should be registers, the others are just for amount handling */
        if (args_amount < args_amount_expected) {
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

            register_error = is_valid_register(expression); /* looks for errors */
            if (register_error != REG_NONE) { /* checks whether the register is valid */
                if (register_error == REG_DOLLAR_SIGN_NOT_FOUND) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(\"$\" not found)\n", f_info.name, f_info.cur_line_number, expression);
                else if (register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                else if (register_error == REG_NOT_NUMBER) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                else if (register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name,f_info.cur_line_number, expression);
                return FALSE;
            }
        } else /* it already encountered 3 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if (!end_of_line(f_info.cur_line_content, i)) {
            if (f_info.cur_line_content[i] != ',') { /* just encountered an unexpected comma */
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just encountered a comma */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if (comma_detected == TRUE) { /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if (args_amount > args_amount_expected) { /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if (args_amount < args_amount_expected) { /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes an i instruction(arithmetic and logic) and validates it */
static bool validate_i_instruction_group_1(file_info f_info, int i, long *ic){
    return validate_i_instruction_no_label(f_info, i, ic);
}

/* takes an i instruction(jumping) and validates it */
static bool validate_i_instruction_group_2(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whethera comma has been detected */
    register_format_error register_error; /* register error status */
    label_format_error label_error; /* label error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operands one by one and validates them, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }
		
        if(args_amount == 0 || args_amount == 1) { /* first 2 operands should be registers */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looks for errors */
            if(register_error != REG_NONE){ /* chekcs whether the register is valid */
                if(register_error == REG_DOLLAR_SIGN_NOT_FOUND) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(\"$\" not found)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_NUMBER) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }		
        else if(args_amount == 2) { /* third operand should be a label, the others are just for amount handling */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            label_error = is_valid_label(expression); /* looks for errors */

            if(label_error != LBL_NONE){ /* chekcs whether the label is valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* it already encountered 3 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 3){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount < 3){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes an i instruction(loading and saving in memory) and validates it */
static bool validate_i_instruction_group_3(file_info f_info, int i, long *ic){
    return validate_i_instruction_no_label(f_info, i, ic); /* i instruction(loading and saving in memory) */
}

/* takes an i instruction(that only has one label) and validates it */
static bool validate_i_instruction_no_label(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has been detected */
    register_format_error register_error; /* register error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operands one by one and validates them, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0 || args_amount == 2) { /* first and third operands should be registers */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looks for errors*/
            if(register_error != REG_NONE){ /* means the register is not valid */
                if(register_error == REG_DOLLAR_SIGN_NOT_FOUND) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(\"$\" not found)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_NUMBER) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else if(args_amount == 1) { /* second operand should be a literal, the others are just for amount handling */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            if(!is_int(expression, 0)){
                fprintf(stderr, "%s:%ld: \"%s\" is not a valid integer\n", f_info.name, f_info.cur_line_number, expression);
                return FALSE;
            }
        }
        else /* it already encountered 3 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 3){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount < 3){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 3)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

static bool validate_jmp_instruction(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has been detected */
    register_format_error register_error; /* register error status */
    label_format_error label_error; /* label error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label or a registers, the others are just for amount handling */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looks for errors */

            if (register_error != REG_NONE) { /* checks whether the register is valid */
                if (register_error != REG_DOLLAR_SIGN_NOT_FOUND) {
                    if (register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                    else if (register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name, f_info.cur_line_number, expression);
                    else fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                    return FALSE;
                }

                label_error = is_valid_label(expression); /* looks for errors */

                if(label_error != LBL_NONE){ /* checks whether the label is valid */
                    if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                    else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                    else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                    return FALSE;
                }
            }
        }
        else /* it already encountered 1 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 1){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount == 0){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 1))\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes a la instruction and validates it */
static bool validate_la_instruction(file_info f_info, int i, long *ic){
    return validate_j_instruction_label_only(f_info, i, ic);
}

/* takes a call instruction and validates it */
static bool validate_call_instruction(file_info f_info, int i, long *ic){
    return validate_j_instruction_label_only(f_info, i, ic);
}

/* takes a stop instruction and validates it */
static bool validate_stop_instruction(file_info f_info, int i, long *ic) {
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has been detected */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }
	
    /* validates the instruction */
    while (!end_of_line(f_info.cur_line_content, i)) {
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until we encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if (!end_of_line(f_info.cur_line_content, i)) {
            if (f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if (comma_detected == TRUE) { /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if (args_amount > 0) { /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 0)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes a j instruction and validates it */
static bool validate_j_instruction_label_only(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has been detected */
    label_format_error label_error; /* label error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

            label_error = is_valid_label(expression); /* looks for errors */

            if(label_error != LBL_NONE){ /* means the label is not valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* it already encounted 1 operand, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 1){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount == 0){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes a db instruction and validates it */
static bool validate_db_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 1);
}

/* takes a dh instruction and validates it */
static bool validate_dh_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 2);
}

/* takes a dw instruction and validates it */
static bool validate_dw_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 4);
}

/* takes info about a data instruction and validates it */
static bool validate_num_creation_instruction(file_info f_info, int i, long *dc, int cells_amount){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has been detected */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operands one by one and validating them, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        if(!is_int(expression, 0)){
            fprintf(stderr, "%s:%ld: \"%s\" is not a valid integer\n", f_info.name, f_info.cur_line_number, expression);
            return FALSE;
        }

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detetced */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        *dc += cells_amount;
        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount == 0){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    return TRUE;
}

/* takes an asciz instruction and validates it */
static bool validate_asciz_instruction(file_info f_info, int i, long *dc){
    int end_index = strlen(f_info.cur_line_content) - 1; /* end of the line */
    int comment_index = end_index; /* acutal end of line */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') { 
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* looks for the comment sign */
    while(comment_index >= 0 && f_info.cur_line_content[comment_index] != COMMENT_SYMBOL)
        comment_index--;
    
    if(comment_index >= 0) 
        end_index = comment_index - 1;    
        
    /* reaches the right quote */
    while(end_index >= 0){
        if(!isspace(f_info.cur_line_content[end_index])){
            if(f_info.cur_line_content[end_index] != QUOTE_SYMBOL) {
                fprintf(stderr, "%s:%ld: ending quote expected at the end\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            break;
        }
        end_index--;
    }

    /* reaches the left quote */
    while(!end_of_line(f_info.cur_line_content, i)){
        if(!isspace(f_info.cur_line_content[i])){
            if(f_info.cur_line_content[i] != QUOTE_SYMBOL) {
                fprintf(stderr, "%s:%ld: opening quote expected at the start\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            break;
        }
        i++;
    }
	
	if(i == end_index){ /* checks whether 2 quotes were detected */
        fprintf(stderr, "%s:%ld: two quotes are expected\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }
    
    for(; i < end_index; i++)
        if(f_info.cur_line_content[i] -= QUOTE_SYMBOL) {
            fprintf(stderr, "%s:%ld: too many quotes\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

    *dc += (end_index - i);  /* updates the data counter */

    return TRUE;
}

/* takes an entry instruction and validates it */
static bool validate_entry_instruction(file_info f_info, int i){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has bee detetced */
    label_format_error label_error; /* label error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* gets the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

            label_error = is_valid_label(expression); /* looks for errors */

            if(label_error != LBL_NONE){ /* checks whether the label is valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* it already encountered 1 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 1){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount == 0){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    return TRUE;
}

/* takes an entry instruction and validates it */
static bool validate_extern_instruction(file_info f_info, int i, symbol_table *table){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* amount of operands it encountered */
    bool comma_detected = FALSE; /* says whether a comma has bee detetced */
    label_format_error label_error; /* label error status */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

    /* getting the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

            label_error = is_valid_label(expression); /* looks for errors */

            if(label_error != LBL_NONE){ /* checks whether the label is valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* it already encountered 1 operands, now it will just keep counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* a comma has been detected */

            i++; /* skips the comma */
            i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        }

        args_amount += 1; /* just handled one operand */
    }

    if(comma_detected == TRUE){ /* unexpected comma at the end */
        fprintf(stderr, "%s:%ld: unexpected comma at the end\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount > 1){ /* this instruction has too many operands */
        fprintf(stderr, "%s:%ld: too many operands(more than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(args_amount == 0){ /* this instruction doesn't have enough operands */
        fprintf(stderr, "%s:%ld: not enough operands(less than 1)\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    if(exists(table, expression, EXTERNAL_SYMBOL))
        return TRUE;

    if(exists(table, expression, ANY_SYMBOL)) add_attributes(table, expression, EXTERNAL_SYMBOL);
    else add_symbol_item(table, expression, EXTERNAL_INSTRUCTION_ADDRESS, EXTERNAL_SYMBOL);

    return TRUE;
}
