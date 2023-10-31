#include <iostream>
#include <cstdint>

#include "include/opcode.h"
#include "include/chip8.h"

void opcodeHandler::execOpcodes(uint16_t currentOpcode, CHIP8& chip8opc) {

    #define X ((currentOpcode >> 8) & 0x000F)
    #define Y ((currentOpcode >> 4) & 0x000F)
    #define N (currentOpcode & 0x000F)
    #define KK (currentOpcode & 0x00FF)
    #define NNN (currentOpcode & 0x0FFF)

    // Compare first character of opcode by masking all other values
    switch(currentOpcode & 0xF000) {
        // All the opcodes that start with 0.
        case 0x0000: {
            // We don't have to compare the 3rd character of opcode because
            // all of the opcodes that start with 0 have an "E" at that location.
            switch(currentOpcode & 0x000F) {
                // If final character of opcode is 0,
                // opc: 00E0 - CLS (clear screen).
                case 0x0000: {
                    for (int i = 0; i < 2048; i++) {
                        chip8opc.gfx[i] = 0;
                    }
                    drawFlag = true;
                    break;
                }

                // If final character of opcode is E,
                // opc: 00EE - RET (return from subroutine). 
                case 0x000E: {
                    chip8opc.progCntr = chip8opc.stack[chip8opc.stackPtr];
                    chip8opc.stackPtr--;
                    break;
                }
            }
            break;
        }

        // opc: 1NNN - JP (jump to address).
        case 0x1000: {
            chip8opc.progCntr = NNN;
            break;
        }

        // opc: 2NNN - CALL (call subroutine at NNN).
        case 0x2000: {
            chip8opc.stackPtr++;
            chip8opc.stack[chip8opc.stackPtr] = chip8opc.progCntr;
            chip8opc.progCntr = NNN;
            break;
        }

        // opc: 3XKK - SE Vx, KK (Skip next instruction if Vx == KK)
        case 0x3000: {
            if (chip8opc.V[X] == KK) {
                chip8opc.progCntr += 2;
            }
            break;
        }

        // opc: 4XKK - SNE Vx, KK (Skip next instruction if Vx != KK)
        case 0x4000: {
            if (chip8opc.V[X] != KK) {
                chip8opc.progCntr += 2;
            }
            break;
        }

        // opc: 5xy0 - SE Vx, Vy (Skip next instruction if Vx == Vy)
        case 0x5000: {
            if (chip8opc.V[X] == chip8opc.V[Y]) {
                chip8opc.progCntr += 2;
            }
            break;
        }

        // opc: 6XKK - LD Vx, KK (Set Vx = KK)
        case 0x6000: {
            chip8opc.V[X] = KK;
            break;
        }

        // opc: 7XKK - ADD Vx, KK (Set Vx = Vx + KK)
        case 0x7000: {
            chip8opc.V[X] = chip8opc.V[X] + KK;
            break;
        }

        // All opcodes starting with 8
        case 0x8000: {
            switch(currentOpcode & 0x000F) {
                // opc: 8XY0 - LD Vx, Vy (Store value of Vy in Vx)
                case 0x0000: {              // NOT WORKING
                    chip8opc.V[X] = chip8opc.V[Y];
                    break;
                }

                // opc: 8XY1 - OR Vx, Vy (OR Vx and Vy, then store the result in Vx)
                case 0x0001: {
                    chip8opc.V[X] = chip8opc.V[X] | chip8opc.V[Y];
                    break;
                }

                // opc: 8XY2 - AND Vx, Vy (Set Vx = Vx AND (bitwise) Vy)
                case 0x0002: {
                    chip8opc.V[X] = chip8opc.V[X] & chip8opc.V[Y];
                    break;
                }

                // opc: 8XY3 - XOR Vx, Vy (Set Vx = Vx XOR Vy)
                case 0x0003: {
                    chip8opc.V[X] = chip8opc.V[X] ^ chip8opc.V[Y];
                    break;
                }

                // opc: 8XY4 - ADD Vx, Vy (Set Vx = Vx + Vy, set VF = 1 IF result is bigger than 8 bits.)
                case 0x0004: {
                    uint16_t result = chip8opc.V[X] + chip8opc.V[Y];
                    // If result is bigger than one byte, set VF to 1, else 0.
                    result > 255 ? chip8opc.V[0xF] = 0x1 : chip8opc.V[0xF] = 0x0;
                    // Mask upper 8 bits of 16 bit integer to make it fit into 8-bit Vx register.
                    chip8opc.V[X] = result&0x00FF;
                    break;
                }

                // opc: 8XY5 - SUB Vx, Vy (Set Vx = Vx - Vy, set VF = NOT borrow.)
                case 0x0005: {
                    // If Vx is bigger than Vy, set VF to 1, else 0.
                    chip8opc.V[X] > chip8opc.V[Y] ? chip8opc.V[0xF] = 0x1 : chip8opc.V[0xF] = 0x0;
                    chip8opc.V[X] = chip8opc.V[X] - chip8opc.V[Y];
                    break;
                }

                // opc: 8XY6 - SHR Vx {, Vy} (Set Vx = Vx SHR 1.)
                case 0x0006: {                      // NOT WORKING
                    chip8opc.V[X] = chip8opc.V[X] >> 7;
                    break;
                }

                // opc: 8XY7 - SUBN Vx, Vy (Set Vx = Vy - Vx, set VF = NOT borrow.)
                case 0x0007: {
                    chip8opc.V[Y] > chip8opc.V[X] ? chip8opc.V[0xF] = 0x1 : chip8opc.V[0xF] = 0x0;
                    chip8opc.V[X] = chip8opc.V[Y] - chip8opc.V[X];
                    break;
                } 

                // opc: 8XYE - SHL Vx {, Vy} (Set Vx = Vx SHL 1.)
                case 0x000E: {
                    
                    break;
                }   
            }
            break;
        }

        // opc: 9XY0 - SNE Vx, Vy (Skip next instruction if Vx != Vy.)
        case 0x9000: {
            if (chip8opc.V[X] != chip8opc.V[Y]) {
                chip8opc.progCntr += 2;
            }
            break;
        }

        // opc: ANNN - LD I, NNN (set index (I) register to value of NNN)
        case 0xA000: {
            chip8opc.I = NNN;
            break;
        }
        
        // opc: BNNN -JP V0, addr (Jump to location nnn + V0.)
        case 0xB000: {
            chip8opc.progCntr = NNN + chip8opc.V[0];
            break;
        }

        // opc: DXYN - DRW Vx, Vy, byte (Display N byte starting at memory location I  at (Vx, Vy), set VF = collision)
        case 0xD000: {
            // X and Y positions = V[x] modulo 64 and 32 respectively.
            uint8_t xPos = chip8opc.V[X]&63;
            uint8_t yPos = chip8opc.V[Y]&31;

            // Get the sprite height
            uint8_t sprHeight = N;

            // Reset VF to 0 to prepare for pixel flips.
            chip8opc.V[0xF] = 0;

            for (uint8_t pass = 0; pass < sprHeight; pass++) {
                // Current sprite (byte) = I register value incremented every loop.
                uint16_t currentByte = chip8opc.I + pass;
                uint8_t byteToDraw = chip8opc.memory[currentByte];

                for (uint8_t i = 0; i < 8; i++) {
                    // Check if pixels go off edge of the screen.
                    // If pixels clip, wrap back around.
                    while (yPos + pass > 32) {
                        yPos = yPos - 32;
                    }
                    while (xPos + pass > 64) {
                        xPos = xPos - 64;
                    }
                    
                    uint8_t currentPxl = (byteToDraw >> 7- i) & 0x1;                   
                    uint16_t gfxPxl = ((xPos + i) + ((yPos + pass) * 64));

                    if (currentPxl == 0x0) { }
                    else {
                         if (chip8opc.gfx[gfxPxl] == 0x0) { chip8opc.gfx[gfxPxl] = 0xFFFFFFFF;}
                         else { chip8opc.gfx[gfxPxl] == 0x0; chip8opc.V[0xF] = 0x1; }
                    }
				}									
            }
            drawFlag = true;
            break;
        }

        default: {
            printf("[ERR] Unkown opcode: %04X, exiting\n", currentOpcode);
            //exit(-1); EXIT ERROR DISABLED
        }
    }
    
    if (debug && currentOpcode != lastOpcode) { printf("[INFO] Executed opcode: %04X\n", currentOpcode); }
    lastOpcode = currentOpcode;
}