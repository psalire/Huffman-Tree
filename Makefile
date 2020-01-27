FLAGS = -Wall -Wextra -Wpedantic -Iinclude
EXECS = encode decode
OBJECTS = binaryTree.o huffman.o minheap.o stack.o
CC = gcc

.PHONY: all, clean
all: $(EXECS)

### Executables ###
encode: $(OBJECTS) encode.o
	$(CC) $(FLAGS) $(OBJECTS) encode.o -o $@

decode: $(OBJECTS) decode.o
	$(CC) $(FLAGS) $(OBJECTS) decode.o -o $@

### Objects ###
%.o: %.c
	$(CC) $(FLAGS) -c $<

### Clean ###
clean:
	rm -f $(EXECS) $(OBJECTS) encode.o decode.o
