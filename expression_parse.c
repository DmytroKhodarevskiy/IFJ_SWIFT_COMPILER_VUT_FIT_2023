#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "tokenizer.h"

#include "stack.h"
#include "expression_parse.h"
#define size 8

int precedence_table[size][size] = {

// +-  */  rel !   ??  (   )   i   $
  {R,  S,  R,  S,  R,  S,  R,  S,  R},  // +-
  {R,  R,  R,  S,  R,  S,  R,  S,  R},  // */
  {S,  S,  E,  S,  R,  S,  R,  S,  R},  // rel
  {R,  R,  R,  E,  R,  E,  R,  E,  R},  // !
  {S,  S,  S,  S,  S,  S,  R,  S,  R},  // ??
  {S,  S,  S,  S,  S,  S,  EQ, S,  E}, // (
  {R,  R,  R,  R,  R,  E,  S,  E,  S}, // )
  {R,  R,  R,  R,  R,  E,  S,  E,  S}, // i
  {S,  S,  S,  S,  S,  S,  E,  S,  END}  // $ 

};

int get_index_from_token(Token token) {
  switch (token.token_type) {
    case T_PLUS:
    case T_MINUS:
      return 0;
    case T_MULTIPLY:
    case T_DIVIDE:
      return 1;
    case T_GREATER:
    case T_GREATER_EQUAL:
    case T_LESS:
    case T_LESS_EQUAL:
    case T_EQUAL:
    case T_NOT_EQUAL:
      return 2;
    case T_NOTNIL:
      return 3;
    case T_BINARY_OP:
      return 4;
    case T_LPAR:
      return 5;
    case T_RPAR:
      return 6;
    case T_TYPE_ID:
    case T_INT:
    case T_DOUBLE:
    case T_FLOAT:
      return 7;
    case T_EOF:
      return 8;
    default:
      return 8;
  }
}

bool parse_expression(Token token_string[], int token_string_size, int *error) {
  TokenStack stack;
  initializeStack(&stack);

  for (int i = 0; i < token_string_size; i++) {
    Token token = token_string[i];
    int column = get_index_from_token(token);
    int row = get_index_from_token(stack.items[stack.top]);

    Action_Letter action_letter = precedence_table[row][column];

    if (action_letter == S) {
      push(&stack, token);
    } 
    
    else if (action_letter == R) {
      // Token top = stack.items[stack.top];
      // while (top.token_type != T_LPAR) {
        // top = pop(&stack);
      // }
      perform_reduce(&stack);
    } 
    
    else if (action_letter == EQ) {
      perform_reduce(&stack);
    } 
    
    else if (action_letter == E) {
      error = 1;
      return false;
    } 
    
    else if (action_letter == END) {
      return true;
    }
  }
}

Rule rules[] = {
  {"E", {"(", "E", ")", NULL}},
  {"E", {"id", NULL, NULL, NULL}},
  {"E", {"E", "+", "E", NULL}},
  {"E", {"E", "-", "E", NULL}},
  {"E", {"E", "*", "E", NULL}},
  {"E", {"E", "/", "E", NULL}},
  {"E", {"E", "!", NULL, NULL}},
  {"E", {"E", "<", "E", NULL}},
  {"E", {"E", ">", "E", NULL}},
  {"E", {"E", "<=", "E", NULL}},
  {"E", {"E", ">=", "E", NULL}},
  {"E", {"E", "==", "E", NULL}},
  {"E", {"E", "!=", "E", NULL}},
  {"E", {"E", "??", "E", NULL}},
  {"E", {"[function-id]", "(", "<argument-list>", ")"}},
  {"<argument-list>", {"<argument>", "<argument-list>"}},
  {"<argument-list>", {NULL}},
  {"<argument>", {"<expression>", NULL}},
  {NULL, {NULL}}
};

int get_rule_index(Token token1, Token token2, Token token3, Token token4) {
  for (int i = 0; rules[i].left_side != NULL; i++) {
    if (strcmp(rules[i].right_side, token.string_value->str) == 0) {
      return i;
    }
  }
  return -1;
}

void perform_reduce(TokenStack *stack) {

  Token top1,top2,top3,top4;

  if (stack->top != 0)
    top1 = stack->items[stack->top];

  if (stack->top > 1)
    top2 = stack->items[stack->top - 1];

  if (stack->top > 2)
    top3 = stack->items[stack->top - 2];

  if (stack->top > 3)
    top4 = stack->items[stack->top - 3];

  if (top1.token_type == T_TYPE_ID) {
    top1.string_value = "id";
  }

  int rule_index = get_rule_index(top4, top3, top2, top1);

  // while (top.token_type != T_LPAR) {
    // top = pop(stack);
  // }
  // pop(stack);
  // push(stack, top);
}


