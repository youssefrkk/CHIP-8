#include <iostream>
#include <SDL2/SDL.h>

#include "include/chip8.h"
#include "include/window.h"

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_sdl.h"
#include "../libs/imgui/imgui_impl_sdlrenderer.h"

#undef main

CHIP8 chip8;
Window window;

int main(int argc, char* argv[]) {
    // Initialize CHIP-8 and load ROM.
    chip8.initialize();
    chip8.loadROM();

    // Initiazlize SDL window.
    window.initialize();

    // Start main window loop.
    while(window.closed == false) {
        window.render(chip8);
        chip8.cycle();

        while (SDL_PollEvent(&window.wndEvent)) {
            ImGui_ImplSDL2_ProcessEvent(&window.wndEvent);
            
            switch (window.wndEvent.type) {
                case SDL_QUIT: {
                    window.closed = true;
                    break;
                }
            }
        }
    }

    window.quit();
    return 0;
}