#pragma once

#include <cstdint>
#include <string>
#include <iostream>

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef enum {
    LOG_INFO,
    LOG_ERROR,
} logType;

static inline std::string logTypeAsString(logType t) {
    switch(t) {
        case LOG_INFO:
            return "INFO ";
        case LOG_ERROR:
            return "ERROR";
        default:
            return "LOG  ";
    }
}

static inline void log(logType type, std::string msg) {
    std::cout << "[" << logTypeAsString(type) << "] " << msg << std::endl;
}