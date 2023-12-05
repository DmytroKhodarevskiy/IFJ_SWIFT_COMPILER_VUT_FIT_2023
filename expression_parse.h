#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include "codegenerator.h"
#include "symtable_stack.h"
#include "symtable.h"
// #include "memory.h"
// #include "ctype.h"
// #include "symtable.h"
// #include "stack.h"
#include "codegenerator.h"
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

//DEBUG_PRINTS
/**
 * Prints the current state of the token stack.
 *
 * @param stack The token stack to be printed.
 */
void print_stack(TokenStack stack);

/**
 * Prints the token type of a given token.
 *
 * @param expression_type The token type to be printed.
 */
void print_expression_type(DataType expression_type);
//-----------------------------------------------------------
//CONVERTERS
/**
 * Converts a token type to a symbol type.
 * @param type The token type to be converted.
 * @return The corresponding symbol type.
 */
DataType convert_tokenType_to_symType(token_type type);

/**
 * Converts a symbol type to a token type.
 * @param type The symbol type to be converted.
 * @return The corresponding token type.
 */
token_type convert_symType_to_tokenType(DataType type);
//-----------------------------------------------------------
/**
 * Determines the token type of the result of a given operation.
 *
 * @param op1 The first operand.
 * @param op3 The third operand.
 * @param rule_type The type of the rule to be applied.
 * @return The token type of the result of the operation.
 */
DataType get_token_type(Token op1, Token op3, int rule_type);

/**
 * Finds the last terminal token in the stack.
 *
 * @param stack The stack from which to find the last terminal token.
 * @return The last terminal token if found; otherwise, a token indicating an empty or error state.
 */
Token last_terminal(TokenStack stack);

/**
 * Inserts a reduction edge token in the stack. If the top token is non-terminal,
 * the edge token is inserted before it; otherwise, it's placed on top of the stack.
 *
 * @param stack A pointer to the stack where the edge token will be inserted.
 */
void insert_edge(TokenStack *stack);

/**
 * Counts the number of tokens before encountering a reduction edge token in the stack.
 *
 * @param stack The stack to be scanned for the count.
 * @return The number of tokens before the reduction edge token; -1 if edge not found.
 */
int count_of_token_before_edge(TokenStack stack);

/**
 * Maps a given token to its corresponding index in the precedence table.
 *
 * @param token The token to be mapped.
 * @return The index of the token in the precedence table.
 */
int get_index_from_token(Token token);

/**
 * Parses an expression using an operator precedence parsing method.
 *
 * @param token The current token to be parsed.
 * @param error A pointer to an integer where the error state will be stored (0 for no error, 1 for error).
 * @param file A pointer to the file stream from which tokens are read.
 * @return DataType of the expression
 */
// DataType parse_expression(SymTable *table, Token *token, int *error, FILE** file);
DataType parse_expression(SymStack *symStack, Token *token, int *error, FILE** file, instr_node *main_gen_list, instr_list_dynamic *instr_llist);

/**
 * Determines the index of the grammar rule to be applied based on the given tokens.
 *
 * @param tokens An array of tokens to be analyzed.
 * @param count The number of tokens in the array.
 * @return The index of the grammar rule; -1 if no rule matches.
 */
// int get_rule_index(SymTable *table,Token tokens[], int count, DataType *expression_type);
int get_rule_index(SymStack *table,Token tokens[], int count, DataType *expression_type);

/**
 * Applies a grammar rule to the token stack based on the rule index.
 *
 * @param rule_index The index of the grammar rule to be applied.
 * @param stack A pointer to the token stack to which the rule will be applied.
 */
void perform_rule(int rule_index, TokenStack *stack, DataType *expression_type);

/**
 * Performs a reduce operation on the stack based on the given count of tokens.
 *
 * @param stack A pointer to the stack on which the reduce operation will be performed.
 * @param count The number of tokens to be considered for the reduction.
 * @return 0 if the reduction is successful; -1 if an error occurs or no rule applies.
 */
// int perform_reduce(SymTable *table,TokenStack *stack, int count, DataType *expression_type);
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
void ARG_LIST_EXP(FILE **file, Token *current_token, ListFuncParam *param);

/**
 * @brief Parses an argument expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void ARG_EXP(FILE **file, Token *current_token, ListFuncParam *param);

/**
 * @brief Parses a prefix expression.
 *
 * @param file A pointer to the input file.
 * @param current_token A pointer to the current token.
 */
void PREFIX_EXP(FILE **file, Token *current_token, ListFuncParam *param);


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
