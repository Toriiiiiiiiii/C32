#pragma once
#define MEM_SIZE 16777216

#define BIOS_BASE 0x000000
#define IO_BASE   0x001000
#define VMEM_BASE 0x002000
#define GRAM_BASE 0x003000
#define STCK_BASE 0xF00000

#include <vector>
#include "device.h"

class MMU {
    private:
        std::vector<Device*> devices = {0};
        byte memory[MEM_SIZE] = {0};

    public:
        int registerDevice(Device* device);
        void unregisterDevice(int id);

        void write(dword nbytes, dword address, dword data);
        dword read(dword nbytes, dword address);
};