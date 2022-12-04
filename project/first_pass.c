/* this file contains function for the first pass */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "code.h"
#include "utils.h"
#include "first_pass.h"

/* this function takes a line and finds a label, if the label exists then if it's valid it adds it to the symbol table */
static int find_and_validate_label(file_info f_info, int i, char *label, symbol_table *table);

/* this function takes r instruction(arithmetic and logic) and validates it */
static bool validate_r_instruction_group_1(file_info f_info, int i, long *ic);
/* this function takes r instruction(copying) and validates it */
static bool validate_r_instruction_group_2(file_info f_info, int i, long *ic);
/* this function takes r instruction(any, knows what the type of the instruction according to amount of args) and validates it */
static bool validate_r_command(file_info f_info, int i, long *ic, int args_amount_expected);

/* this function takes i instruction(arithmetic and logic) and validates it */
static bool validate_i_instruction_group_1(file_info f_info, int i, long *ic);
/* this function takes i instruction(jumping) and validates it */
static bool validate_i_instruction_group_2(file_info f_info, int i, long *ic);
/* this function takes i instruction(loading and saving in memory) and validates it */
static bool validate_i_instruction_group_3(file_info f_info, int i, long *ic);
/* this function takes i instruction(that does not have label) and validates it */
static bool validate_i_instruction_no_label(file_info f_info, int i, long *ic);

/* this function takes jmp instruction and validates it */
static bool validate_jmp_instruction(file_info f_info, int i, long *ic);
/* this function takes la instruction and validates it */
static bool validate_la_instruction(file_info f_info, int i, long *ic);
/* this function takes call instruction and validates it */
static bool validate_call_instruction(file_info f_info, int i, long *ic);
/* this function takes stop instruction and validates it */
static bool validate_stop_instruction(file_info f_info, int i, long *ic);
/* this function takes stop instruction and validates it */
static bool validate_j_instruction_label_only(file_info f_info, int i, long *ic);

/* this function takes db instruction and validates it */
static bool validate_db_instruction(file_info f_info, int i, long *dc);
/* this function takes dh instruction and validates it */
static bool validate_dh_instruction(file_info f_info, int i, long *dc);
/* this function takes dw instruction and validates it */
static bool validate_dw_instruction(file_info f_info, int i, long *dc);
/* this function takes a num creation instruction(know what the type of the instruction according to the amount of cells it takes in memory and validates it */
static bool validate_num_creation_instruction(file_info f_info, int i, long *dc, int cells_amount);

/* this function takes asciz instruction and validates it */
static bool validate_asciz_instruction(file_info f_info, int i, long *dc);

/* this function takes entry instruction and validates it */
static bool validate_entry_instruction(file_info f_info, int i);
/* this function takes extern instruction and validates it */
static bool validate_extern_instruction(file_info f_info, int i, symbol_table *table);

/* this function takes a line and validates it */
bool validate_command(file_info f_info, symbol_table *table, long *ic, long *dc){
    char label[MAX_LINE_LENGTH] /* the label in the beginning of the instruction */, instruction[MAX_LINE_LENGTH]; /* the instruction */
    int i = 0; /* index for given the line */
    bool label_found = FALSE; /* says if we saw a label or not */
    instruction_info *instruct_info; /* information about the instruction */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    if(is_empty_line_or_comment_line(f_info.cur_line_content, i)) return TRUE; /* means the current line is empty or a comment */

    i = find_and_validate_label(f_info, i, label, table); /* moving the index to after the label(if we found a label) */
    if(i == -1) return FALSE; /* means we found a label but i'ts not a valid label */

    if(i != 0) /* means we found a valid label */
        label_found = TRUE;

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    i = get_label_until(f_info.cur_line_content, instruction, i, ','); /* getting the name of the instruction */

	/* getting info about the instruction */
	instruct_info = get_instruction_info(instruction); 

	/* checking which instruction the given instruction is */
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
				if(label_found == TRUE){ /* label before entry instruction is not allowd */
					fprintf(stderr, "%s:%ld: the label \"%s\" can't be defined in entry instruction\n", f_info.name, f_info.cur_line_number, label);
					return FALSE;
				}
				return validate_entry_instruction(f_info, i);
			case EXTERN: 
				if(label_found == TRUE){ /* label before extern instruction is not allowd */
					fprintf(stderr, "%s:%ld: the label \"%s\" can't be defined in extern instruction\n", f_info.name, f_info.cur_line_number,label);
					return FALSE;
				}
				return validate_extern_instruction(f_info, i, table);
			default: break;
        };     
    }  
	
    /* the lines below find out why the instruction isn't correct */
    if(f_info.cur_line_content[i] == ','){
	if(strcmp(instruction, "") == 0) fprintf(stderr, "%s:%ld: unexpected comma before instruction\n", f_info.name, f_info.cur_line_number);
	    else fprintf(stderr, "%s:%ld: \"%s\" is not a defined instruction\n", f_info.name, f_info.cur_line_number, instruction);
		return FALSE;
	}
	
    if(strcmp(instruction, "") == 0) fprintf(stderr, "%s:%ld: no instruction after label\n", f_info.name, f_info.cur_line_number);
    else fprintf(stderr, "%s:%ld: \"%s\" is not a defined instruction\n", f_info.name, f_info.cur_line_number, instruction);

    return FALSE; /* validation failed */
}

/* this function takes a line and finds a label, if the label exists then if it's valid it adds it to the symbol table */
static int find_and_validate_label(file_info f_info, int i, char *label, symbol_table *table){
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = get_label_until(f_info.cur_line_content, label, i, ':'); /* getting all the chars until we see a ':' */

    if(f_info.cur_line_content[i] != ':') /* means it's not a label */
        return 0;

    if((i + 1 < strlen(f_info.cur_line_content)) && !isspace(f_info.cur_line_content[i + 1])) /* a space should appear after we see a ':' if not then it's not valid */
        return 0;

    if(strlen(label) == 0){ /* label is empty */
        fprintf(stderr, "%s:%ld: \":\" is not a valid label\n", f_info.name, f_info.cur_line_number);
        return -1;
    }

    label_error = is_valid_label(label); /* looking for errors of the label */

    if(label_error != LBL_NONE){ /* means the label is not valid */
        if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, label);
        else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, label);
        else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, label);

        return -1;
    }

    /* checking if the name of the label is a keyword */
    if(get_instruction_info(label) != NULL) {
        fprintf(stderr, "%s:%ld: the label \"%s\" can't have the same name as an instruction\n", f_info.name, f_info.cur_line_number, label);
        return -1;
    }

    if(exists(table, label, CODE_SYMBOL | DATA_SYMBOL)){ /* means the label has already been defined(only one definition is allowed) */
        fprintf(stderr, "%s:%ld: the label \"%s\" already exists\n", f_info.name, f_info.cur_line_number, label);
        return -1;
    }

    return i + 1; /* label is valid, moving index to what comes after the label */
}

/* this function takes r instruction(arithmetic and logic) and validates it */
static bool validate_r_instruction_group_1(file_info f_info, int i, long *ic){
    return validate_r_command(f_info, i, ic, 3); /* this instruction has 3 operands */
}

/* this function takes r instruction(copying) and validates it */
static bool validate_r_instruction_group_2(file_info f_info, int i, long *ic){
    return validate_r_command(f_info, i, ic, 2); /* this instruction has 2 operands */
}

/* this function takes r instruction(any, knows what the type of the instruction according to amount of args) and validates it */
static bool validate_r_command(file_info f_info, int i, long *ic, int args_amount_expected) {
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    register_format_error register_error; /* says if the register is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    if (f_info.cur_line_content[i] == ',') { /* unexpected comma before operand detected */
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operands one by one and validates them, in addition it validates the syntax, if it sees more operands then it counts them */
    while (!end_of_line(f_info.cur_line_content, i)) {
        comma_detected = FALSE; /* starting again, validating a new operand */

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

	/* first 3 operands should be registers, the others are just for amount handling */
        if (args_amount < args_amount_expected) {

            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looking for errors of the register */
            if (register_error != REG_NONE) { /* means the register is not valid */
                if (register_error == REG_DOLLAR_SIGN_NOT_FOUND)
                    fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(\"$\" not found)\n", f_info.name,
                            f_info.cur_line_number, expression);
                else if (register_error == REG_NOTHING_AFTER_DOLLAR_SIGN)
                    fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name,
                            f_info.cur_line_number, expression);
                else if (register_error == REG_NOT_NUMBER)
                    fprintf(stderr,
                            "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n",
                            f_info.name, f_info.cur_line_number, expression);
                else if (register_error == REG_NOT_IN_RANGE)
                    fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name,
                            f_info.cur_line_number, expression);

                return FALSE;
            }
        } else /* we already saw 3 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if (!end_of_line(f_info.cur_line_content, i)) {
            if (f_info.cur_line_content[i] != ',') { /* just saw unexpected comma */
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */
}

/* this function takes i instruction(arithmetic and logic) and validates it */
static bool validate_i_instruction_group_1(file_info f_info, int i, long *ic){
    return validate_i_instruction_no_label(f_info, i, ic); /* i instruction(arithmetic and logic) */
}

/* this function takes i instruction(jumping) and validates it */
static bool validate_i_instruction_group_2(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    register_format_error register_error; /* says if the register is invalid and why it's invalid */
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operands one by one and validates them, in addition it validates the syntax, if it sees more operands then it counts them */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }
		
        if(args_amount == 0 || args_amount == 1) { /* first 2 operands should be registers */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looking for errors of the register */
            if(register_error != REG_NONE){ /* means the register is not valid */
                if(register_error == REG_DOLLAR_SIGN_NOT_FOUND) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(\"$\" not found)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_NUMBER) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                else if(register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }		
        else if(args_amount == 2) { /* third operand should be a label, the others are just for amount handling */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            label_error = is_valid_label(expression);

            if(label_error != LBL_NONE){ /* means the label is not valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* we already saw 3 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */
}

/* this function takes i instruction(loading and saving in memory) and validates it */
static bool validate_i_instruction_group_3(file_info f_info, int i, long *ic){
    return validate_i_instruction_no_label(f_info, i, ic); /* i instruction(loading and saving in memory) */
}

/* this function takes i instruction(that does not have label) and validates it */
static bool validate_i_instruction_no_label(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    register_format_error register_error; /* says if the register is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operands one by one and validates them, in addition it validates the syntax, if it sees more operands then it counts them */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0 || args_amount == 2) { /* first and third operands should be registers */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looking for errors of the register */
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
        else /* we already saw 3 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */
}

static bool validate_jmp_instruction(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    register_format_error register_error; /* says if the register is invalid and why it's invalid */
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operand and validates it, in addition it validates the syntax, if it sees more operands then it counts them */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label or a registers, the others are just for amount handling */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            register_error = is_valid_register(expression); /* looking for errors of the register */

            if (register_error != REG_NONE) { /* means the register is not valid */
                if (register_error != REG_DOLLAR_SIGN_NOT_FOUND) {
                    if (register_error == REG_NOTHING_AFTER_DOLLAR_SIGN) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(no number after \"$\")\n", f_info.name, f_info.cur_line_number, expression);
                    else if (register_error == REG_NOT_IN_RANGE) fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(number not between 0 and 31)\n", f_info.name, f_info.cur_line_number, expression);
                    else fprintf(stderr, "%s:%ld: \"%s\" is not a valid register(what comes after \"$\" is not an integer)\n", f_info.name, f_info.cur_line_number, expression);
                    return FALSE;
                }

                label_error = is_valid_label(expression);

                if(label_error != LBL_NONE){ /* means the label is not valid */
                    if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                    else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                    else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                    return FALSE;
                }
            }
        }
        else /* we already saw 1 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */
}

static bool validate_la_instruction(file_info f_info, int i, long *ic){
    return validate_j_instruction_label_only(f_info, i, ic);
}

static bool validate_call_instruction(file_info f_info, int i, long *ic){
    return validate_j_instruction_label_only(f_info, i, ic);
}

static bool validate_stop_instruction(file_info f_info, int i, long *ic) {
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }
	
    /* validates the instruction, if it sees more operands then it counts them */
    while (!end_of_line(f_info.cur_line_content, i)) {
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if (!end_of_line(f_info.cur_line_content, i)) {
            if (f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */
}

static bool validate_j_instruction_label_only(file_info f_info, int i, long *ic){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            label_error = is_valid_label(expression); /* looking for errors of the label */

            if(label_error != LBL_NONE){ /* means the label is not valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* we already saw 1 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    return TRUE; /* validation succeded */ 
}

static bool validate_db_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 1);
}

static bool validate_dh_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 2);
}

static bool validate_dw_instruction(file_info f_info, int i, long *dc){
    return validate_num_creation_instruction(f_info, i, dc, 4);
}

static bool validate_num_creation_instruction(file_info f_info, int i, long *dc, int cells_amount){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */

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

        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        if(!is_int(expression, 0)){
            fprintf(stderr, "%s:%ld: \"%s\" is not a valid integer\n", f_info.name, f_info.cur_line_number, expression);
            return FALSE;
        }

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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

    return TRUE; /* validation succeded */
}

static bool validate_asciz_instruction(file_info f_info, int i, long *dc){
    int end_index = strlen(f_info.cur_line_content) - 1;

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') { 
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    while(end_index >= 0){
        if(!isspace(f_info.cur_line_content[end_index])){
            if(f_info.cur_line_content[end_index] != '\"') {
                fprintf(stderr, "%s:%ld: ending quote expected at the end\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            break;
        }
        end_index--;
    }

    while(!end_of_line(f_info.cur_line_content, i)){
        if(!isspace(f_info.cur_line_content[i])){
            if(f_info.cur_line_content[i] != '\"') {
                fprintf(stderr, "%s:%ld: opening quote expected at the start\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            break;
        }
        i++;
    }

	if(i == end_index){
        fprintf(stderr, "%s:%ld: two quotes are expected\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    *dc += (end_index - i);  /* amount of cells is the amount of chars between the quotes plus one cell for the string terminator */

    return TRUE; /* validation succeded */
}

static bool validate_entry_instruction(file_info f_info, int i){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            label_error = is_valid_label(expression); /* looking for errors of the label */

            if(label_error != LBL_NONE){ /* means the label is not valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* we already saw 1 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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

    return TRUE; /* validation succeded */
}

static bool validate_extern_instruction(file_info f_info, int i, symbol_table *table){
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */
    int args_amount = 0; /* how many operands we saw */
    bool comma_detected = FALSE; /* says of we saw a comma */
    label_format_error label_error; /* says if the label is invalid and why it's invalid */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

    /* unexpected comma before operand detected */
    if (f_info.cur_line_content[i] == ',') {
        fprintf(stderr, "%s:%ld: unexpected comma before operand\n", f_info.name, f_info.cur_line_number);
        return FALSE;
    }

    /* getting the operand and validates it, in addition it validates the syntax */
    while(!end_of_line(f_info.cur_line_content, i)){
        comma_detected = FALSE;

        if (f_info.cur_line_content[i] == ',') { /* atleast 2 commas in a row(not allowed) */
            fprintf(stderr, "%s:%ld: too many commas\n", f_info.name, f_info.cur_line_number);
            return FALSE;
        }

        if(args_amount == 0) { /* first operand should be a label */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

            label_error = is_valid_label(expression); /* looking for errors of the label */

            if(label_error != LBL_NONE){ /* means the label is not valid */
                if(label_error == LBL_TOO_LONG) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(contains more than 31 chars)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_FIRST_CHAR_NOT_ALPHA) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(first char is not alpha)\n", f_info.name, f_info.cur_line_number, expression);
                else if(label_error == LBL_CHAR_NOT_ALPHA_OR_DIGIT) fprintf(stderr, "%s:%ld: \"%s\" is not a valid label(at least one char is not alpha or digit)\n", f_info.name, f_info.cur_line_number, expression);

                return FALSE;
            }
        }
        else /* we already saw 1 operands, now we are jsut counting */
            i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        if(!end_of_line(f_info.cur_line_content, i)){
            if(f_info.cur_line_content[i] != ',') {
                fprintf(stderr, "%s:%ld: comma expected\n", f_info.name, f_info.cur_line_number);
                return FALSE;
            }
            comma_detected = TRUE; /* just saw a comma */

            i++; /* ignoring the comma in order to keep processing */
            i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
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
    else add_symbol_item(table, expression, 0, EXTERNAL_SYMBOL);

    return TRUE; /* validation succeded */
}

