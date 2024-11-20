/* this file contains functions for the second pass */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "second_pass.h"
#include "utils.h"

/* takes a line and skips its label(if exists) */
static int skip_label(file_info f_info);

/* takes an r instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_r_instruction_group_1(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic);
/* takes an r instruction(copying) and tries to assemble it */
static bool assemble_r_instruction_group_2(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic);
/* takes an r instruction and tries to assemble it */
static bool assemble_r_instruction(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic, int expected_args);
/* builds an r instruction via the given details */
static void build_r_instruction(memory_table *ic_memory_table, opcode op, int funct, int rs, int rt, int rd);

/* takes an i instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_i_instruction_group_1(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* takes an i instruction(jumping) and tries to assemble it */
static bool assemble_i_instruction_group_2(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, long *ic);
/* takes an i instruction(loading and saving in memory) and tries to assemble it */
static bool assemble_i_instruction_group_3(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* takes an i instruction(that doesn't have a label) and tries to assemble it */
static bool assemble_i_instruction_no_label(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* builds an i instruction via the given details */
static void build_i_instruction(memory_table *ic_memory_table, opcode op, int rs, int rt, long immed);

/* takes a jmp instruction and tries to assemble it */
static bool assemble_jmp_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* takes a la instruction and tries to assemble it */
static bool assemble_la_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* takes a call instruction and tries to assemble it */
static bool assemble_call_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* takes a j instruction(that only contains one label) and tries to assemble it */
static bool assemble_j_instruction_label_only(file_info f_info, int i, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, opcode op, long *ic);
/* takes a stop instruction and tries to assemble it */
static bool assemble_stop_instruction(memory_table *ic_memory_table, opcode op, long *ic);
/* takes details about a j instruction and converts it to machine code */
static void build_j_instruction(memory_table *ic_memory_table, opcode op, int reg, long address);

/* takes db instruction and tries to assemble it */
static bool assemble_db_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* takes dh instruction and tries to assemble it */
static bool assemble_dh_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* takes dw instruction and tries to assemble it */
static bool assemble_dw_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* takes info about a data instruction and tries to assemble it */
static bool assemble_num_creation_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc, int cells_amount);

/* this function takes asciz instruction and tries to assemble it */
static bool assemble_asciz_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);

/* takes an entry instruction and tries to assemble it */
static bool assemble_entry_instruction(file_info f_info, int i, symbol_table *sym_table);
/* takes an extern instruction and tries to assemble it */
static bool assemble_extern_instruction(file_info f_info, int i, symbol_table *sym_table);

/* takes a line and tries to assemble it */
bool assemble_command(file_info f_info, memory_table *ic_memory_table, memory_table *dc_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic, long *dc){
    char instruction[MAX_LINE_LENGTH];
    int i = 0; /* current index */
    instruction_info *instruct_info; /* information about the instruction */
    
    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    if(is_empty_line(f_info.cur_line_content, i)) return TRUE; /* checks whether the line should be processed */
    
    i = skip_label(f_info); /* skips the label(if exists) */
    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    i = get_label_until(f_info.cur_line_content, instruction, i, ','); /* gets the instruction's name */
    
    /* gets info about the instruction */
    instruct_info = get_instruction_info(instruction);
    
    /* checks the insruction's type */
    switch(instruct_info->id){
        case ADD: return assemble_r_instruction_group_1(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case SUB: return assemble_r_instruction_group_1(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case AND: return assemble_r_instruction_group_1(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case OR: return assemble_r_instruction_group_1(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case NOR: return assemble_r_instruction_group_1(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case MOVE: return assemble_r_instruction_group_2(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case MVHI: return assemble_r_instruction_group_2(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case MVLO: return assemble_r_instruction_group_2(f_info, i, instruct_info->opcode, instruct_info->funct, ic_memory_table, ic);
        case ADDI: return assemble_i_instruction_group_1(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case SUBI: return assemble_i_instruction_group_1(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case ANDI: return assemble_i_instruction_group_1(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case ORI: return assemble_i_instruction_group_1(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case NORI: return assemble_i_instruction_group_1(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case BNE: return assemble_i_instruction_group_2(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, ic);
        case BEQ: return assemble_i_instruction_group_2(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, ic);
        case BLT: return assemble_i_instruction_group_2(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, ic);
        case BGT: return assemble_i_instruction_group_2(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, ic);
        case LB: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case SB: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case LW: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case SW: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case LH: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case SH: return assemble_i_instruction_group_3(f_info, i, instruct_info->opcode, ic_memory_table, ic);
        case JMP: return assemble_jmp_instruction(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, extern_list, ic);
        case LA: return assemble_la_instruction(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, extern_list, ic);
        case CALL: return assemble_call_instruction(f_info, i, instruct_info->opcode, ic_memory_table, sym_table, extern_list, ic);
        case STOP: return assemble_stop_instruction(ic_memory_table, instruct_info->opcode, ic);
        case DB: return assemble_db_instruction(f_info, i, dc_memory_table, dc);
        case DH: return assemble_dh_instruction(f_info, i, dc_memory_table, dc);
        case DW: return assemble_dw_instruction(f_info, i, dc_memory_table, dc);
        case ASCIZ: return assemble_asciz_instruction(f_info, i, dc_memory_table, dc);
        case ENTRY: return assemble_entry_instruction(f_info, i, sym_table);
        case EXTERN: return assemble_extern_instruction(f_info, i, sym_table);
        default: return FALSE;
    };
}

/* takes a line and skips its label(if exists) */
static int skip_label(file_info f_info){
    char label[MAX_LINE_LENGTH];
    int i = 0;

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    i = get_label_until(f_info.cur_line_content, label, i, ':'); /* gets all the chars until it encounters a ':' */

    /* checks whether the insruction has a label */
    if(i == strlen(f_info.cur_line_content) || f_info.cur_line_content[i] != ':')
        return 0;

    if(!is_valid_label(label)) /* checks whether the label is valid */
        return 0;

    return i + 1; /* the label is valid, returns the index of the char that comes just after the label */
}

/* takes an r instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_r_instruction_group_1(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic){
    return assemble_r_instruction(f_info, i, op, f, ic_memory_table, ic, 3); /* this instruction has 3 operands */
}

/* takes an r instruction(copying) and tries to assemble it */
static bool assemble_r_instruction_group_2(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic){
    return assemble_r_instruction(f_info, i, op, f, ic_memory_table, ic, 2); /* this instruction has 2 operands */
}

/* takes an r instruction and tries to assemble it */
static bool assemble_r_instruction(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic, int expected_args){
    int *operands = (int*)malloc(sizeof(int) * expected_args);
    int args_handled = 0; /* amount of operands it encountered */
    char expression[MAX_LINE_LENGTH]; /* current operand as a string */

    if(operands == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* gets the operands one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        operands[args_handled] = get_register(expression); /* gets the int value of the current operand */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i++; /* skips the ','(technical issue) */
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

        args_handled++; /* just handled one operand */
    }

    if(expected_args == 2) build_r_instruction(ic_memory_table, op, f, operands[0], 0, operands[1]); /* converts the r instruction(copying) to machine code */
    else if(expected_args == 3) build_r_instruction(ic_memory_table, op, f, operands[0], operands[1], operands[2]); /* converts the r instruction(arithmetic and logic) to machine code */

    /* frees variables */
    free(operands);

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* builds an r instruction via the given details */
static void build_r_instruction(memory_table *ic_memory_table, opcode op, int funct, int rs, int rt, int rd){
    long value = 0; /* represents the instruction machine code */

    /* converts the given details to machine code */
    value |= op; 
    value = ((value << 5) | (rs & 31)); /* saving 5 LSB */
    value = ((value << 5) | (rt & 31)); /* saving 5 LSB */
    value = ((value << 5) | (rd & 31)); /* saving 5 LSB */
    value = ((value << 5) | (funct & 31)); /* saving 5 LSB */
    value <<= 6; /* shifting 6 bits(task requirements) */

    add_memory_elements(ic_memory_table, 4, value); /* converts the machine code into cells */
}

/* takes an i instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_i_instruction_group_1(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic){
    return assemble_i_instruction_no_label(f_info, i, op, ic_memory_table, ic); /* i instruction(arithmetic and logic) */
}

/* takes an i instruction(jumping) and tries to assemble it */
static bool assemble_i_instruction_group_2(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, long *ic){
    char label[MAX_LINE_LENGTH]; /* the label in the insruction(third operand) */
    int *registers = (int*)malloc(sizeof(int) * 2); /* the registers of the instruction */
    long immed; /* the distance between the current memory location to the label location in memory */
    int args_handled = 0; /* amount of operands it encountered */

    if(registers == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* gets the operands one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i = get_label_until(f_info.cur_line_content, label, i, ','); /* gets all the chars until it encounters a ',' */

        if(args_handled == 2){ /* checks whether the current operand is a label */
            if (!exists(sym_table, label, ANY_SYMBOL)) { /* the label does not exist */
                fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
                return FALSE;
            }

			/* checks whether the label is external(which is not allowd) */
            if(exists(sym_table, label, EXTERNAL_SYMBOL)){
                fprintf(stderr, "%s:%ld: the label \"%s\" can't be external\n", f_info.name, f_info.cur_line_number, label);
                return FALSE;
            }
        }

        if(args_handled < 2) registers[args_handled] = get_register(label); /* checks whether the current operand is a register */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i++; /* skips the ','(technical issue) */
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */

        args_handled++; /* just handled one operand */
    }

    immed = get_address_of(sym_table, label) - *ic; /* calculates the distance between the current memory location to the label location in memory */

    build_i_instruction(ic_memory_table, op, registers[0], registers[1], immed); /* converts the instruction to machine code */

    /* freeing allocated variables */
    free(registers);

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* takes an i instruction(loading and saving in memory) and tries to assemble it */
static bool assemble_i_instruction_group_3(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic){
    return assemble_i_instruction_no_label(f_info, i, op, ic_memory_table, ic); /* i instruction(loading and saving in memory) */
}

/* takes an i instruction(that doesn't have a label) and tries to assemble it */
static bool assemble_i_instruction_no_label(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic) {
    int *registers = (int *) malloc(sizeof(int) * 2); /* the registers of the instruction */
    int registers_handled = 0; /* amount of registers it encountered */
    int args_handled = 0; /* amount of operands it encountered */
    long immed; /* the literal of the instruction(second operand) */
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */

    if (registers == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* gets the operands one by one */
    while (!end_of_line(f_info.cur_line_content, i)) {
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* gets all the chars until it encounters a ',' */

        if (args_handled == 1) immed = get_num(expression);
        else {
            registers[registers_handled] = get_register(expression); /* gets the int value of the current operand */
            registers_handled++; /* just handled one register */
        }

        args_handled++; /* just handled one operand */

        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i++; /* skips the ','(technical issue) */
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    }

    build_i_instruction(ic_memory_table, op, registers[0], registers[1], immed); /* converts the instruction to machine code */

    /* frees variables */
    free(registers); 

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* builds an i instruction via the given details */
static void build_i_instruction(memory_table *ic_memory_table, opcode op, int rs, int rt, long immed){
    long value = 0; /* represents the instruction machine code */

    value |= op;
    value  = ((value << 5) | (rs & 31)); /* saving 5 LSB */
    value  = ((value << 5) | (rt & 31)); /* saving 5 LSB */
    value  = ((value << 16) | (immed & 65535)); /* saving 16 LSB */

    add_memory_elements(ic_memory_table, 4, value); /* converts the machine code into cells */
}

/* takes a jmp instruction and tries to assemble it */
static bool assemble_jmp_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic) {
    char label[MAX_LINE_LENGTH]; /* the instruction's label (if exists) */
    int reg = 0; /* says whether the instruction's operand is a register */
    int address; /* instruction's label's address */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars from i until the end of line */

    if (label[0] == '$') { /* checks whether the operand is a register */
        reg = 1; /* the operand is a register */
        address = get_register(label); /* the address is set to be the value of the register */
    }
    else {
        if (!exists(sym_table, label, ANY_SYMBOL)) { /* checks whether the label exists */
            fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
            return FALSE;
        }
        address = get_address_of(sym_table, label); /* gets the address of the label */
    }

    /* checks the label is external, if so then the label should be saved for later processing */
    if (exists(sym_table, label, EXTERNAL_SYMBOL))
        add_extern_element(extern_list, label, *ic); /* saving the location */

    build_j_instruction(ic_memory_table, op, reg, address); /* converts the instruction to machine code */

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes a la instruction and tries to assemble it */
static bool assemble_la_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic){
    return assemble_j_instruction_label_only(f_info, i, ic_memory_table, sym_table, extern_list, op, ic); /* la instruction */
}

/* takes a call instruction and tries to assemble it */
static bool assemble_call_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic){
    return assemble_j_instruction_label_only(f_info, i, ic_memory_table, sym_table, extern_list, op, ic); /* call instruction */
}

/* takes a j instruction(that only contains one label) and tries to assemble it */
static bool assemble_j_instruction_label_only(file_info f_info, int i, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, opcode op, long *ic){
    char label[MAX_LINE_LENGTH]; /* instruction's label(if exists) */

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    get_label(f_info.cur_line_content, label, i); /* reads all the chars between i and the line's end */

    if (!exists(sym_table, label, ANY_SYMBOL)) { /* checks whether the label exists */
        fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    /* checks the label is external, if so then the label should be saved for later processing */
    if (exists(sym_table, label, EXTERNAL_SYMBOL))
        add_extern_element(extern_list, label, *ic); /* saving the location */

    build_j_instruction(ic_memory_table, op, 0, get_address_of(sym_table, label)); /* converts the instruction to machine code */

    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes a stop instruction and tries to assemble it */
static bool assemble_stop_instruction(memory_table *ic_memory_table, opcode op, long *ic){
    build_j_instruction(ic_memory_table, op, 0, 0);
    *ic += 4; /* an instruction of 4 cells */
    return TRUE;
}

/* takes details about a j instruction and converts it to machine code */
static void build_j_instruction(memory_table *ic_memory_table, opcode op, int reg, long address){
    long value = 0; /* represents the instruction machine code */

    value |= op;
    value  = ((value << 1) | (reg & 1)); /* saving 1 LSB */
    value  = ((value << 25) | (address & 33554431)); /* saving 25 LSB */

    add_memory_elements(ic_memory_table, 4, value); /* converts the machine code into cells */
}

/* takes db instruction and tries to assemble it */
static bool assemble_db_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 1); /* db instruction */
}

/* takes dh instruction and tries to assemble it */
static bool assemble_dh_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 2); /* dh instruction */
}

/* takes dw instruction and tries to assemble it */
static bool assemble_dw_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 4); /* dw instruction */
}

/* takes info about a data instruction and tries to assemble it */
static bool assemble_num_creation_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc, int cells_amount){
    char num[MAX_LINE_LENGTH]; /* buffer contaning the number as a string */

    /* gets the numbers one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
        i = get_label_until(f_info.cur_line_content, num, i, ','); /* gets all the chars until it encounters a ',' */
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i++;

        add_memory_elements(dc_memory_table, cells_amount, get_num(num)); /* adds the number to the memory */

        *dc += cells_amount; /* the number's size is ("cell_amnout" cells) */
    }

    return TRUE;
}

/* takes an asciz instruction and tries to assemble it */
static bool assemble_asciz_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    int end_index = strlen(f_info.cur_line_content) - 1; /* end of the line */
    int comment_index = end_index; /* acutal end of line */

    /* looks for the comment sign */
    while(comment_index >= 0 && f_info.cur_line_content[comment_index] != COMMENT_SYMBOL)
        comment_index--;
    
    if(comment_index >= 0) 
        end_index = comment_index - 1;    
        
    /* looks for the quote sign */
    while(end_index >= 0 && isspace(f_info.cur_line_content[end_index]))
        end_index--;

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    i++; /* skips the quote */

    /* reads the content between the quotes */
    for(; i < end_index; i++){
        add_memory_elements(dc_memory_table, 1, f_info.cur_line_content[i]); /* adds the char to the memory */
        *dc += 1; /* a char takes one cell in memory */
    }
    add_memory_elements(dc_memory_table, 1, '\0');
    *dc += 1; /* a char takes one cell in memory */

    return TRUE;
}

/* takes an entry instruction and tries to assemble it */
static bool assemble_entry_instruction(file_info f_info, int i, symbol_table *sym_table){
    char label[MAX_LINE_LENGTH];

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars between i and the line's end */

    if(!exists(sym_table, label, (ANY_SYMBOL & ~EXTERNAL_SYMBOL))){ /* checks whether the label was created in the file */
        fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    add_attributes(sym_table, label, ENTRY_SYMBOL); /* marks the label as an entry */

    return TRUE;
}

/* takes an extern instruction and tries to assemble it */
static bool assemble_extern_instruction(file_info f_info, int i, symbol_table *sym_table) {
    char label[MAX_LINE_LENGTH];

    i = skip_spaces(f_info.cur_line_content, i); /* skips white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars between i and the line's end */


    /* checks the label is not external(if a label is external then it can only be external) */
    if (exists(sym_table, label, ~EXTERNAL_SYMBOL)) {
        fprintf(stderr, "%s:%ld: the label \"%s\" was created in the current file and can't be external\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    return TRUE;
}
