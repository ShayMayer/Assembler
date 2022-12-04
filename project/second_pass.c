/* this file contains function for the second pass */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "second_pass.h"
#include "utils.h"

/* this function takes a line and skips its label(if exists) */
static int skip_label(file_info f_info);

/* this function takes r instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_r_instruction_group_1(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic);
/* this function takes r instruction(copying) and tries to assemble it */
static bool assemble_r_instruction_group_2(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic);
/* this function takes r instruction(any, knows what the type of the instruction according to amount of args) and tries to assemble it */
static bool assemble_r_instruction(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic, int expected_args);
/* this function takes details about r instruction and convert it to machine code */
static void build_r_instruction(memory_table *ic_memory_table, opcode op, int funct, int rs, int rt, int rd);

/* this function takes i instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_i_instruction_group_1(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* this function takes i instruction(jumping) and tries to assemble it */
static bool assemble_i_instruction_group_2(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, long *ic);
/* this function takes i instruction(loading and saving in memory) and tries to assemble it */
static bool assemble_i_instruction_group_3(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* this function takes i instruction(that does not have label) and tries to assemble it */
static bool assemble_i_instruction_no_label(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic);
/* this function takes details about i instruction and convert it to machine code */
static void build_i_instruction(memory_table *ic_memory_table, opcode op, int rs, int rt, long immed);

/* this function takes jmp instruction and tries to assemble it */
static bool assemble_jmp_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* this function takes la instruction and tries to assemble it */
static bool assemble_la_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* this function takes call instruction and tries to assemble it */
static bool assemble_call_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic);
/* this function takes j instruction(that accepts label only) and tries to assemble it */
static bool assemble_j_instruction_label_only(file_info f_info, int i, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, opcode op, long *ic);
/* this function takes stop instruction and tries to assemble it */
static bool assemble_stop_instruction(memory_table *ic_memory_table, opcode op, long *ic);
/* this function takes details about j instruction and convert it to machine code */
static void build_j_instruction(memory_table *ic_memory_table, opcode op, int reg, long address);

/* this function takes db instruction and tries to assemble it */
static bool assemble_db_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* this function takes dh instruction and tries to assemble it */
static bool assemble_dh_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* this function takes dw instruction and tries to assemble it */
static bool assemble_dw_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);
/* this function takes a num creation instruction(know what the type of the instruction according to the amount of cells it takes in memory and tries to assemble it */
static bool assemble_num_creation_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc, int cells_amount);

/* this function takes asciz instruction and tries to assemble it */
static bool assemble_asciz_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc);

/* this function takes entry instruction and tries to assemble it */
static bool assemble_entry_instruction(file_info f_info, int i, symbol_table *sym_table);
/* this function takes extern instruction and tries to assemble it */
static bool assemble_extern_instruction(file_info f_info, int i, symbol_table *sym_table);

/* this function takes a line and tries to assemble it */
bool assemble_command(file_info f_info, memory_table *ic_memory_table, memory_table *dc_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic, long *dc){
    char instruction[MAX_LINE_LENGTH]; /* the instruction */
    int i = 0;
    instruction_info *instruct_info; /* information about the instruction */
    
    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    if(is_empty_line_or_comment_line(f_info.cur_line_content, i)) return TRUE; /* means the current line is empty or a comment */
    
    i = skip_label(f_info); /* skipping label(if exists) */
    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    i = get_label_until(f_info.cur_line_content, instruction, i, ','); /* getting the nmae of the instruction */
    
    /* getting info about the instruction */
    instruct_info = get_instruction_info(instruction);
    
    /* checking which instruction the given instruction is */
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

/* this function takes a line and skips its label(if exists) */
static int skip_label(file_info f_info){
    char label[MAX_LINE_LENGTH]; /* the label in the beginning of the instruction */
    int i = 0;

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    i = get_label_until(f_info.cur_line_content, label, i, ':'); /* getting all the chars until we see a ':' */

    if(i == strlen(f_info.cur_line_content) || f_info.cur_line_content[i] != ':') /* means that what we read is not a label */
        return 0;

    if(!is_valid_label(label)) /* means that what we read is not a label */
        return 0;

    return i + 1; /* means what we read is a valid label, now we should read what comes after the label(that's why we return i+1) */
}

/* this function takes r instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_r_instruction_group_1(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic){
    return assemble_r_instruction(f_info, i, op, f, ic_memory_table, ic, 3); /* this instruction has 3 operands */
}

/* this function takes r instruction(copying) and tries to assemble it */
static bool assemble_r_instruction_group_2(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic){
    return assemble_r_instruction(f_info, i, op, f, ic_memory_table, ic, 2); /* this instruction has 2 operands */
}

/* this function takes r instruction(any, knows what the type of the instruction according to amount of args) and tries to assemble it */
static bool assemble_r_instruction(file_info f_info, int i, opcode op, funct f, memory_table *ic_memory_table, long *ic, int expected_args){
    int *operands = (int*)malloc(sizeof(int) * expected_args); /* the operands of the instruction */
    int args_handled = 0; /* how many operands we saw */
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */

    /* checking if the malloc function worked */
    if(operands == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* getting the operands one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        operands[args_handled] = get_register(expression); /* getting the actual value from the operand represented as a string */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i++; /* skipping the ',' char in order to keep getting the other operands values */
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

        args_handled++; /* just handled one operand */
    }

    if(expected_args == 2) build_r_instruction(ic_memory_table, op, f, operands[0], 0, operands[1]); /* converting the r instruction(copying) to machine code */
    else if(expected_args == 3) build_r_instruction(ic_memory_table, op, f, operands[0], operands[1], operands[2]); /* converting the r instruction(arithmetic and logic) to machine code */

    /* freeing allocated variables */
    free(operands);

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes details about r instruction and convert it to machine code */
static void build_r_instruction(memory_table *ic_memory_table, opcode op, int funct, int rs, int rt, int rd){
    long value = 0; /* represents the instruction machine code */

    /* in the lines bellow we convert the instruction to machine code(stores the machine code on a variable until final conversion) */
    value |= op; 
    value = ((value << 5) | (rs & 31)); /* saving 5 LSB */
    value = ((value << 5) | (rt & 31)); /* saving 5 LSB */
    value = ((value << 5) | (rd & 31)); /* saving 5 LSB */
    value = ((value << 5) | (funct & 31)); /* saving 5 LSB */
    value <<= 6; /* shifting 6 bits(task requirements) */

    add_memory_elements(ic_memory_table, 4, value); /* converting the machine code into cells */
}

/* this function takes i instruction(arithmetic and logic) and tries to assemble it */
static bool assemble_i_instruction_group_1(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic){
    return assemble_i_instruction_no_label(f_info, i, op, ic_memory_table, ic); /* i instruction(arithmetic and logic) */
}

/* this function takes i instruction(jumping) and tries to assemble it */
static bool assemble_i_instruction_group_2(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, long *ic){
    char label[MAX_LINE_LENGTH]; /* the label in the insruction(third operand) */
    int *registers = (int*)malloc(sizeof(int) * 2); /* the registers of the instruction */
    long immed; /* the distance between the current memory location to the label location in memory */
    int args_handled = 0; /* how many operands we saw */

    /* checking if the malloc function worked */
    if(registers == NULL){
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* getting the operands one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i = get_label_until(f_info.cur_line_content, label, i, ','); /* getting all the chars until we see a ',' */

        if(args_handled == 2){ /* means the current operand should be a label */
            if (!exists(sym_table, label, ANY_SYMBOL)) { /* label does not exist */
                fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
                return FALSE;
            }

            if(exists(sym_table, label, EXTERNAL_SYMBOL)){ /* label is external(not allowd in this instruction) */
                fprintf(stderr, "%s:%ld: the label \"%s\" can't be external\n", f_info.name, f_info.cur_line_number, label);
                return FALSE;
            }
        }

        if(args_handled < 2) registers[args_handled] = get_register(label); /* means the current operand is a register */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i++; /* skipping the ',' char in order to keep getting the other operands values */
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */

        args_handled++; /* just handled one operand */
    }

    immed = get_address_of(sym_table, label) - *ic; /* calculating the distance between the current memory location to the label location in memory */

    build_i_instruction(ic_memory_table, op, registers[0], registers[1], immed); /* converting the instruction to machine code */

    /* freeing allocated variables */
    free(registers);

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes i instruction(loading and saving in memory) and tries to assemble it */
static bool assemble_i_instruction_group_3(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic){
    return assemble_i_instruction_no_label(f_info, i, op, ic_memory_table, ic); /* i instruction(loading and saving in memory) */
}

/* this function takes i instruction(that does not have label) and tries to assemble it */
static bool assemble_i_instruction_no_label(file_info f_info, int i, opcode op, memory_table *ic_memory_table, long *ic) {
    int *registers = (int *) malloc(sizeof(int) * 2); /* the registers of the instruction */
    int registers_handled = 0; /* how many registers we saw */
    int args_handled = 0; /* how many operands we saw */
    long immed; /* the literal in the instruction(second operand) */
    char expression[MAX_LINE_LENGTH]; /* buffer contaning operand as a string */

    /* checking if the malloc function worked */
    if (registers == NULL) {
        fprintf(stderr, "c language error: malloc failed");
        exit(1);
    }

    /* getting the operands one by one */
    while (!end_of_line(f_info.cur_line_content, i)) {
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i = get_label_until(f_info.cur_line_content, expression, i, ','); /* getting all the chars until we see a ',' */

        if (args_handled == 1) immed = get_num(expression);
        else {
            registers[registers_handled] = get_register(expression); /* getting the actual value of the string representation of the register */
            registers_handled++; /* just handled one register */
        }

        args_handled++; /* just handled one operand */

        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i++; /* getting all the chars until we see a ',' */
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    }

    build_i_instruction(ic_memory_table, op, registers[0], registers[1], immed); /* converting the instruction to machine code */

    /* freeing allocated variables */
    free(registers); 

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes details about i instruction and convert it to machine code */
static void build_i_instruction(memory_table *ic_memory_table, opcode op, int rs, int rt, long immed){
    long value = 0; /* represents the instruction machine code */

    value |= op;
    value  = ((value << 5) | (rs & 31)); /* saving 5 LSB */
    value  = ((value << 5) | (rt & 31)); /* saving 5 LSB */
    value  = ((value << 16) | (immed & 65535)); /* saving 16 LSB */

    add_memory_elements(ic_memory_table, 4, value); /* converting the machine code into cells */
}

/* this function takes jmp instruction and tries to assemble it */
static bool assemble_jmp_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic) {
    char label[MAX_LINE_LENGTH]; /* the label in this instruction(if exists) */
    int reg = 0; /* says if the operand in this instruction is a register or not */
    int address; /* the address of the label in this instruction(if exists) */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars from i until the end of line */

    if (label[0] == '$') { /* means the operand is a register */
        reg = 1; /* the operand is a register */
        address = get_register(label); /* the address set to be the value of the register */
    }
    else {
        if (!exists(sym_table, label, ANY_SYMBOL)) { /* means the label doesn't exist */
            fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
            return FALSE;
        }
        address = get_address_of(sym_table, label); /* getting the address of the label */
    }

    if (exists(sym_table, label, EXTERNAL_SYMBOL)) /* we saw an external label, we should save this for later processing */
        add_extern_element(extern_list, label, *ic); /* saving the location */

    build_j_instruction(ic_memory_table, op, reg, address); /* converting the instruction to machine code */

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes la instruction and tries to assemble it */
static bool assemble_la_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic){
    return assemble_j_instruction_label_only(f_info, i, ic_memory_table, sym_table, extern_list, op, ic); /* la instruction */
}

/* this function takes call instruction and tries to assemble it */
static bool assemble_call_instruction(file_info f_info, int i, opcode op, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, long *ic){
    return assemble_j_instruction_label_only(f_info, i, ic_memory_table, sym_table, extern_list, op, ic); /* call instruction */
}

/* this function takes j instruction(that accepts label only) and tries to assemble it */
static bool assemble_j_instruction_label_only(file_info f_info, int i, memory_table *ic_memory_table, symbol_table *sym_table, extern_table *extern_list, opcode op, long *ic){
    char label[MAX_LINE_LENGTH]; /* the label in this instruction(if exists) */

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars from i until the end of line */

    if (!exists(sym_table, label, ANY_SYMBOL)) { /* means the label doesn't exist */
        fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    if (exists(sym_table, label, EXTERNAL_SYMBOL)) /* we saw an external label, we should save this for later processing */
        add_extern_element(extern_list, label, *ic); /* saving the location */

    build_j_instruction(ic_memory_table, op, 0, get_address_of(sym_table, label)); /* converting the instruction to machine code */

    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes stop instruction and tries to assemble it */
static bool assemble_stop_instruction(memory_table *ic_memory_table, opcode op, long *ic){
    build_j_instruction(ic_memory_table, op, 0, 0);
    *ic += 4; /* an instruction of 4 cells */
    return TRUE; /* assembling succeeded */
}

/* this function takes details about j instruction and convert it to machine code */
static void build_j_instruction(memory_table *ic_memory_table, opcode op, int reg, long address){
    long value = 0; /* represents the instruction machine code */

    value |= op;
    value  = ((value << 1) | (reg & 1)); /* saving 1 LSB */
    value  = ((value << 25) | (address & 33554431)); /* saving 25 LSB */

    add_memory_elements(ic_memory_table, 4, value); /* converting the machine code into cells */
}

/* this function takes db instruction and tries to assemble it */
static bool assemble_db_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 1); /* db instruction */
}

/* this function takes dh instruction and tries to assemble it */
static bool assemble_dh_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 2); /* dh instruction */
}

/* this function takes dw instruction and tries to assemble it */
static bool assemble_dw_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    return assemble_num_creation_instruction(f_info, i, dc_memory_table, dc, 4); /* dw instruction */
}

/* this function takes a num creation instruction(know what the type of the instruction according to the amount of cells it takes in memory and tries to assemble it */
static bool assemble_num_creation_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc, int cells_amount){
    char num[MAX_LINE_LENGTH]; /* buffer contaning the number as a string */

    /* getting the numbers one by one */
    while(!end_of_line(f_info.cur_line_content, i)){
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i = get_label_until(f_info.cur_line_content, num, i, ','); /* getting all the chars until we see a ',' */
        i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
        i++;

        add_memory_elements(dc_memory_table, cells_amount, get_num(num)); /* adding the number we read to the memory */

        *dc += cells_amount; /* the number has size of "cell_amout" cells */
    }

    return TRUE; /* assembling succeeded */
}

/* this function takes asciz instruction and tries to assemble it */
static bool assemble_asciz_instruction(file_info f_info, int i, memory_table *dc_memory_table, long *dc){
    int end_index = strlen(f_info.cur_line_content) - 1; /* index points to the end of the line */

    /* decrementing the index until we see a quote */
    while(end_index >= 0 && isspace(f_info.cur_line_content[end_index]))
        end_index--;

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    i++; /* we saw a quote and we move on to the next char in order to read the content between the quotes */

    /* reading the content between the quotes */
    for(; i < end_index; i++){
        add_memory_elements(dc_memory_table, 1, f_info.cur_line_content[i]); /* adding the char to the memory */
        *dc += 1; /* a char takes one cell in memory */
    }
    add_memory_elements(dc_memory_table, 1, '\0'); /* string terminator */
    *dc += 1; /* a char takes one cell in memory */

    return TRUE; /* assembling succeeded */
}

/* this function takes entry instruction and tries to assemble it */
static bool assemble_entry_instruction(file_info f_info, int i, symbol_table *sym_table){
    char label[MAX_LINE_LENGTH];

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars from i until the end of line */

    if(!exists(sym_table, label, ANY_SYMBOL)){ /* means the label doesn't exist */
        fprintf(stderr, "%s:%ld: the label \"%s\" doesn't exist\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    if (exists(sym_table, label, EXTERNAL_SYMBOL)) { /* means the label is external */
        fprintf(stderr, "%s:%ld: the external label \"%s\" can't be entry\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    add_attributes(sym_table, label, ENTRY_SYMBOL); /* means that from now on the label is entry */

    return TRUE; /* assembling succeeded */
}

static bool assemble_extern_instruction(file_info f_info, int i, symbol_table *sym_table) {
	char label[MAX_LINE_LENGTH];

    i = skip_spaces(f_info.cur_line_content, i); /* skipping white spaces */
    get_label(f_info.cur_line_content, label, i); /* reading all the chars from i until the end of line */


    if (exists(sym_table, label, ~EXTERNAL_SYMBOL)) { /* means the label is not external(therefore created in the source file itself) */
        fprintf(stderr, "%s:%ld: the label \"%s\" was created in the current file and can't be external\n", f_info.name, f_info.cur_line_number, label);
        return FALSE;
    }

    return TRUE; /* assembling succeeded */
}
