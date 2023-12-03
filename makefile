# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -lm -fcommon

# Source file
PROG = compiler.c

#Source files
SOURCES = parse.c symtable_stack.c expression_parse.c symtable.c stack.c tokenizer.c dynamic_string.c memory.c

# Output binary
OUTPUT = compiler.exe

# Test file
TEST = tests/test_expression_parse.c

# Test output
TEST_OUTPUT = test_expression_parse.exe

# .PHONY: all clean

# Default target (build)
# all: $(OUTPUT)

# Compile the program
$(OUTPUT): $(PROG)
# $(CC) $(CFLAGS) -o $(OUTPUT) $(PROG) $(SOURCES)
	$(CC) -o $(OUTPUT) $(PROG) $(SOURCES)
# ./$(OUTPUT) test.txt

# Clean up generated files
clean:
	rm -f $(OUTPUT)

# Run the program with an output file
run: $(OUTPUT)
	./$(OUTPUT) test.txt

valgrind: $(OUTPUT)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(OUTPUT) test.txt

test:
	$(CC) -o $(TEST_OUTPUT) $(TEST)
	./$(TEST_OUTPUT)
	rm -f $(TEST_OUTPUT)
