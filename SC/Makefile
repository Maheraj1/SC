# We have to use static version of glfw and dynamic version of yaml-cpp, ImGui and Box2D
# Run the Makefile in extrnal folder to compile ImGui with right configrations it will move the dyanmic libs to lib folder of SC and put the dyanlic lib in bin folder also
# Change this accoding to your choice
COMPILER = clang++
LIBS = -lglfw -lyaml-cpp -lImGUI -lbox2d
RENDERER = OpenGL
FRAMEWORKS = 
OTHER_ARGS = -g3
VERSION = -std=c++17
TYPE ?= 0
TARGET = 0
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/**/*.cpp)
DEFINES = -DSC_RENDERER_$(RENDERER) -DGLFW_INCLUDE_NONE -DSC_WINDOW_API_GLFW -DSC_USE__asm__
INCLUDE_DIR = -Iinclude -I../external/glm -I../external/imgui -I../external/imgui/backends -I../external/box2d -I../extrenal
LIB_DIR = -Llib/MacOS/x86_64
OBJ_NAME = libSC
OBJ_PATH = $(OBJ_NAME)
SRC_COUNT = $(words $(SRC))
BUILD_TYPE = Debug
OBJS = $(addsuffix .o, $(basename $(notdir $(SRC))))

ifneq ($(OS), Windows_NT)
	FRAMEWORKS += -framework IOKit -framework CoreVideo -framework Cocoa
	ifeq ($(RENDERER), OpenGL)
		FRAMEWORKS += -framework OpenGL
		SRC += $(wildcard $(SRC_DIR)/Window/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Renderer/OpenGL/*.cpp)
		SRC += $(wildcard $(SRC_DIR)/Input/OpenGL/*.cpp)
	endif
	#TODO Add Metal Spport
	ifeq ($(RENDERER), Metal)
		FRAMEWORKS += -framework Metal
		DEFINES += -DGLFW_EXPOSE_NATIVE_COCOA
	endif
else
	LIBS += -lopengl32 -luser32 -lgdi32 -lshell32 -lkernel32
	
endif


ifeq ($(TYPE), 0)
	BUILD_TYPE = Debug
	OTHER_ARGS = -g3
	DEFINES += -DSC_DEBUG
endif
ifeq ($(TYPE), 1)
	BUILD_TYPE = Release
	OTHER_ARGS = -O3
endif

%.o: src/Window/OpenGL/%.cpp
	@$(COMPILER) -c -o $@ $< $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

%.o: src/Renderer/OpenGL/%.cpp
	@$(COMPILER) -c -o $@ $< $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

%.o: src/Input/OpenGL/%.cpp
	@$(COMPILER) -c -o $@ $< $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"

%.o: src/**/%.cpp
	@$(COMPILER) -c -o $@ $< $(DEFINES) $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR)
	@printf "\e[33mCompiled $@: \e[34m$<\e[0m\n"


build: all DL
	@printf "\n\e[32mDone Building SC (Renderer: $(RENDERER))\e[0m\n"

all: base
	
base:
	@printf "\e[36m<------------------------------------------- Output ------------------------------------------->\e[0m\n\n"
	@printf "\e[32mBuilding \e[35m$(SRC_COUNT) files\e[32m on\e[36m $(BUILD_TYPE)\e[32m Configration\e[0m\n\n"

DL: $(OBJS)
	@$(COMPILER) -shared $^ $(DEFINES) -DSC_SHARED $(VERSION) $(OTHER_ARGS) $(INCLUDE_DIR) -o $(OBJ_PATH).dylib $(LIB_DIR) $(FRAMEWORKS) $(LIBS) -fPIC -Wall -Winfo -Werror
	@printf "\n\e[35mBuilt $(SRC_COUNT) Sources files\e[0m\n"

	@mv $(OBJ_PATH).dylib bin/$(OBJ_PATH).dylib

	@cp bin/$(OBJ_PATH).dylib ../SandBox/lib/$(OBJ_NAME).dylib
	@cp bin/$(OBJ_PATH).dylib ../SandBox/bin/$(OBJ_NAME).dylib

clean:
	@rm -f *.o $(OBJ_PATH).dylib $(OBJ_NAME).dylib ../SandBox/lib/$(OBJ_NAME).dylib ../SandBox/bin/$(OBJ_NAME).dylib