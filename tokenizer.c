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

  if (token_string == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  while (true){

    char symbol = fgetc(file); //get next character

    appendToDynamicString(&token_string, symbol);
    
    if (symbol == EOF){
      Token token;

      printf("%s\n", token_string);

      if (token_string[0] == '\0') {
        token.token_type = T_EOF;
      } else {
        token.token_type = T_ERR;
      }

      return token;

    }
    
    // compare_token(token_string); //returns token_type
    if (strcmp (token_string, "a = 5") == 0)
    {
      Token token;

      token.token_type = T_ASSIGN;
      token.string_value = "";
      token.int_value = 0;

      token.token_type = T_ASSIGN;

      char* string;
      convert_from_dynamic_string(token_string, &string);
      token.string_value = string;

      // printf("%s\n", string);
      printf("YA PIDORAS\n");
      printf("%s\n", token.string_value);
      printf("MNE PIZDEC\n");

      return token;
    }

    //if token is ready, return it
  }
}
