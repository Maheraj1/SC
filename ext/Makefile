# To Build ImGui

RENDERER ?= opengl

SRC = $(wildcard imgui-core/*.cpp)

SRC += $(wildcard imgui-glfw-opengl/*.cpp)

FLAGS = -std=c++20 -O3
CXX_FLAGS = -DGLFW_INCLUDE_NONE -Iinclude/imgui -Iinclude/glfw

OBJ = $(addsuffix .o, $(basename $(notdir $(SRC))))

libimgui-glfw-opengl.dylib:
	@clang++ $(SRC) -o $@ -lglfw $(FLAGS) -shared -fPIC $(CXX_FLAGS)
	@cp $@ ../SCEditor/$@
	@cp $@ ../SCEditor/bin/$@

libimgui-glfw-metal.dylib:
	@clang++ $(SRC) -o $@ -lglfw $(FLAGS) -shared -fPIC $(CXX_FLAGS)
	@cp $@ ../SCEditor/$@
	@cp $@ ../SCEditor/bin/$@

clean:
	@rm -f libimgui-glfw-opengl.dylib libimgui-glfw-metal.dylib