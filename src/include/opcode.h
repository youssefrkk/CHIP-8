#pragma once

#include <cstdint>

class CHIP8;

class opcodeHandler {
public:
    void execOpcodes(uint16_t, CHIP8&);
    
    uint16_t lastOpcode;
};
