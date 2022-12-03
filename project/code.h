/* this file contains functions and general information about the assembly code */
#ifndef CODE_H
#define CODE_H

#include "utils.h"

#define INSTRUCTIONS_AMOUNT 33 /* amount of instructions the assembly language consists of */

/* this enum conatians ids for each instruction */
typedef enum instruction_id {
	ADD = 0, /* add instruction */
	SUB, /* sub instruction */
	AND, /* and instruction */
	OR, /* or instruction */
	NOR, /* nor instruction */
	MOVE, /* move instruction */
	MVHI, /* mhvi instruction */
	MVLO, /* mvlo instruction */
	ADDI, /* addi instruction */
	SUBI, /* subi instruction */
	ANDI, /* andi instruction */
	ORI, /* ori instruction */
	NORI, /* nori instruction */ 
	BNE, /* bne instruction */
	BEQ, /* beq instruction */
	BLT, /* blt instruction */
	BGT, /* bgt instruction */
	LB, /* lb instruction */
	SB, /* sb instruction */
	LW, /* lw instruction */
	SW, /* sw instruction */
	LH, /* lh instruction */
	SH, /* sh instruction */
	JMP, /* jmp instruction */
	LA, /* la instruction */
	CALL, /* call instruction */
	STOP, /* stop instruction */
	DB, /* db instruction */
	DH, /* dh instruction */
	DW, /* dw instruction */
	ASCIZ, /* asciz instruction */
	ENTRY, /* entry instruction */
	EXTERN /* extern instruction */
} instruction_id;

/* this enum contains the opcodes of all the instrucions(according to the task) */
typedef enum opcodes {
    ADD_OP = 0, /* add opcode */
    SUB_OP = 0, /* sub opcode */
    AND_OP = 0, /* and opcode */
    OR_OP = 0, /* or opcode */
    NOR_OP = 0, /* nor opcode */

    MOVE_OP = 1, /* move opcode */
    MVHI_OP = 1, /* mvhi opcode */
    MVLO_OP = 1, /* mvlo opcode */

    ADDI_OP = 10, /* addi opcode */
    SUBI_OP = 11, /* subi opcode */
    ANDI_OP = 12, /* andi opcode */
    ORI_OP = 13, /* ori opcode */
    NORI_OP = 14, /* nori opcode */

    BNE_OP = 15, /* bne opcode */
    BEQ_OP = 16, /* beq opcode */
    BLT_OP = 17, /* blt opcode */
    BGT_OP = 18, /* bgt opcode */

    LB_OP = 19, /* lb opcode */
    SB_OP = 20, /* sb opcode */
    LW_OP = 21, /* lw opcode */
    SW_OP = 22, /* sw opcode */
    LH_OP = 23, /* lh opcode */
    SH_OP = 24, /* sh opcode */

    JMP_OP = 30, /* jmp opcode */
    LA_OP = 31, /* la opcode */
    CALL_OP = 32, /* call opcode */
    STOP_OP = 63, /* stop opcode */

    NONE_OP = -1 /* not an opcode */
} opcode;

/* this enum contains the functs of all the instrucions(according to the task) */
typedef enum funct {
    ADD_FUNCT = 1, /* add funct */
    SUB_FUNCT = 2, /* sub funct */
    AND_FUNCT = 3, /* and funct */
    OR_FUNCT = 4, /* or funct */
    NOR_FUNCT = 5, /* nor funct */

    MOVE_FUNCT = 1, /* move funct */
    MVHI_FUNCT = 2, /* mvhi funct */
    MVLO_FUNCT = 3, /* mvlo funct */

    NONE_FUNCT = -1 /* not a funct */
} funct;

/* this struct represents all the necessary info about an insturction */
typedef struct instruction_info {
	char *name; /* name of the instruction */
	int id; /* id of the instruction */
	int opcode; /* opcode of the instruction */
	int funct; /* funct of the instruction */
} instruction_info;

/* this function returns the info object which its name is identical to the given parameter */
instruction_info *get_instruction_info(char *name);
/* this function returns the value of a given register */
int get_register(char *register_content);

#endif
