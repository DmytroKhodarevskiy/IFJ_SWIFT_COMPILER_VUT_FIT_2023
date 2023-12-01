#ifndef SYMTABLE_STACK_H
#define SYMTABLE_STACK_H

#include <stdio.h>
#include <stdlib.h>


#define INITIAL_STACK_SIZE 10
#define STACK_GROWTH_FACTOR 2

//#define INITIAL_STACK_SIZE 10
//#define STACK_GROWTH_FACTOR 2

// #include "tokenizer.h"
#include "expression_parse.h"

#include "memory.h"

typedef struct {
  SymTable *items;
  int top;
  int size;
} SymStack;

// Function prototypes
void s_initializeStack(SymStack *stack);
int s_isEmpty(SymStack *stack);
void s_resizeStack(SymStack *stack);
void s_push(SymStack *stack, SymTable item);
SymTable s_pop(SymStack *stack);
void s_freeStack(SymStack *stack);
SymTable s_peek(SymStack *stack);
#endif // STACK_H