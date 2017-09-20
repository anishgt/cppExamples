CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -pthread
CC_FLAGS := -Wall -Wextra -std=c++11

main: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

CC_FLAGS += -MMD
-include $(OBJFILES:.o=.d)

.PHONY: clean
clean:
	rm -f $(OBJ_FILES) main
