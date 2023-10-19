#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.c"

FILE* read_file(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error: File not found\n");
    exit(1);
  }
  return file;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <output_file>\n", argv[0]);
    return 1;
  }

  FILE* file = read_file("test.txt");
  TokenStack tokenStack;
  initializeStack(&tokenStack);

  while (true) {
    if (feof(file)) {
      break;
    }

    Token token = get_token(file);
    push(&tokenStack, token);
  }

  FILE* outputFile = fopen(argv[1], "w");
  if (outputFile == NULL) {
    printf("Error: Unable to open output file\n");
    return 1;
  }

  for (int i = 0; i <= tokenStack.top; i++) {
    fprintf(outputFile, "StackToken %d type: %s, Token value: %s\n", i, tokenTypeNames[tokenStack.items[i].token_type], tokenStack.items[i].string_value->str);
    printf("StackToken %d type: %s, Token value: %s\n", i, tokenTypeNames[tokenStack.items[i].token_type], tokenStack.items[i].string_value->str);
  }

  fclose(outputFile);
  fclose(file);

  return 0;
}
