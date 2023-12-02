#ifndef SYMTABLE_STACK_H
#define SYMTABLE_STACK_H

#include <stdio.h>
#include <stdlib.h>

// #include "memory.h"
#include "ctype.h"
#include "symtable.h"
#include "stack.h"


#define INITIAL_STACK_SIZE 10
#define STACK_GROWTH_FACTOR 2

//#define INITIAL_STACK_SIZE 10
//#define STACK_GROWTH_FACTOR 2

// #include "tokenizer.h"
// #include "expression_parse.h"

#include "memory.h"

typedef struct {
  SymTable *items;
  int top;
  int size;
} SymStack;

#include "symtable_stack.h"

/**
 * @brief Initializes a symbol stack with a predefined initial size and sets its top to -1.
 *
 * @param stack Pointer to the symbol stack to initialize.
 */
void s_initializeStack(SymStack *stack);

/**
 * @brief Checks if the symbol stack is empty.
 *
 * @param stack Pointer to the symbol stack to check.
 * @return Returns 1 if the stack is empty, 0 otherwise.
 */
int s_isEmpty(SymStack *stack);

/**
 * @brief Resizes the symbol stack by a predefined growth factor when it's full.
 *
 * @param stack Pointer to the symbol stack to resize.
 */
void s_resizeStack(SymStack *stack);

/**
 * @brief Returns the top element of the symbol stack without removing it.
 *
 * @param stack Pointer to the symbol stack.
 * @return The top element of the stack.
 */
SymTable s_peek(SymStack *stack);

/**
 * @brief Pushes a new symbol table item onto the top of the stack.
 *
 * @param stack Pointer to the symbol stack.
 * @param item The symbol table item to push onto the stack.
 */
void s_push(SymStack *stack, SymTable item);

/**
 * @brief Pops and returns the top element from the symbol stack.
 *
 * @param stack Pointer to the symbol stack.
 * @return The popped symbol table item.
 */
SymTable s_pop(SymStack *stack);

/**
 * @brief Frees the memory allocated for the stack's items and resets its properties.
 *
 * @param stack Pointer to the symbol stack to be freed.
 */
void s_freeStack(SymStack *stack);

/**
 * @brief Searches for a symbol in the stack, checking each symbol table from top to bottom.
 *
 * @param stack Pointer to the symbol stack to search in.
 * @param key The key of the symbol to search for.
 * @return Pointer to the AVLNode containing the symbol, or NULL if not found.
 */
AVLNode *s_search_symtack(SymStack *stack, char *key);

/**
 * @brief Searches for a parameter in a function's parameter list.
 *
 * @param param_list Pointer to the function's parameter list.
 * @param key The key of the parameter to search for.
 * @return Pointer to the AVLNode containing the parameter, or NULL if not found.
 */
AVLNode *s_search_param_list(ListFuncParam *param_list, char *key);

/**
 * @brief Prints the names of the symbol tables in the stack from bottom to top.
 *
 * @param stack Pointer to the symbol stack to be printed.
 */
void Print_Sym_stack(SymStack *stack);




#endif // STACK_H