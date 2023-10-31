#include <SDL2/SDL.h>

#pragma once

class CHIP8;

class Window {
public:
    void initialize();
    void render(CHIP8& ref);
    void quit();

    SDL_Window* mainWnd;
    SDL_Renderer *wndRenderer;
    SDL_Texture* wndTexture;
    SDL_Event wndEvent;

    uint8_t scale = 8;

    bool closed = false;
};