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


//Kevin Frazier 
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    // Control signals from spincore.h
    // Value 1 means enabled, 0 indicates selected path of Mx, 2 dont care
    switch (op){

    // r-type instructions 
    case 0x000000000: 
    controls->RegDst = 0x1;
	controls->Jump = 0x0;  // not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x7;
	controls->MemWrite = 0x0; //  not writing to mem
	controls->ALUSrc = 0x0;
	controls->RegWrite = 0x1; // writing data to reg
    break;  

    // i-tpye instructions 
    case 0x20000000: 
    controls->RegDst = 0x0; 
	controls->Jump = 0x0; // not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; //not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x0; // ADD ALU
	controls->MemWrite = 0x0;  // not writing to mem
	controls->ALUSrc = 0x1;
	controls->RegWrite = 0x1; // writing data to reg
    break;

    // load word (i-type instruction)
    case 0x8c000000: 
    controls->RegDst = 0x0;
	controls->Jump = 0x0; // not jumping 
	controls->Branch = 0x0; // not branching 
	controls->MemRead = 0x1; 
	controls->MemtoReg = 0x1;
	controls->ALUOp = 0x0; // ADD ALU
	controls->MemWrite = 0x0;  // not writing to mem
	controls->ALUSrc = 0x1; 
	controls->RegWrite = 0x1; // writing data to reg
    break;

    // store word(i-type instruction)
    case 0xac000000:
    controls->RegDst = 0x0; 
	controls->Jump = 0x0; // not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x0; // ADD ALU
	controls->MemWrite = 0x1;
	controls->ALUSrc = 0x1;
	controls->RegWrite = 0x0; // not writing data to reg
    break;

    // i-type 
    case 0x3c000000:
    controls->RegDst = 0x0;
	controls->Jump = 0x0; // not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x6;
	controls->MemWrite = 0x0;
	controls->ALUSrc = 0x1;
	controls->RegWrite = 0x1; // writing data to reg
    break;

    // i-type
    case 0x28000000:
    controls->RegDst = 0x0;
	controls->Jump = 0x0; //not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x2;
	controls->MemWrite = 0x0; // not writing to mem
	controls->ALUSrc = 0x1;
	controls->RegWrite = 0x1; //  writing data to reg
    break;

    // i-type
    case 0x2c000000:
    controls->RegDst = 0x0;
	controls->Jump = 0x0; // not jumping
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x3;
	controls->MemWrite = 0x0; // not writing to mem
	controls->ALUSrc = 0x1;
	controls->RegWrite = 0x1; // writing data to reg
    break;

    // branch eqaul to
    case 0x10000000:
    controls->RegDst = 0x0;
	controls->Jump = 0x0; // not jumping
	controls->Branch = 0x1;
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0;
	controls->ALUOp = 0x1;
	controls->MemWrite = 0x0; // not writing to mem
	controls->ALUSrc = 0x0;
	controls->RegWrite = 0x0; // not writing data to reg
    break;

    //Jump
    case 0x08000000:
    controls->RegDst = 0x0;
	controls->Jump = 0x1; 
	controls->Branch = 0x0; // not branching
	controls->MemRead = 0x0; // not reading mem
	controls->MemtoReg = 0x0; 
	controls->ALUOp = 0x0; // ADD ALU
	controls->MemWrite = 0x0; //  not writing to mem
	controls->ALUSrc = 0x0;
	controls->RegWrite = 0x0; //not writing data to reg
    break;

    default:
    // returning halt 
        return 1;
    }

    return 0;
    // not hault return 0
   

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

