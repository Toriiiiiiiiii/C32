#include <cstdio>
#include <pthread.h>
#include <vector>

#include "arguments.h"
#include "memory.h"
#include "cpu.h"

void printUsage(char **argv) {
    printf("Usage : %s [-h]\n", argv[0]);
}

void runEmulator() {
    MMU mmu = MMU();
    mmu.loadBIOS("BIOS");

    CPU cpu = CPU(&mmu);
    cpu.writeReg(1, HL0, 0xfa);
    cpu.writeReg(1, LH0, 0xce);

    printf("0x%08x\n", cpu.readReg(4, 0));
    printf("0x%04x\n", cpu.readReg(2, L0));
}

int main(int argc, char **argv) {
    std::vector<ArgumentTemplate> usage = {
        ArgumentTemplate{ARG_OPTIONAL | ARG_IMPLIED, "-h", "help"}
    };

    ArgumentParser *argParser = new ArgumentParser(argc, argv, usage);

    std::vector<Argument> args;
    if(!argParser->parseArguments(args)) {
        printUsage(argv);
        goto cleanup;
    }

    if(argParser->argumentExists(args, "help")) {
        printUsage(argv);
        goto cleanup;
    }

    runEmulator();

cleanup:
    delete argParser;
    return 0;
}