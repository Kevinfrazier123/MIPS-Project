#include "spimcore.h"


/* ALU */
/* 10 Points */
// Michael C. (4/16/2024 @17:45:00)
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    unsigned Z = 0; // Result (Teacher just uzed (Z) so I used (Z).)
    // [ALU(...) #1,#4 in teacher's guideline]
    if (ALUControl == 0x0) {
        // ALU Control: 000
        // Meaning: Z = A + B
        Z = A + B;
    } else if (ALUControl == 0x1) {
        // ALU Control: 001
        // Meaning: Z = A – B
        Z = A - B;
    } else if (ALUControl == 0x2) {
        // ALU Control: 010
        // Meaning: if A < B, Z = 1; otherwise, Z = 0
        if ((int) A < (int) B) {
            Z = 1;
        } else {
            Z = 0;
        }
    } else if (ALUControl == 0x3) {
        // ALU Control: 011
        // Meaning: if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
        Z = (A < B) ? 1 : 0;
    } else if (ALUControl == 0x4) {
        // ALU Control: 100
        // Meaning: Z = A AND B
        Z = A & B;
    } else if (ALUControl == 0x5) {
        // ALU Control: 101
        // Meaning: Z = A OR B
        Z = A | B;
    } else if (ALUControl == 0x6) {
        // ALU Control: 110
        // Meaning: Z = Shift B left by 16 bits
        Z = B << 16;
    } else if (ALUControl == 0x7) {
        // ALU Control: 111
        // Meaning: Z = NOT A
        Z = ~A;
    }
    // [ALU(...) #2 in teacher's guideline]
    *ALUresult = Z; // Set ALUresult to (Z)

    // [ALU(...) #3 in teacher's guideline]
    if (*ALUresult == 0) {
        // Set Zero to 1 if Z is 0
        *Zero = 1;
    } else {
        // Otherwise set Zero to 0
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
// Michael C. (4/17/2024 @20:28:00)
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // [read_register(...) #1 in teacher's guideline]
    *data1 = Reg[r1]; // Read r1 from Reg and write to data1
    // [read_register(...) #1 in teacher's guideline]
    *data2 = Reg[r2]; // Read r2 from Reg and write to data2
}


/* Sign Extend */
/* 10 Points */
// Michael C. (4/17/2024 @23:54:00)
void sign_extend(unsigned offset,unsigned *extended_value)
{
    if ((offset >> 15) == 1) {
        *extended_value = offset | 0xFFFF0000;
    } else {
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

