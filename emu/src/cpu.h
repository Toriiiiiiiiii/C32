#pragma once
#include "global.h"
#include "memory.h"

#define R0 0
    #define H0 0
        #define HH0 0
        #define HL0 1
    #define L0 1
        #define LH0 2
        #define LL0 3
#define R1 1
    #define H1 2
        #define HH1 4
        #define HL1 5
    #define L1 3
        #define LH1 6
        #define LL1 7
#define R2 2
    #define H2 4
        #define HH2 8
        #define HL2 9
    #define L2 5
        #define LH2 10
        #define LL2 11
#define R3 3
    #define H3 6
        #define HH3 12
        #define HL3 13
    #define L3 7
        #define LH3 14
        #define LL3 15

typedef enum {
    NOP, MOV, ADD, SUB,
    MUL, DIV, MOD, AND,
    LOR, NOT, CMP, PSH,
    POP, JMP, JIZ, JIC,
    JIL, JIG, JSR, RTS
} opcode;

typedef enum {
    IMPL, REG, IMM, ABS, IND
} addressModes;

typedef struct {
    byte opcode;
    byte bitmode;
    byte sourcemode;
    byte destmode;
    byte rs1;
    byte rs2;
} Instruction;

class CPU {
    private:
        MMU *mmu;

    public:
        dword *regs;
        dword pc, sp;

        CPU(MMU *mmu);
        
        Instruction fetch();
        dword readReg(byte bitmode, byte index);
        void writeReg(byte bitmode, byte index, dword value);

        dword readSource(Instruction inst);
        dword readDest(Instruction inst);
        void writeDest(Instruction inst, dword value);
        int exec();
};