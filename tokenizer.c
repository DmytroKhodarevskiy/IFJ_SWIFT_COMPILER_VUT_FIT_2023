#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "dynamic_string.c"

#define START 100
#define EOF_STATE 101


void copyString(char *destination, char *source) {
  strcpy(destination, source);
  free(source);
}




Token init_token(){
  Token token;

  token.token_type = T_EMPTY;
  token.int_value = 0;
  token.double_value = 0.0;
  token.string_value = createDynamicString();

  return token;
}

Token get_token(FILE *file){
  Dynamic_string *token_string = createDynamicString();

  int state = START;
  Token token = init_token();

  while (true) {
    char symbol = (char)fgetc(file);
    appendToDynamicString(token_string, symbol);

    switch (state) {
    case START:
      if (symbol == '\n' || symbol == EOF) {
        state = EOF_STATE;
      } else if (symbol == '+') {
        token.token_type = T_PLUS;
        //strcpy(token.string_value->str, token_string->str);
        //dynamic_string_free(token_string);
        copyString(token.string_value->str, token_string->str);
        return token;
      } else if (symbol == '-') {
        token.token_type = T_MINUS;
        copyString(token.string_value->str, token_string->str);
        return token;
      } else if (symbol == '*') {
        token.token_type = T_MULTIPLY;
        copyString(token.string_value->str, token_string->str);
        return token;
      }
      break;




    case EOF_STATE:
      if (symbol == EOF) {
        token.token_type = T_EOF;
        return token;
      } else {
        ungetc(symbol, file);
        token.token_type = T_ERR;
        return token;
      }
      break;
    }
  }
}