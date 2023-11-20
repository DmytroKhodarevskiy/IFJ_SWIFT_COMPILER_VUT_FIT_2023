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

#define Checking_Exponential 107
#define Reading_Expontential_number_with_sign_STATE 108
#define Reading_Exponential_without_sign_STATE 109

#define Checking_Comment_STATE 111
#define Single_Line_Comment_STATE 112
#define Multi_Line_Comment_STATE 113
#define Checking_end_of_Multi_Line_Comment_STATE 114

#define Checking_Exclamation_STATE 115
#define Checking_Questionmark_STATE 116

#define Reading_String_STATE 117
#define Multi_line_string_check1_STATE 118
#define Multi_line_string_check2_STATE 119
#define Multi_line_FINALLY_STATE 121
#define Multi_line_string_check4_STATE 122
#define Multi_line_string_check5_STATE 123
#define Multi_line_hex_STATE 124
#define Multi_line_escape_sequence_STATE 125
#define Multi_line_hex_num_STATE 126
#define Single_line_hex_STATE 127
#define Single_line_hex_num_STATE 128
#define Escape_char_err_check_STATE 129

#define Escape_sequence_STATE 130

#define check_keyword_questionmark_STATE 131

#define special_underscore_STATE 132

#define CheckifArrowState 133


Token lookaheadToken;
bool hasPeeked = false;

int isKeyword(const char* word) {
    
    if (strcmp(word, "func") == 0) {
        return 1;
    } else if (strcmp(word, "var") == 0) {
        return 1;
    } else if (strcmp(word, "let") == 0) {
        return 1;
    } else if (strcmp(word, "while") == 0) {
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
//   if (source != NULL)
    // free(source);
}


Token peekNextToken(FILE *file) {
    if (!hasPeeked) {
        lookaheadToken = get_token(file);
        hasPeeked = true;
    }
    return lookaheadToken;
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

    if (hasPeeked) {
        hasPeeked = false;
        return lookaheadToken;
    }

    Dynamic_string *token_string;
    token_string = createDynamicString();


    int state = START;
    Token token = init_token();

    while (true) {
        char symbol = (char) fgetc(file);

        switch (state) {
            case START:

                if (symbol == '\n') {
                    state = START;


                } else if (symbol == '\t') {
                    state = START;


                } else if (symbol == '\r') {
                    state = START;


                } else if (symbol == '_') {
                    appendToDynamicString(token_string, symbol);
                    state = special_underscore_STATE;


                } else if (symbol == EOF) {
                    state = EOF_STATE;


                } else if (isspace(symbol)) {
                    state = START;


                } else if (symbol == '+') {
                   token.token_type = T_PLUS;
                    appendToDynamicString(token_string, symbol);
                    copyString(token.string_value->str, token_string->str);
                    return token;


                } else if (symbol == '-') {
                    appendToDynamicString(token_string, symbol);
                    state = CheckifArrowState;


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


                } else if (symbol == '!') {
                    appendToDynamicString(token_string, symbol);
                    state = Checking_Exclamation_STATE;


                } else if (symbol == '?') {
                    appendToDynamicString(token_string, symbol);
                    state = Checking_Questionmark_STATE;


                } else if (isalpha(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Identifier_or_Keyword_STATE;
               

                } else if (symbol == '/') {
                    appendToDynamicString(token_string, symbol);
                    state = Checking_Comment_STATE;
                

                } else if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                
                
                } else {
                    token.token_type = T_ERR;
                    return token;
                }
                break;
            
            case CheckifArrowState:
                if (symbol == '>') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_ARROW;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    token.token_type = T_MINUS;
                    return token;
                }
                break;

            case special_underscore_STATE:
                if (symbol == ' ') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_UNDERSCORE_ID;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    token.token_type = T_ERR;
                    return token;
                }
                break;

            case Reading_String_STATE:
                if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_string_check1_STATE;
                } else if (symbol == '\\'){
                    appendToDynamicString(token_string, symbol);
                    state = Escape_sequence_STATE;
                } else if (symbol == '\n' || symbol == EOF) {
                    token.token_type = T_ERR;
                    return token;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                }
                break;

            case Escape_sequence_STATE:
                if (symbol == 'n') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                } else if (symbol == 't') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                } else if (symbol == 'r') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                } else if (symbol == 'u') {
                    appendToDynamicString(token_string, symbol);
                    state = Single_line_hex_STATE;
                } else if (symbol == '\\') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                } else if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Escape_char_err_check_STATE;
                } else {
                    token.token_type = T_ERR;
                    return token;
                }
                break;

            case Escape_char_err_check_STATE:
                if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_SING_STRING;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else if (symbol == '\n' || symbol == EOF) {
                    token.token_type = T_ERR;
                    return token;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                }
                break;

            case Single_line_hex_STATE:
                if (symbol == '{') {
                    appendToDynamicString(token_string, symbol);
                    state = Single_line_hex_num_STATE;
                } else {
                    token.token_type = T_ERR;
                    return token;
                }
                break;

            case Single_line_hex_num_STATE:
                if (isxdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Single_line_hex_num_STATE;
                } else if (symbol == '}') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_String_STATE;
                } else {
                    token.token_type = T_ERR;
                    return token;
                }
                break;

            case Multi_line_string_check1_STATE:
                if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_string_check2_STATE;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_SING_STRING;
                    copyString(token.string_value->str, token_string->str);
                    return token;                    
                }
                break;

         
            case Multi_line_string_check2_STATE:
                if (symbol == '\n') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_FINALLY_STATE;
                } else if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_SING_STRING;    
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_SING_STRING;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;


            case Multi_line_FINALLY_STATE:
                if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_string_check4_STATE;
                } else if (symbol == EOF) {
                    token.token_type = T_ERR;
                    return token;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_FINALLY_STATE;
                }
                break;

            // case Multi_line_escape_sequence_STATE:
            //     printf("symbol: %c\n", symbol);
            //     if (symbol == 'n') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else if (symbol == 't') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else if (symbol == 'r') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else if (symbol == '\\') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else if (symbol == '"') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else if (symbol == 'u') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_hex_STATE;
            //     } else {
            //         token.token_type = T_ERR;
            //         return token;
            //     }
            //     break;

            // case Multi_line_hex_STATE:
            //     if (symbol == '{') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_hex_num_STATE;
            //     } else {
            //         token.token_type = T_ERR;
            //         return token;
            //     }
            //     break;

            // case Multi_line_hex_num_STATE:
            //     if (isxdigit(symbol)) {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_hex_num_STATE;
            //     } else if (symbol == '}') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Multi_line_FINALLY_STATE;
            //     } else {
            //         token.token_type = T_ERR;
            //         return token;
            //     }
            //     break;

            case Multi_line_string_check4_STATE:
                if (symbol == '\"') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_string_check5_STATE;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_line_FINALLY_STATE;
                }
                break;


            case Multi_line_string_check5_STATE:
                if (symbol == '"') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_MUL_STRING;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    token.token_type = T_ERR;
                    return token;
                }


            case Checking_Questionmark_STATE:
                if (symbol == '?') {
                    token.token_type = T_BINARY_OP;
                    appendToDynamicString(token_string, symbol);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_ERR;
                    return token;
                }
                break;


            case Checking_Exclamation_STATE:
                if (symbol == '=') {
                    token.token_type = T_NOT_EQUAL;
                    appendToDynamicString(token_string, symbol);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_NOTNIL;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;


            case Checking_Comment_STATE:
                if (symbol == '/') {
                    appendToDynamicString(token_string, symbol);
                    state = Single_Line_Comment_STATE;
                } else if (symbol == '*') {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_Line_Comment_STATE;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_DIVIDE;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;


            case Single_Line_Comment_STATE:
                if (symbol == '\n' || symbol == EOF) {
                    token.token_type = T_SING_COMMENT;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Single_Line_Comment_STATE;
                }
                break;


            case Multi_Line_Comment_STATE:
                if (symbol == '*') {
                    appendToDynamicString(token_string, symbol);
                    state = Checking_end_of_Multi_Line_Comment_STATE;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_Line_Comment_STATE;
                }
                break;


            case Checking_end_of_Multi_Line_Comment_STATE:
                if (symbol == '/') {
                    appendToDynamicString(token_string, symbol);
                    token.token_type = T_MUL_COMMENT;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    appendToDynamicString(token_string, symbol);
                    state = Multi_Line_Comment_STATE;
                }
                break;
                
            case check_keyword_questionmark_STATE:
                if (symbol == '?') {
                    token.token_type = T_KEYWORD;
                    appendToDynamicString(token_string, symbol);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_KEYWORD;
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;

            case Reading_Identifier_or_Keyword_STATE:
                if (isalnum(symbol) || symbol == '_') {
                    appendToDynamicString(token_string, symbol);
                } else {
                    ungetc(symbol, file);
                    if (isKeyword(token_string->str)) {
                        if(!strcmp(token_string->str, "Double") || !strcmp(token_string->str, "Int") || !strcmp(token_string->str, "String")){
                            state = check_keyword_questionmark_STATE;
                            break;
                        }
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


            // case Reading_Number_STATE:
            //     if (isdigit(symbol)) {
            //         appendToDynamicString(token_string, symbol);
            //         state = Reading_Number_STATE;
            //     } else if (symbol == '.') {
            //         appendToDynamicString(token_string, symbol);
            //         state = Reading_Number_STATE;
            //     } else if (!isdigit(symbol)) {
            //         if (symbol == 'e' || symbol == 'E'){
            //             appendToDynamicString(token_string, symbol);
            //             state = Checking_Exponential;
            //         } else {
            //             token.token_type = T_ERR;
            //             return token;
            //         }
            //     } else {

            //         ungetc(symbol, file);
            //         if(strchr(token_string->str, '.') != NULL) {
            //             token.token_type = T_DOUBLE;
            //             token.double_value = atof(token_string->str);
            //         } else {
            //             token.token_type = T_INT;
            //             token.int_value = atoi(token_string->str);
            //         }

            //         token.int_value = atoi(token_string->str);
            //         copyString(token.string_value->str, token_string->str);
            //         return token;
            //     }
            //     break;

            case Reading_Number_STATE:
                if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Number_STATE;
                } else if (symbol == '.') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Number_STATE;
                } else if (symbol == 'e' || symbol == 'E') {
                    appendToDynamicString(token_string, symbol);
                    state = Checking_Exponential;
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


            case Checking_Exponential:
                if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Exponential_without_sign_STATE;
                } else if (symbol == '+' || symbol == '-') {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Expontential_number_with_sign_STATE;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_EXPONENT_INT;
                    token.double_value = atof(token_string->str);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;


            case Reading_Exponential_without_sign_STATE:
                if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Exponential_without_sign_STATE;
                } else if (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == EOF) {
                    ungetc(symbol, file);
                    token.token_type = T_EXPONENT_INT;
                    token.double_value = atof(token_string->str);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_EXPONENT_INT;
                    token.double_value = atof(token_string->str);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                }
                break;


            case Reading_Expontential_number_with_sign_STATE:
                if (isdigit(symbol)) {
                    appendToDynamicString(token_string, symbol);
                    state = Reading_Expontential_number_with_sign_STATE;
                } else if (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == EOF) {
                    ungetc(symbol, file);
                    token.token_type = T_EXPONENT_INT;
                    token.double_value = atof(token_string->str);
                    copyString(token.string_value->str, token_string->str);
                    return token;
                } else {
                    ungetc(symbol, file);
                    token.token_type = T_EXPONENT_INT;
                    token.double_value = atof(token_string->str);
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