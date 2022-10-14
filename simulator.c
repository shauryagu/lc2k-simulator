/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */

// File
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

int convertNum(int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    for (int i=0; i<NUMREGS; i++) {
              state.reg[i] = 0;
    }
  
    int opcode = 0;
    int numInst = 0;
    state.pc = 0;
    while (opcode != 0b110) {
        opcode = (state.mem[state.pc] >> 22) & 0x7;
        printState(&state);
        numInst++;
        int regA = 0; int regB = 0; int regC = 0;
        if(opcode == 0b000){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            regC = state.mem[state.pc] & 0x7;
            state.reg[regC] = state.reg[regA] + state.reg[regB];
            state.pc++;
        }
        else if(opcode == 0b001){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            regC = (state.mem[state.pc]) & 0x7;
            state.reg[regC] = ~(state.reg[regA] | state.reg[regB]);
            state.pc++;
        }
        else if(opcode == 0b010){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            regC = state.mem[state.pc] & 0xFFFF;
            int offset = convertNum(regC);
            state.reg[regB] = state.mem[state.reg[regA]+offset];
            state.pc++;
        }
        else if(opcode == 0b011){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            regC = state.mem[state.pc] & 0xFFFF;
            int offset = convertNum(regC);
            state.mem[state.reg[regA]+offset] = state.reg[regB];
            state.pc++;
        }
        else if(opcode == 0b100){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            regC = state.mem[state.pc] & 0xFFFF;
            int offset = convertNum(regC);
            if(state.reg[regA] == state.reg[regB])
                state.pc = state.pc + 1 + offset;
            else
                state.pc++;
        }
        
        else if(opcode == 0b101){
            regA = (state.mem[state.pc] >> 19) & 0x7;
            regB = (state.mem[state.pc] >> 16) & 0x7;
            state.reg[regB] = state.pc + 1;
            state.pc = state.reg[regA];
        }
        else if(opcode == 0b111){
            state.pc++;
        }
        else if(opcode == 0b110){
            state.pc++;
        }
    }
    printf("machine halted\n");
    printf("total of %d instructions executed\nfinal state of machine:\n", numInst);
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}
