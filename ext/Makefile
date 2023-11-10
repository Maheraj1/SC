# ImGui

RENDERER ?= opengl

SRC = $(wildcard imgui-core/*.cpp)

FLAGS = -std=c++20 -O3
CXX_FLAGS = -Iinclude/imgui -Iinclude/glfw -Wall -Wformat
LD_FLAGS = -shared -fPIC -dynamiclib -lglfw

OBJ = $(addsuffix .o, $(basename $(notdir $(SRC))))
FRAMEWORKS = -framework Metal -framework MetalKit -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore 

imgui_impl_metal.o:
	@clang++ imgui-glfw-metal/imgui_impl_metal.mm -c -ObjC++ -fobjc-weak -fobjc-arc $(FLAGS) $(CXX_FLAGS) -o $@

%.o: imgui-core/%.cpp
	@clang++ $< -c $(FLAGS) $(CXX_FLAGS) -o $@

libimgui-glfw-opengl.dylib:
	@clang++ $(SRC) imgui-glfw-opengl/*.cpp -o $@ $(FLAGS) $(CXX_FLAGS) $(LD_FLAGS) -DGLFW_INCLUDE_NONE 
	@cp $@ ../SCEditor/$@
	@cp $@ ../SCEditor/bin/$@

libimgui-glfw-metal.dylib: $(OBJ) imgui_impl_metal.o
	@clang++ $^ -o $@ $(FLAGS) $(LD_FLAGS) $(FRAMEWORKS)
	@cp $@ ../SCEditor/$@
	@cp $@ ../SCEditor/bin/$@

# ImGuizmo

_SRC += $(wildcard imguizmo/*.cpp)

_FLAGS = -std=c++20 -O3
_CXX_FLAGS = -DGLFW_INCLUDE_NONE -Iinclude/imgui -L. -limgui-glfw-metal

libimguizmo.dylib:
	@clang++ $(_SRC) -o $@ $(_FLAGS) $(_CXX_FLAGS) $(LD_FLAGS)
	@cp $@ ../SCEditor/$@
	@cp $@ ../SCEditor/bin/$@

clean:
	@rm -f libimgui-glfw-opengl.dylib libimgui-glfw-metal.dylib libimguizmo.dylib *.o