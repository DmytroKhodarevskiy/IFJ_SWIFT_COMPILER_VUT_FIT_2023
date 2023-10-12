#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "dynamic_string.c"

// Token get_token(FILE* file){
// Token get_token(char *file, int* position){

Token init_token(){
  Token token;

  token.token_type = T_EMPTY;
  token.string_value = " ";
  token.int_value = 0;
  token.double_value = 0.0;

  return token;
}

Token get_token(FILE *file){

  // char* token = malloc(sizeof(char) * 100);
  char *token_string = createDynamicString();

  if (token_string == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  while (true){

    // printf("ya gay\n");

    char symbol = fgetc(file); //get next character

    appendToDynamicString(&token_string, symbol);
    printf("token_string: %s\n", token_string);
    
    if (symbol == EOF){
      Token token = init_token();

      printf("%s\n", token_string);

      if (token_string[0] == '\0') {
        token.token_type = T_EOF;
      } else {
        token.token_type = T_ERR;
      }

      // token.string_value = " ";

      return token;

    }
    
    // compare_token(token_string); //returns token_type
    if (strcmp (token_string, "a = 5") == 0)
    {
      Token token = init_token();

      token.token_type = T_ASSIGN;

      //char* string;
      // printf("%s\n", token_string);

      char *string;
      strcpy(string, token_string);
      token.string_value = string;

      // printf("YA PIDORAS\n");
      // printf("MNE PIZDEC\n");

      free(token_string);

      return token;
    }

    //if token is ready, return it
  }
}
