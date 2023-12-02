#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>


#define INITIAL_STACK_SIZE 10
#define STACK_GROWTH_FACTOR 2

//#define INITIAL_STACK_SIZE 10
//#define STACK_GROWTH_FACTOR 2

#include "tokenizer.h"

typedef struct {
    Token *items;
    int top;
    int size;
} TokenStack;


// Function prototypes
/**
 * @brief Initializes a stack for storing tokens.
 *
 * This function sets up a new stack by allocating memory for its items,
 * setting the initial size, and initializing the top index. It handles memory
 * allocation errors by printing an error message and exiting.
 *
 * @param stack Pointer to the stack to be initialized.
 */

/**
 * @brief Checks if the stack is empty.
 *
 * This function returns an integer indicating whether the stack is empty
 * (i.e., the top index is -1).
 *
 * @param stack Pointer to the stack to be checked.
 * @return int Returns 1 if the stack is empty, otherwise 0.
 */
int isEmpty(TokenStack *stack);


/**
 * @brief Resizes the stack to accommodate more items.
 *
 * This function increases the size of the stack when it's full. It reallocates
 * memory for the stack's items using a growth factor and handles memory
 * allocation errors.
 *
 * @param stack Pointer to the stack to be resized.
 */
void resizeStack(TokenStack *stack);


/**
 * @brief Pushes a token onto the stack.
 *
 * This function adds a new token to the top of the stack. If the stack is full,
 * it resizes the stack to create more space before pushing the token.
 *
 * @param stack Pointer to the stack where the token will be pushed.
 * @param item The token to be pushed onto the stack.
 */
void push(TokenStack *stack, Token item);


/**
 * @brief Pops the top token from the stack.
 *
 * This function removes and returns the token at the top of the stack. If the
 * stack is empty, it prints an error message and exits.
 *
 * @param stack Pointer to the stack from which the token will be popped.
 * @return Token The token removed from the top of the stack.
 */
Token pop(TokenStack *stack);

/**
 * @brief Frees the memory allocated for the stack's items.
 *
 * This function deallocates the memory used by the stack's items and resets
 * the stack's properties (top index and size).
 *
 * @param stack Pointer to the stack to be freed.
 */
void freeStack(TokenStack *stack);

#endif // STACK_H