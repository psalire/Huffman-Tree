FLAGS = -Wall -Wextra -Wpedantic -Iinclude -Isrc
EXECS = encode decode
BIN = bin/
OBJECTS = $(BIN)binaryTree.o $(BIN)huffman.o $(BIN)minheap.o $(BIN)stack.o
CC = gcc

.PHONY: all, clean
all: $(EXECS)

### Executables ###
encode: $(OBJECTS) $(BIN)encode.o
	$(CC) $(FLAGS) $(OBJECTS) $(BIN)encode.o -o $@

decode: $(OBJECTS) $(BIN)decode.o
	$(CC) $(FLAGS) $(OBJECTS) $(BIN)decode.o -o $@

### Objects ###
bin/%.o: src/%.c
	$(CC) $(FLAGS) -c $<
	@mv *.o $(BIN)

### Clean ###
clean:
	rm -f $(EXECS) $(wildcard bin/*.o)
