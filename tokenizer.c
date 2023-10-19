#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "dynamic_string.c"

#define START 100
#define EOF_STATE 101
#define EQUAL_STATE_or_ASSIGN_STATE 102
#define Greater_or_GreaterEqual_STATE 103
#define Less_or_LessEqual_STATE 104
#define Reading_Number_STATE 105
#define Reading_Identifier_or_Keyword_STATE 106


int isKeyword(const char* word) {
    
    if (strcmp(word, "func") == 0) {
        return 1;
    } else if (strcmp(word, "var") == 0) {
        return 1;
    } else if (strcmp(word, "return") == 0) {
        return 1;
    } else if (strcmp(word, "else") == 0) {
        return 1;
    } else if (strcmp(word, "if") == 0) {
        return 1;
    } else if (strcmp(word, "nil") == 0) {
        return 1;
    } else if (strcmp(word, "Int") == 0) {
        return 1;
    } else if (strcmp(word, "Double") == 0) {
        return 1;
    } else if (strcmp(word, "String") == 0) {
        return 1;
    } else if (strcmp(word, "String?") == 0) {
        return 1;
    } else if (strcmp(word, "Int?") == 0) {
        return 1;
    } else if (strcmp(word, "Double?") == 0) {
        return 1;
    }  else {
        return 0;
    }
    
    return 0; 
}

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
    Dynamic_string *token_string;
    token_string = createDynamicString();


    int state = START;
    Token token = init_token();

    while (true) {
        char symbol = (char) fgetc(file);

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
                } else if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Number_STATE;


                } else if (isalpha(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Identifier_or_Keyword_STATE;
                }

                break;

                
            case Reading_Identifier_or_Keyword_STATE:
                if (isalnum(symbol)) {
                    appendToDynamicString(token_string, symbol);
                } else {
                    ungetc(symbol, file);
                    if (isKeyword(token_string->str)) {
                        token.token_type = T_KEYWORD;
                        copyString(token.string_value->str, token_string->str);
                        return token;
                    } else {
                        token.token_type = T_TYPE_ID;
                        copyString(token.string_value->str, token_string->str);
                        return token;
                    }
                }
                break;


            case Reading_Number_STATE:
                if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Number_STATE;
                } else if (symbol == '.') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Number_STATE;
                } else {
                    ungetc(symbol, file);
                    if(strchr(token_string->str, '.') != NULL) {
                        token.token_type = T_DOUBLE;
                        token.double_value = atof(token_string->str);
                    } else {
                        token.token_type = T_INT;
                        token.int_value = atoi(token_string->str);
                    }

                    token.int_value = atoi(token_string->str);
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