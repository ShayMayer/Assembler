/* this file contains functions and general information about the assembly code */
#include <string.h>
#include <stdlib.h>
#include "code.h"

instruction_info *get_instruction_info(char *name){
	/* info about the instructions */
	static instruction_info info[INSTRUCTIONS_AMOUNT] = {
		{"add", ADD, ADD_OP, ADD_FUNCT, R_INSTRUCTION},
		{"sub", SUB, SUB_OP, SUB_FUNCT, R_INSTRUCTION},
		{"and", AND, AND_OP, AND_FUNCT, R_INSTRUCTION},
		{"or", OR, OR_OP, OR_FUNCT, R_INSTRUCTION},
		{"nor", NOR, NOR_OP, NOR_FUNCT, R_INSTRUCTION},
		{"move", MOVE, MOVE_OP, MOVE_FUNCT, R_INSTRUCTION},
		{"mvhi", MVHI, MVHI_OP, MVHI_FUNCT, R_INSTRUCTION},
		{"mvlo", MVLO, MVLO_OP, MVLO_FUNCT, R_INSTRUCTION},
		{"addi", ADDI, ADDI_OP, NONE_FUNCT, I_INSTRUCTION},
		{"subi", SUBI, SUBI_OP, NONE_FUNCT, I_INSTRUCTION},
		{"andi", ANDI, ANDI_OP, NONE_FUNCT, I_INSTRUCTION},
		{"ori", ORI, ORI_OP, NONE_FUNCT, I_INSTRUCTION},
		{"nori", NORI, NORI_OP, NONE_FUNCT, I_INSTRUCTION},
		{"bne", BNE, BNE_OP, NONE_FUNCT, I_INSTRUCTION},
		{"beq", BEQ, BEQ_OP, NONE_FUNCT, I_INSTRUCTION},
		{"blt", BLT, BLT_OP, NONE_FUNCT, I_INSTRUCTION},
		{"bgt", BGT, BGT_OP, NONE_FUNCT, I_INSTRUCTION},
		{"lb", LB, LB_OP, NONE_FUNCT, I_INSTRUCTION},
		{"sb", SB, SB_OP, NONE_FUNCT, I_INSTRUCTION},
		{"lw", LW, LW_OP, NONE_FUNCT, I_INSTRUCTION},
		{"sw", SW, SW_OP, NONE_FUNCT, I_INSTRUCTION},
		{"lh", LH, LH_OP, NONE_FUNCT, I_INSTRUCTION},
		{"sh", SH, SH_OP, NONE_FUNCT, I_INSTRUCTION},
		{"jmp", JMP, JMP_OP, NONE_FUNCT, J_INSTRUCTION},
		{"la", LA, LA_OP, NONE_FUNCT, J_INSTRUCTION},
		{"call", CALL, CALL_OP, NONE_FUNCT, J_INSTRUCTION},
		{"stop", STOP ,STOP_OP, NONE_FUNCT, J_INSTRUCTION},
		{".db", DB, NONE_OP, NONE_FUNCT, DB_INSTRUCTION},
		{".dh", DH, NONE_OP, NONE_FUNCT, DH_INSTRUCTION},
		{".dw", DW, NONE_OP, NONE_FUNCT, DW_INSTRUCTION},
		{".asciz", ASCIZ, NONE_OP, NONE_FUNCT, ASCIZ_INSTRUCTION},
		{".entry", ENTRY, NONE_OP, NONE_FUNCT, ENTRY_INSTRUCTION},
		{".extern", EXTERN, NONE_OP, NONE_FUNCT, EXTERN_INSTRUCTION}
	};

	int i;
	for(i = 0; i < INSTRUCTIONS_AMOUNT; i++) /* looking for an instruction which its name is identical to the given parameter */
		if(strcmp(info[i].name, name) == 0)
			return &info[i];
	return NULL;
}


/* this function returns the value of a given register */
int get_register(char *register_content) {
    char reg_value[3]; /* the value of the given register, represented as a string */
	reg_value[0] = register_content[1]; 
	reg_value[1] = register_content[2]; 
	reg_value[2] = '\0'; 
    return atoi(reg_value); /* converting the string to a number and returns it */
}