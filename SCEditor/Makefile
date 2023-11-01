include ../ext/ext.mk

COMPILER = clang++
RENDERER = OpenGL

OBJ_NAME = SCEditor
OBJ_PATH = bin/$(OBJ_NAME)_

SRC = $(wildcard src/*.cpp)
INCLUDE_DIR = -Iinclude -I../SC/include $(EXT_CFLAGS)
OBJ = $(addsuffix .o, $(notdir $(basename $(SRC))))
DEFINES = -DSC_RENDERER_$(RENDERER) -DGLFW_INCLUDE_NONE -DSC_WINDOW_API_GLFW -DSC_USE__asm__ -DSC_EDITOR_IMPL -DSC_DEBUG
VERSION = -std=c++20
OTHER_ARGS = -g3 -Wall -Wno-undefined-var-template
LIBS = -lSC -limgui-glfw-opengl -lglfw
LIB_DIR = -L. $(EXT_LDFLAGS)

%.o: src/%.cpp
	@$(COMPILER) -o $@ $< -c $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

build: $(OBJ)
	@$(COMPILER) -o $(OBJ_PATH) $^ $(LIBS) $(LIB_DIR) $(OTHER_ARGS)
	@printf "\e[32mBuilt $(OBJ_NAME)\e[0m\n"

clean: 
	@rm -f *.o $(OBJ_PATH)