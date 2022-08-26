TYPE ?= 0
SRC = $(wildcard src/*.cpp)
OBJ = $(addsuffix .o, $(basename $(notdir $(SRC))))
OTHER_ARGS = -g3

ifeq ($(TYPE), 0)
OTHER_ARGS = -g3
endif
ifeq ($(TYPE), 1)
OTHER_ARGS = -O3
endif

.PHONY: bin/SandBox

%.o: src/%.cpp
	@clang++ -c -o $@ $< -std=c++20 -I../SC/include -Iinclude -I../external/glm -I../external/box2d/include $(OTHER_ARGS)
	@printf "\e[33mBuilt $@:\e[34m $<\e[0m\n"

bin/SandBox_: $(OBJ)
	@clang++ -o $@ -std=c++20 -Llib -lSC $^ $(OTHER_ARGS)
	@printf "\e[32mBuilt SandBox\e[0m\n"

clean:
	@rm -f bin/SandBox *.o
