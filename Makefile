CC=g++
CFLAGS =-m64 -g -O0 -Wextra -lSDL2 -lSDL2main -lSDL2_image -o chip8

IMGUI +=libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp
IMGUI +=libs/imgui/imgui_impl_sdl.cpp libs/imgui/imgui_impl_sdlrenderer.cpp
IMGUI +=libs/imgui/imgui_widgets.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_demo.cpp
IMGUI +=libs/ImGuiFileDialog/ImGuiFileDialog.cpp

CHIP8: src/main.cpp src/chip8.cpp src/opcode.cpp src/window.cpp
	$(CC) $(IMGUI) src/main.cpp src/chip8.cpp src/opcode.cpp src/window.cpp $(CFLAGS)