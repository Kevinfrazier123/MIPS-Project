if (ALUControl == 0x0) {
        // ALU Control: 000
        // Meaning: Z = A + B
        Z = A + B;
    } else if (ALUControl == 0x1) {
        // ALU Control: 001
        // Meaning: Z = A – B