#include <cstdint>

#pragma once


extern bool debug;
extern bool drawFlag;

class CHIP8 {
public:
    /*                  
        Reset all registers and initialize CHIP-8.
    */
    void initialize();
    uint16_t progCntr;
    uint8_t stackPtr;
    uint8_t keypad[16]; // All 16 keys that CHIP-8 can handle 
    uint8_t V[16]; uint16_t stack[12], I ; // All 16 8-bit registers, stack, and 16-bit register for r and w from RAM
    uint8_t delayTmr, soundTmr; // Delay and sound timers
    uint8_t memory[4096]; // 4096 B (4KB) of memory
    uint32_t gfx[64*32]; // Screen is 2048 pixels total (64x32)

    uint8_t fontset[80] = { // CHIP-8 fontset
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    /*
        Initialize end.
    */



   /*
        Load ROM into memory.
   */
    void loadROM();
    std::string romPath;
    FILE* rom;
    uint16_t romSize;
    char* romBuffer;
    uint16_t romSpace; // Space left for ROM after initialization
    /*
        Load ROM end.
    */



    /* 
        Emulate CHIP-8 CPU cycle.
    */
    void cycle();
    uint16_t opcode;
    uint32_t pixels[2048]; // Temp video buffer for drawing.
    /* 
        Cycle end.
    */
};