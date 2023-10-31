#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/window.h"
#include "include/chip8.h"

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_sdl.h"
#include "../libs/imgui/imgui_impl_sdlrenderer.h"
#include "../libs/ImGuiFileDialog/ImGuiFileDialog.h"

#undef main

bool _true = true;

void Window::initialize() {
    // Initialize SDL
    if (debug) { printf("Initializing SDL...\n"); }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        if (debug) { printf("SDL initialization failed: %s\n", SDL_GetError()); }
        exit(-1);
    } 
    else if (debug) { printf("SDL initialized successfully.\n\n"); }

    // Create window with title 'CHIP-8'
    mainWnd = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                1024, 512, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    closed = false;
    if (mainWnd == NULL) {
        if (debug) { printf("Failed to create window: %s\n", SDL_GetError()); }
        exit(-1);
    }

    // Create window renderer
    wndRenderer = SDL_CreateRenderer(mainWnd, -1, 0);
    if (wndRenderer == NULL) {
        if (debug) { printf("Failed to create window renderer: %s\n", SDL_GetError()); }
        exit(-1);
    }

    // Create CHIP-8 "texture" (64x32 pixels)
    wndTexture = SDL_CreateTexture(wndRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (wndTexture == NULL) {
        if (debug) { printf("Failed to create window texture: %s\n", SDL_GetError()); }
        exit(-1);
    }

    // Finish up with SDL.
    SDL_SetWindowIcon(mainWnd, IMG_Load("icon.png"));
    SDL_RenderClear(wndRenderer);
    SDL_RenderPresent(wndRenderer);

    // Initialize ImGUI.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.WantCaptureKeyboard = true;
    io.WantCaptureMouse = true;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(mainWnd);
    ImGui_ImplSDLRenderer_Init(wndRenderer);
}



void Window::quit() {
    // Cleanup ImGui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Quit SDL
    SDL_DestroyRenderer(wndRenderer);
    SDL_DestroyWindow(mainWnd);
    SDL_Quit();
    if (debug) { printf("Quiting SDL...\n"); }

    closed = true;
    exit(0);
}

void Window::render(CHIP8& chip8wnd) {
    // ImGui+SDL boilerplate
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(mainWnd);
    ImGui::NewFrame();

    // -----------------
    // IMGUI WINDOWS
    // -----------------

    // Menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open")) {
                ImGui::Begin("Load ROM", NULL); // WINDOW DOES NOT OPEN???
                ImGuiFileDialog::Instance()->Display("Load ROM", ImGuiWindowFlags_NoCollapse, ImVec2(256, 312), ImVec2(512, 636));
                ImGui::End();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Emulator")) {
            if(ImGui::BeginMenu("Scaling")) {
                if(ImGui::MenuItem("2x"))  { scale = 2;  }
                if(ImGui::MenuItem("4x"))  { scale = 4;  }
                if(ImGui::MenuItem("6x"))  { scale = 6;  }
                if(ImGui::MenuItem("8x"))  { scale = 8;  }
                if(ImGui::MenuItem("10x")) { scale = 10; }
                if(ImGui::MenuItem("12x")) { scale = 12; }
                if(ImGui::MenuItem("14x")) { scale = 14; }
                if(ImGui::MenuItem("16x")) { scale = 16; }
                if(ImGui::MenuItem("20x")) { scale = 20; }
                if(ImGui::MenuItem("24x")) { scale = 24; }
                if(ImGui::MenuItem("28x")) { scale = 28; }
                if(ImGui::MenuItem("36x")) { scale = 36; }
                if(ImGui::MenuItem("44x")) { scale = 44; }
                if(ImGui::MenuItem("52x")) { scale = 52; }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();    
    }
    
    // CHIP-8 Display
    ImGui::Begin("CHIP-8 Display", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | 
                                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(64*scale, 32*scale));
    ImGui::SetWindowPos(ImVec2(0,18)); // 0,18 because that's the height of the menu bar (through experimentation)
    ImGui::Image(wndTexture, ImGui::GetContentRegionAvail()); // Put CHIP-8 texture into an ImGui::Image and set the window size to the
                                                              // size of the available content (texture)
    ImGui::End();

    // Stack information
    ////// TODO: IMPLEMENT STACK WINDOW WITH CONSTRAINTS TO OTHER WINDOW
    
    
    ImGui::Render();

    SDL_SetRenderDrawColor(wndRenderer, 128, 128, 128, 255);
    SDL_RenderClear(wndRenderer);
    if (drawFlag) {
        drawFlag = false;
        SDL_UpdateTexture(wndTexture, NULL, chip8wnd.gfx, 64 * 4);
    }
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(wndRenderer);
}