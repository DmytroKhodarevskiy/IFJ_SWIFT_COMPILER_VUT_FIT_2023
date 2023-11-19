# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -lm -fcommon

# Source file
PROG = compiler.c

# Output binary
OUTPUT = compiler.exe

# Test file
TEST = tests/test_expression_parse.c

# Test output
TEST_OUTPUT = test_expression_parse.exe

.PHONY: all clean

# Default target (build)
# all: $(OUTPUT)

# Compile the program
$(OUTPUT): $(PROG)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(PROG)
	./$(OUTPUT)

# Clean up generated files
clean:
	rm -f $(OUTPUT)

# Run the program with an output file
run: $(OUTPUT)
	./$(OUTPUT) output.txt

test:
	$(CC) -o $(TEST_OUTPUT) $(TEST)
	./$(TEST_OUTPUT)
	rm -f $(TEST_OUTPUT)
