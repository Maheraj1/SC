# put the libglfw3 and libbox2d in the lib folder (in SC)
# the build will be kept in lib folder (in SC) after it is built

CXX = clang++

SRC = $(wildcard imgui/*.cpp)
OBJ = $(addsuffix .o, $(basename $(notdir $(SRC)))) windowing.o graphics.o

SRC_COUNT = $(words $(SRC))

EXT = dylib
UNAME_S := $(shell uname)
WINDOW = imgui/backends/imgui_impl_glfw.cpp
RENDERER = imgui/backends/imgui_impl_opengl3.cpp
_OS = 

ifeq ($(UNAME_S), Darwin)
EXT = dylib
CXX = clang++
_OS = MacOS
endif

ifeq ($(UNAME_S), linux)
EXT = so
CXX = g++
_OS = Linux
endif

ifeq ($(OS), Windows_NT)
EXT = dll
CXX = g++
_OS = WIN
endif

.PHONY: build clean
	@mv libimgui.$(EXT) ../SC/lib/$(OS)/x68_64/libimgui.$(EXT)

build: libimgui.$(EXT)

%.o: imgui/%.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3
	@printf "\e[32mCompiled\e[33m $<\e[0m\n"

windowing.o: imgui/backends/imgui_impl_glfw.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iimgui
	@printf "\e[32mCompiled\e[33m $< \e[32mwindowing backend\e[0m\n"

graphics.o: imgui/backends/imgui_impl_opengl3.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iimgui
	@printf "\e[32mCompiled\e[33m $< \e[32mgraphics backend\e[0m\n"

libimgui.$(EXT): $(OBJ)
	@$(CXX) -shared -o$@ $^ -std=c++11 -lglfw -g3
	@printf "\e[35mCompiled ImGui \e[33m$@\e[0m\n"

clean:
	@rm *.o

MT:
	@make -j4
