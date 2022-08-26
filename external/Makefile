# put the libglfw3 and libbox2d in the lib folder (in SC)
# the build will be kept in lib folder (in SC) after it is built

CXX = clang++

SRC = $(wildcard imgui/*.cpp)
SRC_YAML = $(wildcard yaml-cpp/src/*.cpp) $(wildcard yaml-cpp/src/contrib/*.cpp)
OBJ = $(addsuffix .o, $(basename $(notdir $(SRC)))) windowing.o graphics.o
OBJ_YAML = $(addsuffix .o, $(basename $(notdir $(SRC_YAML))))

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

build: libimgui.$(EXT) libyaml-cpp.$(EXT)

libyaml-cpp.$(EXT): $(OBJ_YAML)
	@$(CXX) -shared -o$@ $^ -std=c++11 -g3
	@printf "\e[33mCompiled \e[32mYaml-Cpp \e[35m$@\e[0m\n"

%.o: yaml-cpp/src/%.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iyaml-cpp/include/yaml-cpp
	@printf "\e[33mCompiled\e[34m $<\e[0m\n"

%.o: yaml-cpp/src/contrib/%.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iyaml-cpp/include/yaml-cpp
	@printf "\e[33mCompiled\e[34m $<\e[0m\n"

%.o: yaml-cpp/src/%.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3
	@printf "\e[33mCompiled\e[34m $<\e[0m\n"

%.o: imgui/%.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3
	@printf "\e[33mCompiled\e[34m $<\e[0m\n"

windowing.o: imgui/backends/imgui_impl_glfw.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iimgui
	@printf "\e[33mCompiled\e[34m $< \e[32mwindowing backend\e[0m\n"

graphics.o: imgui/backends/imgui_impl_opengl3.cpp
	@$(CXX) -c $< -std=c++11 -o $@ -g3 -Iimgui
	@printf "\e[33mCompiled\e[34m $< \e[32mgraphics backend\e[0m\n"

libimgui.$(EXT): $(OBJ)
	@$(CXX) -shared -o$@ $^ -std=c++11 -lglfw -g3
	@printf "\e[33mCompiled\e[32m ImGui \e[35m$@\e[0m\n"

clean:
	@rm -f *.o *.dylib

MT:
	@make -j4
