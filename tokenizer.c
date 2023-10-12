#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "dynamic_string.c"

// Token get_token(FILE* file){
// Token get_token(char *file, int* position){
Token get_token(FILE *file){

  // char* token = malloc(sizeof(char) * 100);
  char *token_string = createDynamicString();

  while (true){

    char symbol = fgetc(file); //get next character

    if (symbol == EOF){
      Token token;
      token.token_type = T_EOF;
      token.string_value = token_string;
      return token;
    }
    

    if (token_string != NULL) {
      appendToDynamicString(&token_string, symbol);
    }

    // position++; //increment position

    // compare_token(token_string); //returns token_type
    if (token_string == "a = 5")
    {
      Token token;
      token.token_type = T_ASSIGN;
      token.string_value = token_string;
      return token;
    }

    // position = //position after reading token
    //if token is ready, return it
  }
}
