include ../ext/ext.mk

# We have to use static version of glfw and dynamic version of yaml-cpp, ImGui and Box2D
# Run the Makefile in external folder to compile ImGui with right configrations it will move the dyanmic libs to lib folder of SC and put the dynamic lib in bin folder also
# Change this accoding to your choice
CXX = clang++
# Options: OpenGL(Only Supported), Metal, DX11
RENDERER = OpenGL
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
DEFINES = -DSC_RENDERER_$(RENDERER) -DGLFW_INCLUDE_NONE -DSC_WINDOW_API_GLFW -DSC_USE__asm__ -DSC_CORE_IMPL -DSC_SHARED
INCLUDE_DIR = -Iinclude $(EXT_CFLAGS) -I$(MONO_INCLUDE)

LIBS = -lglfw -lyaml-cpp -lImGUI -lbox2d -lmonosgen-2.0
LIB_DIR = -Llib -L$(MONO_LIB)
OBJ_PATH = libSC
SRC_COUNT = $(words $(SRC))

ifeq ($(OS), Windows_NT)
	CXX = g++
	OBJ_PATH += .dll
	ifeq ($(RENDERER), OpenGL)
		LIBS += -lopengl32 -luser32 -lgdi32 -lshell32 -lkernel32
	endif
else ifeq ($(UNAME_S), Darwin)
	CXX = clang++
	OBJ_PATH += .dylib
	FRAMEWORKS += -framework IOKit -framework CoreVideo -framework Cocoa
	ifeq ($(RENDERER), OpenGL)
		FRAMEWORKS += -framework OpenGL
		SRC += $(wildcard $(SRC_DIR)/Window/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Renderer/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Input/OpenGL/*.cpp)
	endif

	# TODO Add Metal Spport	
	ifeq ($(RENDERER), Metal)
		FRAMEWORKS += -framework Metal
		DEFINES += -DGLFW_EXPOSE_NATIVE_COCOA
		SRC += $(wildcard $(SRC_DIR)/Window/Metal/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Renderer/Metal/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Input/Metal/*.cpp)
	endif
endif

CXXFLAGS = $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR) $(WARNS) -c
LDFLAGS  = $(VERSION) $(LIB_DIR) $(FRAMEWORKS) $(LIBS) -fPIC $(WARNS)

%.o: src/**/**/%.cpp
	@$(CXX) -o $@ $< $(CXXFLAGS)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

%.o: src/**/%.cpp
	@$(CXX) -o $@ $< $(CXXFLAGS)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"


build: all DL
	@printf "\n\e[32mDone Building SC (Renderer: $(RENDERER))\e[0m\n"

all: base
	
base:
	@printf "\e[36m<------------------------------------------- Output ------------------------------------------->\e[0m\n\n"
	@printf "\e[32mBuilding \e[35m$(SRC_COUNT) files\e[32m on\e[36m $(BUILD_TYPE)\e[32m Configration\e[0m\n\n"

DL: $(OBJS)
	@$(CXX) -shared $^ -o $(OBJ_PATH)
	@printf "\n\e[35mBuilt $(SRC_COUNT) Sources files\e[0m\n"

	@mv $(OBJ_PATH) bin/$(OBJ_PATH)

	@cp bin/$(OBJ_PATH) ../SandBox/lib/$(OBJ_NAME)
	@cp bin/$(OBJ_PATH) ../SandBox/bin/$(OBJ_NAME)
	
	@cp bin/$(OBJ_PATH) ../SCEditor/bin/$(OBJ_NAME)
	@cp bin/$(OBJ_PATH) ../SCEditor/$(OBJ_NAME)
	@cp bin/$(OBJ_PATH) ../SCEditor/libs/$(OBJ_NAME)

clean:
	@rm -f *.o $(OBJ_PATH) $(OBJ_NAME) ../SandBox/lib/$(OBJ_NAME) ../SandBox/bin/$(OBJ_NAME) ../SCEditor/bin/$(OBJ_NAME) ../SCEditor/$(OBJ_NAME).dylib ../SCEditor/libs/$(OBJ_NAME)