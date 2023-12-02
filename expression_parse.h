#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// #include "memory.h"
// #include "ctype.h"
// #include "symtable.h"
// #include "stack.h"
#include "symtable_stack.h"

typedef struct {
  token_type left_side;
  token_type right_side[4]; // Assuming the maximum size of right_side array is 4
} Rule;

typedef enum{
  S, // shift
  R, // reduce
  EQ, // equal
  E, // error
  END, // END
} Action_Letter;

// Debugging Utilities
/**
 * @brief Prints the current state of the token stack for debugging purposes.
 *
 * @param stack The token stack to be printed.
 */
void print_stack(TokenStack stack);

/**
 * @brief Prints the token type of a given token for debugging purposes.
 *
 * @param expression_type The token type to be printed.
 */
void print_expression_type(DataType expression_type);

// Converters
/**
 * @brief Converts a token type to its corresponding symbol type.
 *
 * @param type The token type to be converted.
 * @return The corresponding symbol type.
 */
DataType convert_tokenType_to_symType(token_type type);

/**
 * @brief Converts a symbol type to its corresponding token type.
 *
 * @param type The symbol type to be converted.
 * @return The corresponding token type.
 */
token_type convert_symType_to_tokenType(DataType type);

// Token Type Operations
/**
 * @brief Determines the token type of the result for a specific operation.
 *
 * @param op1 The first operand token.
 * @param op3 The third operand token.
 * @param rule_type The type of the rule being applied.
 * @return The token type resulting from the operation.
 */
DataType get_token_type(Token op1, Token op3, int rule_type);

// Token Stack Operations
/**
 * @brief Retrieves the last terminal token from the stack.
 *
 * @param stack The stack from which the last terminal token is to be found.
 * @return The last terminal token, or a token indicating an empty/error state.
 */
Token last_terminal(TokenStack stack);

/**
 * @brief Inserts a reduction edge token into the stack.
 *
 * @param stack A pointer to the stack where the edge token will be inserted.
 */
void insert_edge(TokenStack *stack);

/**
 * @brief Counts the number of tokens in the stack before encountering a reduction edge token.
 *
 * @param stack The stack to be scanned.
 * @return The number of tokens before the reduction edge token, or -1 if the edge is not found.
 */
int count_of_token_before_edge(TokenStack stack);

// Precedence Table Operations
/**
 * @brief Maps a given token to its corresponding index in the precedence table.
 *
 * @param token The token to be mapped.
 * @return The index of the token in the precedence table.
 */
int get_index_from_token(Token token);

// Expression Parsing
/**
 * @brief Parses an expression using operator precedence parsing.
 *
 * @param table The symbol stack used for parsing.
 * @param token The current token to be parsed.
 * @param error A pointer to an integer for storing the error state (0 for no error, 1 for error).
 * @param file A pointer to the file stream for reading tokens.
 * @return The data type of the parsed expression.
 */
DataType parse_expression(SymStack *table, Token *token, int *error, FILE** file);

// Grammar Rule Application
/**
 * @brief Determines the appropriate grammar rule index based on a set of tokens.
 *
 * @param table The symbol stack.
 * @param tokens An array of tokens for rule determination.
 * @param count The number of tokens in the array.
 * @param expression_type A pointer to the resulting expression type.
 * @return The index of the applicable grammar rule, or -1 if no rule matches.
 */
int get_rule_index(SymStack *table,Token tokens[], int count, DataType *expression_type);

/**
 * @brief Applies a specified grammar rule to the token stack.
 *
 * @param rule_index The index of the grammar rule to apply.
 * @param stack A pointer to the token stack where the rule will be applied.
 * @param expression_type A pointer to the resulting expression type.
 */
void perform_rule(int rule_index, TokenStack *stack, DataType *expression_type);

/**
 * @brief Executes a reduction operation on the stack using a specified number of tokens.
 *
 * @param stack A pointer to the stack for reduction.
 * @param count The number of tokens to be included in the reduction.
 * @return 0 if reduction is successful; -1 in case of an error or if no rule applies.
 */
int perform_reduce(SymStack *table, TokenStack *stack, int count, DataType *expression_type);


//Functions for parsing expressions with functions as operands

/**
 * @brief Parses a function call expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void FUNC_CALLS_EXP(FILE **file, Token *current_token);

/**
 * @brief Parses an argument list expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void ARG_LIST_EXP(FILE **file, Token *current_token);

/**
 * @brief Parses an argument expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void ARG_EXP(FILE **file, Token *current_token);

/**
 * @brief Parses a prefix expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void PREFIX_EXP(FILE **file, Token *current_token);


//helper functions for parsing expressions with functions as operands

/**
 * @brief Searches for a newline character ('\n') in an open file.
 *
 * This function reads characters from the given file until it encounters
 * a newline character or a non-space character. It increments the global
 * line number variable (linenum) for each newline character found.
 *
 * @param file A pointer to the open file to search.
 * @return true if a newline character ('\n') is found, false otherwise.
 */
bool findNewLineInFile(FILE *file);

/**
 * @brief Checks if a given data type is nullable.
 *
 * This function determines whether a data type is nullable by comparing
 * it to predefined nullable types (e.g., TYPE_INT_NULLABLE).
 *
 * @param type The data type to check for nullability.
 * @return true if the data type is nullable, false otherwise.
 */
bool is_nullable(DataType type);

/**
 * @brief Checks if a specific operation is allowed for a given data type.
 *
 * This function verifies whether a specified operation is allowed for a
 * given data type, taking into account nullability constraints. It provides
 * an error message if the operation is not allowed on nullable types or nil.
 *
 * @param expression_type The data type of the expression.
 * @param operation The operation to check (e.g., "+", "-", "*", "/").
 * @return true if the operation is allowed, false if it violates nullability rules.
 */
bool restricted_operations_with_operation(DataType expression_type, char *operation);


#endif // _EXPR_PARSER_H
