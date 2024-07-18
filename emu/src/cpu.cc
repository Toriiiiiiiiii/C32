#include "cpu.h"
#include <cstdio>

/*class CPU {
    private:
        MMU *mmu;

        Instruction fetch();
        dword readSource(Instruction inst);
        dword readDest(Instruction inst);
        void writeDest(Instruction inst, dword value);

    public:
        dword *regs;
        dword pc, sp;

        CPU(MMU *mmu);
        
        int exec();
};*/

CPU::CPU(MMU *mmu) {
    this->mmu = mmu;
    this->regs = new dword[4];

    regs[0] = regs[1] = regs[2] = regs[3] = 0;
}

Instruction CPU::fetch() {
    Instruction result = {};
    result.opcode = mmu->read(1, pc);
    
    byte bitmode_sourcemode = mmu->read(1, pc+1);
    byte destmode_unused = mmu->read(1, pc+2);
    byte rs1_rs2 = mmu->read(1, pc+3);

    result.bitmode = (bitmode_sourcemode >> 4) & 0xf;
    result.sourcemode = bitmode_sourcemode & 0xf;
    result.destmode = (destmode_unused >> 4) & 0xf;
    result.rs1 = (rs1_rs2 >> 4) & 0xf;
    result.rs2 = rs1_rs2 & 0xf;

    pc += 4;
    return result;
}

dword CPU::readSource(Instruction inst) {
    switch(inst.sourcemode) {
        case IMPL: return 0;
        case REG: return readReg(inst.bitmode, inst.rs1);
        case IMM: 
            pc += inst.bitmode;
            return mmu->read(inst.bitmode, pc-inst.bitmode);
        case ABS:
            pc += 4;
            return mmu->read(inst.bitmode, mmu->read(4, pc-4));
        case IND:
            pc += 4;
            return mmu->read(inst.bitmode, mmu->read(4, mmu->read(4, pc-4)));
        default:
            return 0;
    }
}

dword CPU::readDest(Instruction inst) {
    inst.sourcemode = inst.destmode;
    return readSource(inst);
}

void CPU::writeDest(Instruction inst, dword value) {
    switch(inst.destmode) {
        case IMPL: return;
        case REG: 
            writeReg(inst.bitmode, inst.rs2, value);
            break;
        case IMM: return;
        case ABS:
            mmu->write(inst.bitmode, mmu->read(4, pc), value);
            pc += 4;
            break;

        case IND:
            mmu->write(inst.bitmode, mmu->read(4, mmu->read(4, pc)), value);
            pc += 4;
            break;
    }
}

dword CPU::readReg(byte bitmode, byte index) {
    if(bitmode == 4) return regs[index & 0b11];

    if(bitmode == 2) {
        dword regVal32 = regs[(index / 2) & 0b11];
        return (regVal32 >> ( (index%2 == 0)? 16 : 0 )) & 0xffff;
    }

    if(bitmode == 1) {
        dword regVal32 = regs[(index / 4) & 0b11];
        dword regVal16 = (regVal32 >> ( (index%4 < 2)? 16 : 0 )) & 0xffff;
        return (regVal16 >> ( (index%2 == 0)? 8 : 0 )) & 0xff;
    }

    return 0;
}

void CPU::writeReg(byte bitmode, byte index, dword value) {
    if(bitmode == 4) regs[index & 0b11] = value;

    if(bitmode == 2) {
        value = (value & 0xffff) << ( (index%2 == 0)? 16 : 0 );
        dword mask = ~(0xffff << ( (index%2 == 0)? 16 : 0 ));

        regs[(index/2) & 0b11] &= mask;
        regs[(index/2) & 0b11] |= value;
    }

    if(bitmode == 1) {
        value = (value & 0xff) << 24;
        value = value >> (8 * (index % 4));

        dword mask = ~(0xff000000 >> 8 * (index % 4));
        regs[(index/4) & 0b11] &= mask;
        regs[(index/4) & 0b11] |= value;
    }
}