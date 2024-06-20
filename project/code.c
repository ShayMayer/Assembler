/* this file contains functions and general information about the assembly code */
#include <string.h>
#include <stdlib.h>
#include "code.h"

/* returns the instrucion_info which its name is the same as the given name */
instruction_info *get_instruction_info(char *name){
    /* info about the instructions */
    static instruction_info info[INSTRUCTIONS_AMOUNT] = {
        {"add", ADD, ADD_OP, ADD_FUNCT},
        {"sub", SUB, SUB_OP, SUB_FUNCT},
        {"and", AND, AND_OP, AND_FUNCT},
        {"or", OR, OR_OP, OR_FUNCT},
        {"nor", NOR, NOR_OP, NOR_FUNCT},
        {"move", MOVE, MOVE_OP, MOVE_FUNCT},
        {"mvhi", MVHI, MVHI_OP, MVHI_FUNCT},
        {"mvlo", MVLO, MVLO_OP, MVLO_FUNCT},
        {"addi", ADDI, ADDI_OP, NONE_FUNCT},
        {"subi", SUBI, SUBI_OP, NONE_FUNCT},
        {"andi", ANDI, ANDI_OP, NONE_FUNCT},
        {"ori", ORI, ORI_OP, NONE_FUNCT},
        {"nori", NORI, NORI_OP, NONE_FUNCT},
        {"bne", BNE, BNE_OP, NONE_FUNCT},
        {"beq", BEQ, BEQ_OP, NONE_FUNCT},
        {"blt", BLT, BLT_OP, NONE_FUNCT},
        {"bgt", BGT, BGT_OP, NONE_FUNCT},
        {"lb", LB, LB_OP, NONE_FUNCT},
        {"sb", SB, SB_OP, NONE_FUNCT},
        {"lw", LW, LW_OP, NONE_FUNCT},
        {"sw", SW, SW_OP, NONE_FUNCT},
        {"lh", LH, LH_OP, NONE_FUNCT},
        {"sh", SH, SH_OP, NONE_FUNCT},
        {"jmp", JMP, JMP_OP, NONE_FUNCT},
        {"la", LA, LA_OP, NONE_FUNCT},
        {"call", CALL, CALL_OP, NONE_FUNCT},
        {"stop", STOP ,STOP_OP, NONE_FUNCT},
        {".db", DB, NONE_OP, NONE_FUNCT},
        {".dh", DH, NONE_OP, NONE_FUNCT},
        {".dw", DW, NONE_OP, NONE_FUNCT},
        {".asciz", ASCIZ, NONE_OP, NONE_FUNCT},
        {".entry", ENTRY, NONE_OP, NONE_FUNCT},
        {".extern", EXTERN, NONE_OP, NONE_FUNCT}
    };

    int i;
    for(i = 0; i < INSTRUCTIONS_AMOUNT; i++) 
        if(strcmp(info[i].name, name) == 0)
            return &info[i];
    return NULL;
}
