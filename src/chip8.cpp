#include <iostream>
#include <SDL2/SDL.h>

#include "include/chip8.h"
#include "include/opcode.h"

opcodeHandler opcHandler;

bool debug = true; // Debug flag prints out basic debug info.
bool drawFlag = false;

void CHIP8::initialize() {
    progCntr    = 0x200;
    delayTmr    = 0;
    soundTmr    = 0;
    stackPtr    = 0;
    I           = 0;

    // Clear memory
    for (int i = 0; i < 4096; i++) {
        memory[i] = 0;
        if (debug && i == 4095) {
            printf("Emptied all memory addresses. Final memAddr %d: 0x%d\n", i, memory[i]); }
    }

    // Clear registers
    for (int i = 0; i < 16; i++) {
        V[i] = 0;
        // If on the last iteration of loop, empty 16-bit register 'I' and print debug message.
        if (i == 15) {
            I = 0;
            if (debug) {
                printf("Emptied all registers (V[i] & I). Final register V%d: 0x%d. Register I: 0x%d\n", i, V[i], I); }
        }
    }

    // Clear stack
    for (int i = 0; i < 12; i++) {
        stack[i] = 0;
        if (debug && i == 11) {
        printf("Emptied all stack levels. Final level %d: 0x%d\n\n", i, stack[i]); }
    }

    // Clear video buffer
    for (int i = 0; i < 2048; i++) {
        gfx[i] = 0;
    }

    // Load fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}



void CHIP8::loadROM() {
    /* TEMP ROM PATH */ 
                        //romPath = "D:\\Coding\\Projects\\C++\\CHIP-8 Emulator\\roms\\test_opcode.ch8"; 
                        romPath = "/run/media/youssef/1TB/Coding/Projects/C++/CHIP-8 Emulator/roms/test_opcode.ch8";
    /* TEMP ROM PATH*/


    // Add file open command to 'rom' and open it in read-only binary mode
    const char* romPathChar = romPath.c_str(); 
    FILE* rom = fopen(romPathChar, "rb");
    if (rom == NULL) {
        if (debug) { printf("ROM failed to open!\n"); }
        exit(-1);
    }
    
    // Get ROM file size and put it into 'romSize'
    fseek(rom, 0, SEEK_END);
    romSize = ftell(rom);
    rewind(rom);

    // Allocate memory for ROM
    char* romBuffer = (char*)malloc(sizeof(char) *romSize);
    if (romBuffer == NULL) {
        if (debug) { printf("ROM memory allocation failed!\n"); }
        exit(-1);
    } else if (debug) { printf("Allocated %d bytes of memory for ROM.\n", romSize); }

    // Copy ROM into buffer
    int checkROMsize = fread(romBuffer, sizeof(char), (size_t)romSize, rom);
    if (checkROMsize != romSize) {
        if (debug) { printf("Failed to copy ROM into buffer!\n"); }
        exit(-1);
    } else if (debug) { printf("Successfully copied %d bytes into ROM buffer.\n", romSize); }

    // Load ROM in memory (that was just allocated) from 512 (0x200)
    romSpace = 4096-512;
    if (romSize < romSpace) {
        for (int i = 0; i < romSize; i++) { memory[i+512] = (int)romBuffer[i]; } // i+512 because first 512 bytes
        if (debug) { printf("ROM loaded!\n\n"); }                           // of RAM are reserved
    } else {                                                                     
        if (debug) { printf("ROM size too big to fit into memory!\n"); }
        exit(-1);
    }
}



void CHIP8::cycle() {
    // Fetch opcode and increment program counter by 2 to prepare for next instruction.
    opcode = memory[progCntr] << 8 | memory[progCntr + 1];
    progCntr += 2;
    opcHandler.execOpcodes(opcode, *this);
    }