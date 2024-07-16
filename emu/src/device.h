#pragma once
#include "global.h"

#define SIG_NODEVICE 0
#define SIG_KBDEVICE 1
#define SIG_DSDEVICE 2

class Device {
    public:
        byte signature;
        dword address;

        void write(dword nbytes, dword address, dword data);
        dword read(dword nbytes, dword address);
};