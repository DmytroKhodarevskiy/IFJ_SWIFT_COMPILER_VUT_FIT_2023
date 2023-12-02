#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

// #include "symtable_stack.h"
#include "dynamic_string.h"
#include "memory.h"


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
  T_ARROW, // ->



  T_NOTNIL, // !
  T_BINARY_OP, // ??

  T_EOF, // END OF FILE
  T_EOL, // END OF LINE

  T_KEYWORD, // KEYWORD

  T_EMPTY, // EMPTY

  T_UNDERSCORE_ID, // ( _ = ...)
  T_RD_EDGE, // edge of right side of rule
  T_NT, // ( <non-terminal> = ...)

  T_DOLLAR, // $

  T_T

} token_type;




// typedef struct
// {
// 	char *str; /// string ened by '\0' byte
// 	unsigned int length; /// lenght of string
//   unsigned int allocSize; /// size of allocated memory  
// } Dynamic_string;



typedef struct {

  token_type grammar_token_type;
  token_type token_type;
  keyword keyword_type;

  int line_number;

  Dynamic_string *string_value;
  int int_value;
  double double_value;

} Token;

// const char *tokenTypeNames[] = {
//     "Left Parenthesis",         // T_LPAR
//     "Right Parenthesis",        // T_RPAR
//     "Left Brace",               // T_LBRACE
//     "Right Brace",              // T_RBRACE
//     "Plus",                     // T_PLUS
//     "Minus",                    // T_MINUS
//     "Multiply",                 // T_MULTIPLY
//     "Divide",                   // T_DIVIDE
//     "Multi-line Comment",       // T_MUL_COMMENT
//     "Single-line Comment",      // T_SING_COMMENT
//     "Multi-line String",        // T_MUL_STRING
//     "Single-line String",       // T_SING_STRING
//     "Identifier",               // T_TYPE_ID
//     "Error",                    // T_ERR
//     "Integer",                  // T_INT
//     "Double",                   // T_DOUBLE
//     "Float",                    // T_FLOAT
//     "String",                   // T_STRING
//     "Exponent Integer",         // T_EXPONENT_INT
//     "Exponent Float",           // T_EXPONENT_FLOAT
//     "Semicolon",                // T_SEMICOLON
//     "Comma",                    // T_COMMA
//     "Colon",                    // T_COLON
//     "Less Than",                // T_LESS
//     "Greater Than",             // T_GREATER
//     "Less Than or Equal",       // T_LESS_EQUAL
//     "Greater Than or Equal",    // T_GREATER_EQUAL
//     "Not Equal",                // T_NOT_EQUAL
//     "Assignment",               // T_ASSIGN
//     "Equal",                    // T_EQUAL
//     "Not Nil",                  // T_NOTNIL
//     "Nullish Coalescing",       // T_BINARY_OP
//     "End of File",              // T_EOF
//     "End of Line",              // T_EOL
//     "Keyword",                   // T_KEYWORD
//     "Empty",                     // T_EMPTY
//     "Underscore Identifier"     // T_UNDERSCORE_ID
// };



/**
 * @brief Initializes a new token with default values.
 *
 * This function creates a new token and initializes its fields to default
 * values. It sets appropriate types and initializes numerical values to zero
 * and strings to empty.
 *
 * @return Token The initialized token with default values.
 */
Token init_token();
/**
 * @brief Tokenizes the input from a file based on a state machine.
 *
 * This is the main function for tokenizing the input from a file. It reads
 * characters from the file and uses a state machine to identify and return
 * tokens based on the language's syntax rules. The function handles different
 * token types like keywords, identifiers, literals, and operators.
 *
 * @param file The file pointer from which to read and tokenize the input.
 * @return Token The next token identified from the input stream.
 */
Token get_token(FILE *file);

/**
 * @brief Peeks at the next token in the input file without consuming it.
 *
 * This function looks ahead to the next token in the input file without
 * advancing the file pointer. It is used to facilitate decision-making
 * in the tokenizer based on upcoming tokens.
 *
 * @param file The file pointer from which to read the token.
 * @return Token The next token in the input stream.
 */
Token peekNextToken(FILE *file);

/**
 * @brief Checks if a given string is a keyword in the programming language.
 *
 * This function compares a given string with a predefined list of keywords
 * in the programming language. It returns an integer indicating whether the
 * string is a keyword.
 *
 * @param word The string to check against the list of keywords.
 * @return int Returns 1 if the string is a keyword, otherwise 0.
 */
int isKeyword(const char* word);

/**
 * @brief Copies a string from the source to the destination.
 *
 * This function copies a string from a source location to a destination
 * location. It uses the standard string copy function from the C library.
 *
 * @param destination The destination string where the content will be copied.
 * @param source The source string to be copied.
 */
void copyString(char *destination, char *source);


#endif // _TOKENIZER_H