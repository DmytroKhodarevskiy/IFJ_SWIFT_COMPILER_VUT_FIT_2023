
#include "symtable_stack.h"

// void initializeStack(SymStack *stack) {
void s_initializeStack(SymStack *stack) {
  stack->top = -1;
  stack->size = INITIAL_STACK_SIZE;
  stack->items = (SymTable *)malloc(stack->size * sizeof(SymTable));

  if (stack->items == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

int s_isEmpty(SymStack *stack) {
  return stack->top == -1;
}

void s_resizeStack(SymStack *stack) {
  stack->size *= STACK_GROWTH_FACTOR;
  stack->items = (SymTable *)realloc(stack->items, stack->size * sizeof(SymTable));

  if (stack->items == NULL) {
    exitWithError("Memory allocation failed.\n", ERR_INTERNAL);
    // fprintf(stderr, "Error: Memory allocation failed\n");
    // exit(EXIT_FAILURE);
  }
}

SymTable s_peek(SymStack *stack) {
    if (s_isEmpty(stack)) {
        // fprintf(stderr, "Error: Stack is empty\n");
        // exit(EXIT_FAILURE);
        exitWithError("Error: Stack is empty\n", ERR_INTERNAL);
    }

    return stack->items[stack->top];
}


void s_push(SymStack *stack, SymTable item) {
  if (stack->top == stack->size - 1) {
    // Stack is full, resize it
    s_resizeStack(stack);
  }

  stack->items[++stack->top] = item;
}

SymTable s_pop(SymStack *stack) {
  SymTable item;
  if (s_isEmpty(stack)) {
    fprintf(stderr, "Error: Stack underflow\n");
    exit(EXIT_FAILURE);
  }

  item = stack->items[stack->top--];
  return item;
}

void s_freeStack(SymStack *stack) {
  free(stack->items);
  stack->top = -1;
  stack->size = 0;
}

SymData s_search_symtack(SymStack *stack, char *key) {
  SymData data;
  AVLNode *node;
  SymTable *table;
  int i;

  for (i = stack->top; i >= 0; i--) {
    *table = stack->items[i];
    node = search_SymTable(table, key);
    if (node != NULL) {
      data = node->data;
      return data;
    }

  }

  exitWithError("Error: Undefined variable or function\n", ERR_SEMANT_UNDF_VALUE);
}