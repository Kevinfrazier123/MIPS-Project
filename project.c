#include "spimcore.h"
//Members: 
//Kevin Frazier
// Michael C.
//Paul B.

/* ALU */
/* 10 Points */
// Michael C. (4/16/2024 @17:45:00)
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    int aluOp = (int) ALUControl;
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
    // 1. Store a value into the instruction variable
    // Get the instruction put it somewhere 
    *instruction = Mem[PC >> 2];

    // Instruction address should be word aligned (divisble by 4)
    if ((PC % 4) != 0) {

    // Returning 1, since a halt condition occured 
        return 1;
    }
   

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
    *op = (instruction >> 26) & 0x3f; 
    // instruction [25-21]
    *r1 = (instruction >> 21) & 0x1F; 
    // instruction [20-16]
    *r2 = (instruction >> 16) & 0x1F; 
    // instruction [15-1]
    *r3 = (instruction >> 11) & 0x1F; 

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
    case 0: 
    controls->RegDst = 1;
	controls->Jump = 0;  // not jumping
	controls->Branch = 0; // not branching
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 0;
	controls->ALUOp = 7;
	controls->MemWrite = 0; //  not writing to mem
	controls->ALUSrc = 0;
	controls->RegWrite = 1; // writing data to reg
    break;  

    // i-tpye instructions 
    case 8: 
    controls->RegDst = 0; 
	controls->Jump = 0; // not jumping
	controls->Branch = 0; // not branching
	controls->MemRead = 0; //not reading mem
	controls->MemtoReg = 0;
	controls->ALUOp = 0; // ADD ALU
	controls->MemWrite = 0;  // not writing to mem
	controls->ALUSrc = 1;
	controls->RegWrite = 1; // writing data to reg
    break;

    
    case 10: 
    controls->RegDst = 0;
	controls->Jump = 0; // not jumping 
	controls->Branch = 0; // not branching 
	controls->MemRead = 0; 
	controls->MemtoReg = 0;
	controls->ALUOp = 2; // ADD ALU
	controls->MemWrite = 0;  // not writing to mem
	controls->ALUSrc = 1; 
	controls->RegWrite = 1; // writing data to reg
    break;

    // left immediate unsigned
    case 11:
    controls->RegDst = 0; 
	controls->Jump = 0; // not jumping
	controls->Branch = 0; // not branching
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 0;
	controls->ALUOp = 3; // ADD ALU
	controls->MemWrite = 0;
	controls->ALUSrc = 1;
	controls->RegWrite = 1; //  writing data to reg
    break;

     // jumping
    case 2:
    controls->RegDst = 0;
	controls->Jump = 1; //jumping
	controls->Branch = 0; // not branching
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 0;
	controls->ALUOp = 0;
	controls->MemWrite = 0;
	controls->ALUSrc = 0;
	controls->RegWrite = 0; // writing data to reg
    break;

    // branching equals 
    case 4:
    controls->RegDst = 2;
	controls->Jump = 0; //not jumping
	controls->Branch = 1; // branching
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 2;
	controls->ALUOp = 1;
	controls->MemWrite = 0; // not writing to mem
	controls->ALUSrc = 0;
	controls->RegWrite = 0; //  writing data to reg
    break;

    // load word
    case 35:
    controls->RegDst = 0;
	controls->Jump = 0; // not jumping
	controls->Branch = 0; // not branching
	controls->MemRead = 1; //  reading mem
	controls->MemtoReg = 1;
	controls->ALUOp = 0;
	controls->MemWrite = 0; // not writing to mem
	controls->ALUSrc = 1;
	controls->RegWrite = 1; // writing data to reg
    break;

    
    case 43:
    controls->RegDst = 2;
	controls->Jump = 0; // not jumping
	controls->Branch = 0;
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 2;
	controls->ALUOp = 0;
	controls->MemWrite = 1; // writing to mem
	controls->ALUSrc = 1;
	controls->RegWrite = 0; // not writing data to reg
    break;

    //upper immediate load
    case 15:
    controls->RegDst = 0;
	controls->Jump = 0; 
	controls->Branch = 0; // not branching
	controls->MemRead = 0; // not reading mem
	controls->MemtoReg = 0; 
	controls->ALUOp = 6; // ADD ALU
	controls->MemWrite = 0; //  not writing to mem
	controls->ALUSrc = 1;
	controls->RegWrite = 1; //writing data to reg
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

//DO
/* Sign Extend */
/* 10 Points */
// Michael C. (4/17/2024 @23:54:00)
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // declaring the unsigned signbit value, then extracting
    unsigned Bit = ((offset >> 15) & 1);

    if (Bit == 1){
        *extended_value = offset | 0xFFFF0000;  // Extend bits
        // Extend bits if signaficiant bit is set
    } else {
        *extended_value = offset & 0x0000FFFF;  // Clear upper bits
    }
}

// DONE
/* ALU operations */
/* 10 Points */
// Michael C. (4/21/2024 @15:22:00)
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    int op =0;
    // [ALU_operations(...) #1 in teacher's guideline]

    // [ALU_operations(...) #2 in teacher's guideline]
    
    if (ALUOp == 7){ //If ALUop is equal to 7 this will indicate the R-type operation

        switch(funct){
            case 32:
            //adding 
                op = 0;
                break;

            case 34:
            //Subtracting
                op = 1;
                break;
            // AND
            case 36:
                op = 4;
                break;
            // OR
            case 37:
                op = 5;
                break;
            //Less than
            case 42:
                op = 2;
                break;
            //less than unsigned
            case 43:
                op = 3;
                break;
            default:
                return 1;
                // returning 1 is an invalid ALU OP
        } 
        } else {
            op = ALUOp;
        }
        
        // [ALU_operations(...) #3 and #4 in teacher's guideline]
        // Once ALU is determined the function will call the ALU function with the corresponding parameters
        if (ALUSrc ==1){
            // responsible for perfomring ALU operations
            ALU(data1,extended_value,op,ALUresult, Zero);
        } else {
            // This is also responsible for performing ALU operations
            ALU(data1, data2,op,ALUresult, Zero);
        }
    
    return 0; // Otherwise return 0
}

//DONE
/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

    if (MemRead==1){ //Checks if the control signal is active 
        if(ALUresult %4 !=0){ // Checks if the address is aligned
            return 1;
        }
        *memdata = Mem[ALUresult>>2]; // stores aligned word in memdata 
    }

    if (MemWrite==1){ // if control signal is active mem write operation should happen
        if(ALUresult%4 !=0){
            return 1;
        }
        Mem[ALUresult>>2]= data2;
    }
    return 0; // if aligned for read and write return 0 
}

// Paul DONE
/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if(RegWrite==1){
    // determing data to write
    unsigned data;
    if (MemtoReg ==1){
        data = memdata; // Writing ALU result to reg
    } else {
        data = ALUresult; // ALU result to the reg
    }
    // This will determine the address of the reg from regDst control signal
      unsigned registerN;
      if (RegDst == 1){
        registerN = r3; //Destination reg
      } else {
        registerN = r2; // Destination reg
      }
        // This will write the result the to the specific reg
      Reg[registerN] = data;
  }



}

// Paul DONE
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	// Will increase by 4
    *PC = *PC+4; // This will add 4 to the value of the pointer *PC
    if(Branch ==1 && Zero ==1 ){
        // Will add extnedd value if the branch and jump is eqaul to 1
        *PC = *PC +(extended_value <<2);
    }
    if (Jump==1){ // jump flag = 1

        *PC = ((*PC)&0xF0000000) ^ (jsec<<2); // shifting jump adress left by 2 bits
    }

}
