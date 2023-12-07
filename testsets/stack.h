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

//// Define token_type and keyword enums
//typedef enum {
//  TOKEN_IDENTIFIER,
//  TOKEN_NUMBER,
//  // Add more token types as needed
//} token_type;
//
//typedef enum {
//  KEYWORD_IF,
//  KEYWORD_WHILE,
//  // Add more keyword types as needed
//} keyword;
//
//// Define Dynamic_string structure
//typedef struct {
//  char *str;
//  unsigned int length;
//  unsigned int allocSize;
//} Dynamic_string;
//
//// Define Token structure
//typedef struct {
//  token_type token_type;
//  keyword keyword_type;
//
//  Dynamic_string *string_value;
//  int int_value;
//  double double_value;
//} Token;
//
//// Define TokenStack structure
//typedef struct {
//  Token *items;
//  int top;
//  int size;
//} TokenStack;

// Function prototypes
void initializeStack(TokenStack *stack);
int isEmpty(TokenStack *stack);
void resizeStack(TokenStack *stack);
void push(TokenStack *stack, Token item);
Token pop(TokenStack *stack);
void freeStack(TokenStack *stack);

#endif // STACK_H