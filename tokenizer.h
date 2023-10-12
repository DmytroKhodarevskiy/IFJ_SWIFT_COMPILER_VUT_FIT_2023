#include <stdio.h>

// Tokenizer type.  You need to fill in the type as part of your implementation.
#ifndef _TOKENIZER_H
#define _TOKENIZER_H

typedef enum{
  //Double, else, func, if, Int, let, nil, return, String,
  //var, while

  K_DOUBLE,
  K_ELSE,
  K_FUNC,
  K_IF,
  K_INT,
  K_LET,
  K_NIL,
  K_RETURN,
  K_STRING,
  K_VAR,
  K_WHILE,
  K_DOUBLE_WITHQ,
  K_STRING_WITHQ,
  K_INT_WITHQ,

} keyword;

typedef enum{

  T_LPAR, // (
  T_RPAR, // )
  T_LBRACE, // {
  T_RBRACE, // }

  T_PLUS, // +
  T_MINUS, // -
  T_MULTIPLY, // *
  T_DIVIDE, // / 

  T_MUL_COMMENT, // /* */
  T_SING_COMMENT, // //

  T_MUL_STRING, // """ """
  T_SING_STRING, // ' '  " "

  T_TYPE_ID, // VARIABLE
  T_ERR, // ERROR
  T_INT, // INTEGER
  T_DOUBLE, // DOUBLE
  T_FLOAT, // FLOAT
  T_STRING, // STRING
  T_EXPONENT_INT, // EXPONENT INTEGER
  T_EXPONENT_FLOAT, // EXPONENT FLOAT

  T_SEMICOLON, // ;
  T_COMMA, // ,
  T_COLON, // :

  T_LESS, // <
  T_GREATER, // >
  T_LESS_EQUAL, // <=
  T_GREATER_EQUAL, // >=
  T_NOT_EQUAL, // !=
  T_ASSIGN, // =
  T_EQUAL, // ==

  T_NOTNIL, // !
  T_BINARY_OP, // ??

  T_EOF, // END OF FILE
  T_EOL, // END OF LINE

  T_KEYWORD, // KEYWORD

} token_type;

typedef struct {

  token_type token_type;
  keyword keyword_type;

  char *string_value;
  int int_value;
  double double_value;

} Token;











#endif // _TOKENIZER_H