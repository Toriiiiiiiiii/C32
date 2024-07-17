#pragma once
#define MEM_SIZE 16777216

#define BIOS 0x000000
    #define BIOS_BASE 0x000000
    #define BIOS_MAX  0x000FFF
#define IO   0x001000
    #define IO_TABLE  0x001000
    #define IO_BASE   0x001100
    #define IO_MAX    0x001FFF
#define VMEM 0x002000
    #define VMEM_BASE 0x002000
    #define VMEM_MAX  0x002FA0
    #define VMEM_MODE 0x002FA1
    #define VMEM_INST 0x002FA2
#define GRAM 0x003000
    #define GRAM_BASE 0x003000
    #define GRAM_MAX  0xEFFFFF
#define STCK 0xF00000
    #define STK_BASE  0xF00000
    #define STK_MAX   0xFFFFFF

#include <vector>
#include <string>
#include "device.h"

class MMU {
    private:
        Device **devices;
        int deviceCount;

        byte *memory;

        void __write_noprotect(dword nbytes, dword address, dword data);

    public:
        MMU();

        int registerDevice(Device* device);
        void unregisterDevice(int id);

        int loadBIOS(std::string fpath);
        void write(dword nbytes, dword address, dword data);
        dword read(dword nbytes, dword address);
};