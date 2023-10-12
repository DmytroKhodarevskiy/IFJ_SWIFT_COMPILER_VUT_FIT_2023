# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -lm -fcommon

# Source file
PROG = compiler.c

# Output binary
OUTPUT = compiler

# Default target (build and run)
all: $(OUTPUT)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(PROG)
	./$(OUTPUT)

# Compile the program
$(OUTPUT): $(PROG)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(PROG)

# Clean up generated files
clean:
	rm -f $(OUTPUT)

.PHONY: all clean
