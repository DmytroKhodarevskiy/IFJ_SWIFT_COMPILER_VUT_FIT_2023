# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -lm -fcommon

# Source file
PROG = compiler.c

# Output binary
OUTPUT = compiler

.PHONY: all clean

# Default target (build)
all: $(OUTPUT)

# Compile the program
$(OUTPUT): $(PROG)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(PROG)

# Clean up generated files
clean:
	rm -f $(OUTPUT)

# Run the program with an output file
run: $(OUTPUT)
	./$(OUTPUT) output.txt
