/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results.

#include "header.h"
#include "lab8header.h"

extern char *regNameTab[N_REG];
unsigned int PCRegister = 0;   // actual definition here

void CPU(char *mem) {
    unsigned int machineCode = 0;
    unsigned char opcode = 0;

    PCRegister = CODESECTION;  // PC starts from code section

    do {
        printf("\nPC:%X\n", PCRegister);

        machineCode = CPU_fetchCode(mem, PCRegister);

        if (machineCode == 0) {   // end of code
            break;
        }

        opcode = CPU_Decode(machineCode);
        printf("Fetched Machine Code: %08X\n", machineCode);
        printf("Decoded Opcode is: %02X\n", opcode);

        // Lab 9: execution will be enabled later
        // CPU_Execution(opcode, machineCode, mem);

        // For Lab 8 only, move to next instruction after fetch/decode
        PCRegister += 4;

    } while (1);

    printRegisterFiles();      // print all register contents
    printDataMemoryDump(mem);  // dump first 256 bytes from data section
}

// Lab 8 - Step 1. Read 32-bit machine code from memory.
unsigned int CPU_fetchCode(char *mem, int codeOffset) {
    return read_dword(mem, codeOffset);
}

// Lab 8 - Step 2. Decode the instruction and return opcode/function.
// For R-type: opcode field is 000000, so return funct field instead.
// For I-type and J-type: return opcode field.
unsigned char CPU_Decode(unsigned int machineCode) {
    unsigned char opcode = 0;

    opcode = (machineCode >> 26) & 0x3F;   // top 6 bits

    if (opcode == 0) {
        // R-type instruction, return funct field
        return (unsigned char)(machineCode & 0x3F);
    }

    // I-type or J-type instruction, return opcode field
    return opcode;
}

// Lab 9: Finish the function CPU_Execution to run all the instructions.
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem) {
    unsigned char rt = 0;

    switch (opcode) {
        // This is an example how lab will be executed. Please follow this example and finish executions of the code.
        // Hint: you need to implement the following instructions here:
        //       la, add, lb, bge, lw, sw, addi, j

        case 0b101111:   // "la" instruction
            rt = (machineCode & 0x001F0000) >> 16;
            regFile[rt] = machineCode & 0x0000FFFF;

            if (DEBUG_CODE) {
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        case 0b100000:   // "lb" instruction
            // ....
            break;

        // continue to all the other cases used in the program.
        // case ......:

        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode, machineCode);
            system("PAUSE");
            exit(3);
            break;
    }
}

// Lab 8 - Step 3. Print all the registers in regFile with register names.
void printRegisterFiles() {
    int i;

    printf("\n=========== Register File Dump ===========\n");

    for (i = 0; i < N_REG; i++) {
        if (regNameTab != NULL && regNameTab[i] != NULL) {
            printf("%-6s = 0x%08X\n", regNameTab[i], regFile[i]);
        } else {
            printf("R%-5d = 0x%08X\n", i, regFile[i]);
        }
    }
}

// Lab 8 - Step 4. Dump first 256 bytes from data section.
void printDataMemoryDump(char *mem) {
    printf("\n=========== Data Memory Dump ===========\n");
    memory_dump(mem, DATASECTION, MIN_DUMP_SIZE);
}
