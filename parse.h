#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include "symtable_stack.h"
#include "expression_parse.h"
#include "memory.h"

#ifndef PARSE_H
#define PARSE_H

/**
 * @brief Main function to parse a given file.
 *
 * This function initiates the parsing process. It consists of two phases:
 * the first phase reads global symbol tables and functions, and the second
 * phase processes the program structure.
 *
 * @param file The file stream to parse.
 */
void Parse(FILE *file);

/**
 * @brief Handles the second phase of parsing.
 *
 * In this phase, the function processes the program structure and handles
 * specific programming constructs like loops, conditionals, etc.
 *
 * @param file The file stream being parsed.
 */
void PHASE_SECOND(FILE *file);

/**
 * @brief Handles the first phase of parsing.
 *
 * This phase involves reading global symbol tables and functions. It initializes
 * the symbol table stack and creates the global symbol table.
 *
 * @param file The file stream being parsed.
 * @param stack The symbol table stack used for parsing.
 */
void PHASE_FIRST(FILE *file);

/**
 * @brief Determines the data type from a string.
 *
 * @param str The string representation of the data type.
 * @return DataType The enumerated data type.
 */
DataType get_type(char *str);

/**
 * @brief Checks if two data types are compatible.
 *
 * This function is used to determine if an assignment is semantically valid
 * based on the data types.
 *
 * @param assignType The data type of the value being assigned.
 * @param variableType The data type of the variable receiving the value.
 * @return bool True if the types are compatible, false otherwise.
 */
bool is_compatible(DataType assignType, DataType variableType);

/**
 * @brief Reads and processes the global symbol tables and functions.
 *
 * This function iterates through the file and fills in the symbol tables
 * and functions accordingly.
 *
 * @param file The file stream to read from.
 * @param stack The symbol table stack for managing scopes.
 */
void FILL_TREES(FILE *file, SymStack *stack);

/**
 * @brief Parses the main program structure.
 *
 * @param file The file stream being parsed.
 */
void PROGRAM(FILE *file);

/**
 * @brief Parses global declarations and functions.
 *
 * @param file The file stream being parsed.
 */
void DECLARE_GLOBAL_FUNC(FILE *file);

/**
 * @brief Parses a list of statements.
 *
 * This function processes multiple programming statements sequentially.
 *
 * @param file The file stream being parsed.
 */
void STMT_LIST(FILE *file);

/**
 * @brief Parses an individual statement.
 *
 * This function identifies the type of statement (e.g., if, while, variable declaration)
 * and processes it accordingly.
 *
 * @param file The file stream being parsed.
 */
void STMT(FILE *file);

/**
 * @brief Parses either an assignment statement or a function call.
 *
 * @param file The file stream being parsed.
 */
void ASSIGN_STMT_OR_FUNCALL(FILE *file);

/**
 * @brief Parses function calls and their arguments.
 *
 * @param file The file stream being parsed.
 */
void FUNC_CALLS(FILE *file);

/**
 * @brief Parses the argument list of a function call.
 *
 * @param file The file stream being parsed.
 */
void ARG_LIST(FILE *file);

/**
 * @brief Parses an individual argument in a function call.
 *
 * @param file The file stream being parsed.
 */
void ARG(FILE *file);

/**
 * @brief Parses the prefix of a function argument.
 *
 * @param file The file stream being parsed.
 */
void PREFIX(FILE *file);

/**
 * @brief Parses the return type of a function.
 *
 * @param file The file stream being parsed.
 */
void RETURN_TYPE(FILE *file);

/**
 * @brief Parses the parameter list for a function declaration.
 *
 * @param file The file stream being parsed.
 * @param params Pointer to store parsed parameters.
 * @param param_cnt Pointer to store the count of parameters.
 */
void PARAM_LIST_FIRST(FILE *file, ListFuncParam **params, int *param_cnt);

/**
 * @brief Parses an individual parameter in a function declaration.
 *
 * @param file The file stream being parsed.
 * @param params Pointer to store parsed parameter.
 */
void PARAM_FIRST(FILE *file, ListFuncParam **params);

/**
 * @brief Parses the list of parameters in a function declaration.
 *
 * @param file The file stream being parsed.
 */
void PARAM_LIST(FILE *file);

/**
 * @brief Parses an individual parameter in a function declaration.
 *
 * @param file The file stream being parsed.
 */
void PARAM(FILE *file);

/**
 * @brief Parses the name of a parameter in a function declaration.
 *
 * @param file The file stream being parsed.
 */
void PARAM_NAME(FILE *file);

/**
 * @brief Parses the prefix of a parameter in a function declaration.
 *
 * @param file The file stream being parsed.
 */
void PARAM_PREFIX(FILE *file);

/**
 * @brief Parses a modified statement for let or var keywords.
 *
 * @param file The file stream being parsed.
 */
void MB_STMT_LET_VAR(FILE *file);

/**
 * @brief Parses an assignment expression.
 *
 * @param file The file stream being parsed.
 * @param type The data type expected in the assignment.
 */
void MB_ASSIGN_EXPR(FILE *file, DataType type);

/**
 * @brief Parses a data type.
 *
 * @param file The file stream being parsed.
 * @param type Pointer to store the parsed data type.
 */
void TYPE(FILE *file, DataType *type);

/**
 * @brief Parses a while loop expression.
 *
 * @param file The file stream being parsed.
 */
void WHILE_EXP(FILE *file);

/**
 * @brief Parses an if statement expression.
 *
 * @param file The file stream being parsed.
 */
void IF_EXP(FILE *file);

/**
 * @brief Parses an expression.
 *
 * This function is used in contexts like if, while conditions or return statements.
 *
 * @param file The file stream being parsed.
 */
void EXP(FILE *file);


#endif // PARSE_H