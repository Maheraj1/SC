COMPILER = clang++
RENDERER = OpenGL

OBJ_NAME = SCEditor
OBJ_PATH = bin/$(OBJ_NAME)_

SRC = $(wildcard src/*.cpp)
INCLUDE_DIR = -Iinclude -I../external/glm -I../external/imgui -I../external/imgui/backends -I../external/box2d -I../extrenal -I../extrenal/yaml-cpp/include -I../SC/include
OBJ = $(addsuffix .o, $(notdir $(basename $(SRC))))
DEFINES = -DSC_RENDERER_$(RENDERER) -DGLFW_INCLUDE_NONE -DSC_WINDOW_API_GLFW -DSC_USE__asm__ -DSC_EDITOR_IMPL -DSC_DEBUG
VERSION = -std=c++20
OTHER_ARGS = -g3 -Wall -Wno-undefined-var-template
LIBS = -lSC -lSCImGui
LIB_DIR = -L.

%.o: src/%.cpp
	@$(COMPILER) -o $@ $< -c $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

build: $(OBJ)
	@$(COMPILER) -o $(OBJ_PATH) $^ -g3 $(LIBS) $(LIB_DIR)
	@printf "\e[32mBuilt $(OBJ_NAME)\e[0m\n"

clean: 
	@rm -f *.o $(OBJ_PATH)