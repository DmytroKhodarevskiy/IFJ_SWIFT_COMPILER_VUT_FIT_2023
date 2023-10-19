#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "dynamic_string.c"

#include "stack.c"

#define START 100
#define EOF_STATE 101
#define EQUAL_STATE_or_ASSIGN_STATE 102
#define Greater_or_GreaterEqual_STATE 103
#define Less_or_LessEqual_STATE 104


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

    switch (state) {
    case START:
      if (symbol == '\n' || symbol == EOF) {
        state = EOF_STATE;


      } else if (isspace(symbol)) {
        state = START;


      } else if (symbol == '+') {
        token.token_type = T_PLUS;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == '-') {
        token.token_type = T_MINUS;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == '*') {
        token.token_type = T_MULTIPLY;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == '=') {
        appendToDynamicString(token_string, symbol);
        state = EQUAL_STATE_or_ASSIGN_STATE;


      } else if (symbol == '>') {
        appendToDynamicString(token_string, symbol);
        state = Greater_or_GreaterEqual_STATE;


      } else if (symbol == '<') {
        appendToDynamicString(token_string, symbol);
        state = Less_or_LessEqual_STATE;


      } else if (symbol == '(') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_LPAR;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == '{') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_LBRACE;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == ')') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_RPAR;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == '}') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_RBRACE;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == ';') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_SEMICOLON;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == ':') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_COLON;
        copyString(token.string_value->str, token_string->str);
        return token;


      } else if (symbol == ',') {
        appendToDynamicString(token_string, symbol);
        token.token_type = T_COMMA;
        copyString(token.string_value->str, token_string->str);
        return token;
      }
      break;


    case Less_or_LessEqual_STATE:
      if (symbol == '=') {
        token.token_type = T_LESS_EQUAL;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;
      } else {
        ungetc(symbol, file);
        token.token_type = T_LESS;
        copyString(token.string_value->str, token_string->str);
        return token;
      }
      break;


    case Greater_or_GreaterEqual_STATE:
      if (symbol == '=') {
        token.token_type = T_GREATER_EQUAL;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;
      } else {
        ungetc(symbol, file);
        token.token_type = T_GREATER;
        copyString(token.string_value->str, token_string->str);
        return token;
      }
      break;

    case EQUAL_STATE_or_ASSIGN_STATE:
      if (symbol == '=') {
        token.token_type = T_EQUAL;
        appendToDynamicString(token_string, symbol);
        copyString(token.string_value->str, token_string->str);
        return token;
      } else {
        ungetc(symbol, file);
        token.token_type = T_ASSIGN;
        copyString(token.string_value->str, token_string->str);
        return token;
      }
      break;

    case EOF_STATE:
      if (symbol == EOF) {
        token.token_type = T_EOF;
        return token;
      } else {
        token.token_type = T_ERR;
        return token;
      }
      break;
    }
  }
}