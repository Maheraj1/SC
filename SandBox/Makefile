SRC = $(wildcard src/*.cpp)
OBJ = $(addsuffix .o, $(basename $(notdir $(SRC))))

.PHONY: bin/SandBox

%.o: src/%.cpp
	@clang++ -c -o $@ $< -std=c++17 -I../SC/include -Iinclude -I../external/glm -I../external/box2d/include -g3
	@printf "\e[33mBuilt $@:\e[34m $<\e[0m\n"

bin/SandBox_: $(OBJ)
	@clang++ -o $@ -std=c++17 -Llib -lSC $^ -g3
	@printf "\e[32mBuilt SandBox\e[0m\n"

clean:
	@rm -f bin/SandBox *.o
