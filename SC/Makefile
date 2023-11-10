include ../ext/ext.mk

# We have to use static version of glfw and dynamic version of yaml-cpp, ImGui and Box2D
# Run the Makefile in external folder to compile ImGui with right configrations it will move the dyanmic libs to lib folder of SC and put the dynamic lib in bin folder also
# Change this accoding to your choice
CXX = clang++
# Options: OpenGL, Metal, *DX11
# `*` Not supported
RENDERER = Metal
FRAMEWORKS = 
OTHER_ARGS = -g3
VERSION = -std=c++20
TYPE ?= 0
BUILD_TYPE = Debug
UNAME_S = $(shell uname -s)

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS = $(addsuffix .o, $(basename $(notdir $(SRC))))

WARNS = -Wall -Wno-undefined-var-template
DEFINES = -DSC_RENDERER_$(RENDERER) -DGLFW_INCLUDE_NONE -DSC_WINDOW_API_GLFW -DSC_USE__asm__ -DSC_EDITOR_IMPL -DSC_SHARED -DSC_DEBUG
INCLUDE_DIR = -Iinclude $(EXT_CFLAGS) -I$(MONO_INCLUDE)

LIBS = -lglfw -lyaml-cpp -lglad -lstbi -lbox2d -lmonosgen-2.0
LIB_DIR = -Llib -L$(MONO_LIB)
OBJ_NAME = libSC
OBJ = 
SRC_COUNT = $(words $(SRC))

ifeq ($(UNAME_S), Darwin)
	CXX = clang++
	OBJ = $(OBJ_NAME).dylib
	FRAMEWORKS += -framework IOKit -framework CoreVideo -framework Cocoa
	ifeq ($(RENDERER), OpenGL)
		FRAMEWORKS += -framework OpenGL
		SRC += $(wildcard $(SRC_DIR)/Window/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Renderer/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Input/OpenGL/*.cpp)
	endif

	# TODO Add Metal Spport	
	ifeq ($(RENDERER), Metal)
		FRAMEWORKS += -framework Metal -framework QuartzCore -framework Foundation
		DEFINES += -DGLFW_EXPOSE_NATIVE_COCOA
		SRC += $(wildcard $(SRC_DIR)/Window/Metal/*.mm)
		SRC += $(wildcard $(SRC_DIR)/Renderer/Metal/*.mm)
		SRC += $(wildcard $(SRC_DIR)/Input/Metal/*.mm)
	endif
endif

ifeq ($(OS), Windows_NT)
	CXX = g++
	OBJ = $(OBJ_NAME).dll
	ifeq ($(RENDERER), OpenGL)
		LIBS += -lopengl32 -luser32 -lgdi32 -lshell32 -lkernel32
	endif
endif

CXXFLAGS = $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR) $(WARNS) -c
LDFLAGS  = $(VERSION) $(LIB_DIR) $(FRAMEWORKS) $(LIBS) -fPIC $(WARNS) $(EXT_LDFLAGS)

%.o: src/**/**/%.cpp
	@$(CXX) -o $@ $< $(CXXFLAGS)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"


%.o: src/**/**/%.mm
	@$(CXX) -o $@ $< $(CXXFLAGS) -ObjC++ -fobjc-weak -fobjc-arc
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

%.o: src/**/%.cpp
	@$(CXX) -o $@ $< $(CXXFLAGS)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"


build: all
	@printf "\n\e[32mDone Building SC (Renderer: $(RENDERER))\e[0m\n"

all: base DL
	
base:
	@printf "\e[36m<------------------------------------------- Output ------------------------------------------->\e[0m\n\n"
	@printf "\e[32mBuilding \e[35m$(SRC_COUNT) files\e[32m on\e[36m $(BUILD_TYPE)\e[32m Configration\e[0m\n\n"

DL: $(OBJS)
	@$(CXX) -shared $^ -o $(OBJ) $(LDFLAGS) $(OTHER_ARGS)
	@printf "\n\e[35mBuilt $(SRC_COUNT) Sources files\e[0m\n"

	@mv $(OBJ) bin/$(OBJ)

	@cp bin/$(OBJ) ../SandBox/lib/$(OBJ)
	@cp bin/$(OBJ) ../SandBox/bin/$(OBJ)
	
	@cp bin/$(OBJ) ../SCEditor/bin/$(OBJ)
	@cp bin/$(OBJ) ../SCEditor/$(OBJ)
	@cp bin/$(OBJ) ../SCEditor/libs/$(OBJ)

clean:
	@rm -f *.o $(OBJ) $(OBJ) ../SandBox/lib/$(OBJ) ../SandBox/bin/$(OBJ) ../SCEditor/bin/$(OBJ) ../SCEditor/$(OBJ).dylib ../SCEditor/libs/$(OBJ)