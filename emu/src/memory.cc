#include "memory.h"
#include "device.h"
#include "global.h"
#include <string>
#include <filesystem>
#include <fstream>

MMU::MMU() {
    memory = new byte[MEM_SIZE];
    devices = new Device*[MEM_SIZE];
    deviceCount = 0;
}

int MMU::registerDevice(Device *device) {
    if(deviceCount >= 64) {
        log(LOG_ERROR, "Could not register device : Maximum number of devices reached.");
        return -1;
    }

    int index = 0;
    while(index < 64 && devices[index] == NULL) {
        index++;
    }

    if(index >= 64) return -1;

    devices[index] = device;
    deviceCount++;
    return index;
}

void MMU::unregisterDevice(int id) {
    if(id < 0 || id >= 64) return;

    devices[id] = NULL;
    deviceCount--;
}

void MMU::write(dword nbytes, dword address, dword data) {
    if((address >= BIOS_BASE && address <= BIOS_MAX) || 
       (address >= IO_TABLE && address < IO_BASE)) {
        std::string msg = "Attempted to write to protected memory (address " + std::to_string(address) + ")";
        log(LOG_ERROR, msg);
        return;
    }

    __write_noprotect(nbytes, address, data);
}

dword MMU::read(dword nbytes, dword address) {
    address &= 0xFFFFFF;
    dword result = 0;
    switch(nbytes) {
        case 1:
            result |= memory[address];
            break;
        case 2:
            result |= memory[address];
            result |= memory[address + 1] << 8;
            break;
        case 3:
            result |= memory[address];
            result |= memory[address + 1] << 8;
            result |= memory[address + 2] << 16;
            break;
        case 4:
            result |= memory[address];
            result |= memory[address + 1] << 8;
            result |= memory[address + 2] << 16;
            result |= memory[address + 3] << 24;
            break;

        default:
            break;
    }

    return result;
}

void MMU::__write_noprotect(dword nbytes, dword address, dword data) {
    address &= 0xFFFFFF;
    switch(nbytes) {
        case 1:
            memory[address + 0] = (data >>  0) & 0xFF;
            break;
        case 2:
            memory[address + 0] = (data >>  0) & 0xFF;
            memory[address + 1] = (data >>  8) & 0xFF;
            break;
        case 3:
            memory[address + 0] = (data >>  0) & 0xFF;
            memory[address + 1] = (data >>  8) & 0xFF;
            memory[address + 2] = (data >> 16) & 0xFF;
            break;
        case 4:
            memory[address + 0] = (data >>  0) & 0xFF;
            memory[address + 1] = (data >>  8) & 0xFF;
            memory[address + 2] = (data >> 16) & 0xFF;
            memory[address + 3] = (data >> 24) & 0xFF;
            break;
    }
}

int MMU::loadBIOS(std::string fpath) {
    std::filesystem::path path{fpath};
    dword fileSize = std::filesystem::file_size(path);

    if(fileSize > 0x1000) {
        log(LOG_ERROR, "BIOS File " + fpath + " is too large to load - Can have maximum size of 4KiB.");
        return -1;
    }

    byte data = 0;
    dword address = 0;
    std::ifstream biosFile(fpath, std::ios::binary | std::ios::in);

    if(!biosFile.is_open()) {
        log(LOG_ERROR, "Could not load BIOS File " + fpath + ".");
        return -1;
    }

    while(address < fileSize) {
        biosFile.read((char*)&data, 1);
        __write_noprotect(1, address, data);

        address++;
    }

    log(LOG_INFO, "BIOS File was loaded successfully.");
    return 0;
}