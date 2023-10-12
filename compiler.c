#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.c"


FILE* read_file(char* filename){
  FILE* file = fopen(filename, "r");
  if(file == NULL){
    printf("Error: File not found\n");
    exit(1);
  }
  return file;
}

int main(int argc, char* argv){
  
  FILE* file = read_file("test.txt");
  // int *position = 0;

  while (file != EOF){
    // Token token = get_token(file, &position);
    Token token = get_token(file);
    printf("Token: %s\n", token.string_value);
  }
  
  return 0;
}