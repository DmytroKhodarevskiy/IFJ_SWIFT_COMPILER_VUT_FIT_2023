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
    SymTable **items;
    int top;
    int size;
} SymStack;

// Function prototypes
void s_initializeStack(SymStack *stack);
int s_isEmpty(SymStack *stack);
void s_resizeStack(SymStack *stack);
// void s_push(SymStack *stack, SymTable item);
void s_push(SymStack *stack, SymTable *item);

SymTable s_pop(SymStack *stack);
void s_freeStack(SymStack *stack);
// SymTable s_peek(SymStack *stack);
SymTable *s_peek(SymStack *stack);
// SymData s_search_symtack(SymStack *stack, char *key);
AVLNode *s_search_symtack(SymStack *stack, char *key);
AVLNode *s_search_param_list(ListFuncParam *param_list, char *key);
// void Print_Sym_stack(SymStack stack);
void Print_Sym_stack(SymStack *stack);
SymData* s_getFirstFunctionSymData(SymStack *stack);
// int Get_Deepness(SymStack *stack);
int Get_deepness_current(SymStack *stack);
int Get_deepness_of_var(SymStack *stack, char* id_name) ;





#endif // STACK_H