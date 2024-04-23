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

// Kevin Frazier
/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

    // Instruction address should be word aligned (divisble by 4)
    if ((PC % 4) != 0) {

    // Returning 1, since a halt condition occured
        return 1;
    }

    // 1. Store a value into the instruction variable
    // Get the instruction put it somewhere
    *instruction = Mem[PC >> 2];

    return 0; // Returning 0 since there is no halt



}

// Kevin Frazier
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // Extracting bits into integers
    //Given intructions and splitting them up into different parts

    // [bits 31-26]
    *op = instruction >> 26;
    // instruction [25-21]
    *r1 = instruction >> 21 & 0x1F;
    // instruction [20-16]
    *r2 = instruction >> 16 & 0x1F;
    // instruction [15-1]
    *r3 = instruction >> 11 & 0x1F;

    // instruction [5-0]
    *funct = instruction & 0x3F;
    // instruction [15-0]
    *offset = instruction & 0xFFFF;
    // instruction [25-0]
    *jsec = instruction & 0x3FFFFFF;
}


// REDONE @KEVIN
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls){
    // Initialize
    controls->RegDst = 0;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemRead = 0;
    controls->MemtoReg = 0;
    controls->ALUOp = 0;
    controls->MemWrite = 0;
    controls->ALUSrc = 0;
    controls->RegWrite = 0;
    // R-Type
    if (op == 0x0) {
        controls->RegDst = 1;
        controls->ALUOp = 7;
        controls->RegWrite = 1;
    // Add
    } else if (op == 0x8) {
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
    // Load
    } else if (op == 0x23) {
        controls->RegWrite = 1;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUSrc = 1;
    // Store
    } else if (op == 0x2b) {
        controls->MemWrite = 1;
        controls->RegDst = 2;
        controls->MemtoReg = 2;
        controls->ALUSrc = 1;
    // Upper
    } else if (op == 0xf) {
        controls->RegWrite = 1;
        controls->ALUOp = 6;
        controls->ALUSrc = 1;
    // Branch
    } else if (op == 0x4) {
        controls->Branch = 1;
        controls->RegDst = 2;
        controls->MemtoReg = 2;
        controls->ALUSrc = 1;
        controls->ALUOp = 1;
    // Less Than (Signed)
    } else if (op == 0xa) {
        controls->ALUOp = 2;
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
    // Less Than (Unsigned)
    } else if (op == 0xb) {
        controls->ALUOp = 3;
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
    // Jump
    } else if (op == 0x2) {
        controls->Jump = 1;
        controls->RegDst = 2;
        controls->Branch = 2;
        controls->MemtoReg = 2;
        controls->ALUSrc = 2;
        controls->ALUOp = 2;
    // Halt
    } else {
        return 1;
    }
    // Success
    return 0;
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
    unsigned sign_extended_value; // Result (Sign-extended value)
    if ((offset >> 15) == 1) {
        // Extend bits if signaficiant bit is set
        sign_extended_value = offset | 0xFFFF0000; // Extend bits
    } else {
        // Otherwise clear upper bits
        sign_extended_value = offset & 0x0000FFFF; // Clear upper bits
    }
    // [sign_extend(...) #1 in teacher's guideline]
    *extended_value = sign_extended_value;
}

/* ALU operations */
/* 10 Points */
// Michael C. (4/21/2024 @15:22:00)
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // [ALU_operations(...) #1 in teacher's guideline]
    if (ALUSrc == 1) {
        // Based on ALUSrc
        data2 = extended_value;
    }
    // [ALU_operations(...) #2 in teacher's guideline]
    if (ALUOp == 7){
        // Based on ALUOp and funct
        if (funct == 32) {
            ALUOp = 0;
        } else if (funct == 34) {
            ALUOp = 1;
        } else if(funct == 42) {
            ALUOp = 2;
        } else if (funct == 43) {
            ALUOp = 3;
        } else if (funct == 36) {
            ALUOp = 4;
        } else if (funct == 4) {
            ALUOp = 5;
        } else if (funct == 39) {
            ALUOp = 6;
        } else {
            // [ALU_operations(...) #5 in teacher's guideline]
            return 1; // Halt operation
        }
        // [ALU_operations(...) #3 and #4 in teacher's guideline]
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    } else {
        // [ALU_operations(...) #3 and #4 in teacher's guideline]
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    return 0; // Otherwise return 0
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned * memdata, unsigned * Mem) {
    //if memory is being read but not written
    if (MemRead == 0x1 && MemWrite == 0x0) {
        return * memdata;
    }
    //if memory is being written but not read
    if (MemRead == 0x0 && MemWrite == 0x1) {
        if (ALUresult % 4 == 0) {
            Mem[ALUresult >> 2] = data2;
        }
        return 0x1;
    }
    //if memory is being read and written
    if (MemRead == 0x1 && MemWrite == 0x1) {
        if (ALUresult % 4 == 0) {
            Mem[ALUresult >> 2] = data2;
        }
        return * memdata;
    }
    //if memory is not being read or written
    if (MemRead == 0x0 && MemWrite == 0x0) {
        return 0x0;
    }
    return 0x0; // REDONE @KEVIN I Added Because otherwise didn't have return in all instances. (Error for coding in C)
}

// REDONE @KEVIN
/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // If Register File
    if (RegWrite == 1) {
        // Determine Based on Source
        if (MemtoReg == 1) {
            // Write Memory
            Reg[r2] = memdata;
        } else if (MemtoReg == 0){
            // Determine Based on RegDst
            if (RegDst == 1) {
                // Write Memory
                Reg[r3] = ALUresult;
            } else {
                // Write Memory
                Reg[r2] = ALUresult;
            }
        }
    }
}

// REDONE @KEVIN
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // Increase Counter
    *PC += 4;
    // If Branched & Got Zero
    if (Branch == 1 && Zero == 1) {
        // Add Extended Value to PC
        *PC += extended_value << 2;
    }
    // If Need Jump
    if (Jump == 1) {
        *PC = (jsec << 2) | (*PC & 0xf000000);
    }
}
