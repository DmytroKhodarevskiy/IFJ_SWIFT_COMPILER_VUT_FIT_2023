#include "stack.h"

void initializeStack(TokenStack *stack) {
  stack->top = -1;
  stack->size = INITIAL_STACK_SIZE;
  stack->items = (Token *)malloc(stack->size * sizeof(Token));

  if (stack->items == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

int isEmpty(TokenStack *stack) {
  return stack->top == -1;
}

void resizeStack(TokenStack *stack) {
  stack->size *= STACK_GROWTH_FACTOR;
  stack->items = (Token *)realloc(stack->items, stack->size * sizeof(Token));

  if (stack->items == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

void push(TokenStack *stack, Token item) {
  if (stack->top == stack->size - 1) {
    // Stack is full, resize it
    resizeStack(stack);
  }

  stack->items[++stack->top] = item;
}

Token pop(TokenStack *stack) {
  Token item;
  if (isEmpty(stack)) {
    fprintf(stderr, "Error: Stack underflow\n");
    exit(EXIT_FAILURE);
  }

  item = stack->items[stack->top--];
  return item;
}

void freeStack(TokenStack *stack) {
  free(stack->items);
  stack->top = -1;
  stack->size = 0;
}