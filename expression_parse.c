#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "expression_parse.h"

#include "stack.c"
#include "tokenizer.c"
#define size 9

typedef struct token_symbol {
  char *string;
  int index;
} Token_Symbol;

int precedence_table[size][size] = {

// +-  */  rel !   ??  (   )   i   $
  {R,  S,  R,  S,  R,  S,  R,  S,  R},  // +-
  {R,  R,  R,  S,  R,  S,  R,  S,  R},  // */
  {S,  S,  E,  S,  R,  S,  R,  S,  R},  // rel
  {R,  R,  R,  E,  R,  E,  R,  E,  R},  // !
  {S,  S,  S,  S,  S,  S,  R,  S,  R},  // ??
  {S,  S,  S,  S,  S,  S,  EQ, S,  E},  // (
  {R,  R,  R,  R,  R,  E,  S,  E,  R},  // )
  {R,  R,  R,  R,  R,  E,  S,  E,  R},  // i
  {S,  S,  S,  S,  S,  S,  E,  S,  END} // $ 

};

void print_stack(TokenStack stack) {
  printf("[$] ");
  for (int i = 0; i <= stack.top; i++) {
    printf("[%d] ", stack.items[i].token_type);
  }
  printf("\n");
}

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

// bool parse_expression(Token token_string[], int token_string_size, int *error) {
//   TokenStack stack;
//   initializeStack(&stack);

//   for (int i = 0; i < token_string_size; i++) {
//     Token token = token_string[i];
//     int column = get_index_from_token(token);
//     int row = get_index_from_token(stack.items[stack.top]);

//     Action_Letter action_letter = precedence_table[row][column];

//     if (action_letter == S) {
//       push(&stack, token);
//     } 
    
//     else if (action_letter == R) {
//       // Token top = stack.items[stack.top];
//       // while (top.token_type != T_LPAR) {
//         // top = pop(&stack);
//       // }
//       perform_reduce(&stack);
//     } 
    
//     else if (action_letter == EQ) {
//       perform_reduce(&stack);
//     } 
    
//     else if (action_letter == E) {
//       error = 1;
//       return false;
//     } 
    
//     else if (action_letter == END) {
//       return true;
//     }
//   }
// }

bool end_of_expr(Token token) {
  if (token.token_type == T_LBRACE) {
    return true;
  }

  return false;
}

bool parse_expression(Token token, int *error, FILE* file) {
  TokenStack stack;
  initializeStack(&stack);

  // while (end_of_expr(token) == false || isEmpty(&stack) == false) {
  // while (end_of_expr(token) == false && strcmp(stack.items[stack.top].string_value->str, "E")) {
  // while (end_of_expr(token) == false && stack.items[stack.top].token_type != T_NT) {
  while (!(end_of_expr(token) == true && stack.items[stack.top].token_type == T_NT && stack.top == 0)) {
  
    int column = get_index_from_token(token);

    Token Trow = init_token();

    int row;

    if (stack.top != -1) {
      Trow = stack.items[stack.top];
      row = get_index_from_token(Trow);

      if (strcmp(Trow.string_value->str, "E") == 0) {
        if (stack.top > 0) {
          Trow = stack.items[stack.top - 1];
          row = get_index_from_token(Trow);
        }
        else
          row = 8;
      }

    }

    else {
      row = 8;
    }
    

    Action_Letter action_letter = precedence_table[row][column];

    printf("row: %d, column: %d, action_letter: %d\n", row, column, action_letter);

    if (action_letter == S) {
      push(&stack, token);

      printf("SHIFT\n");

      token = get_token(file);
    } 
    
    else if (action_letter == R) {
      printf("REDUCE\n");
      perform_reduce(&stack, error);
      print_stack(stack);
      if (*error == 1) {
        return false;
      }
      printf("REDUCE DONE\n");
    } 
    
    else if (action_letter == EQ) {
      perform_reduce(&stack, error);
      if (*error == 1) {
        return false;
      }
    } 
    
    else if (action_letter == E) {
      *error = 1;
      return false;
    } 
    
    else if (action_letter == END) {
      return true;
    }

  }

  return true;
}

// Rule rules[] = {
//   {"E",               {"(",             "E",              ")",              NULL}},
//   {"E",               {"id",            NULL,             NULL,             NULL}},
//   {"E",               {"E",             "+",              "E",              NULL}},
//   {"E",               {"E",             "-",              "E",              NULL}},
//   {"E",               {"E",             "*",              "E",              NULL}},
//   {"E",               {"E",             "/",              "E",              NULL}},
//   {"E",               {"E",             "!",              NULL,             NULL}},
//   {"E",               {"E",             "<",              "E",              NULL}},
//   {"E",               {"E",             ">",              "E",              NULL}},
//   {"E",               {"E",             "<=",             "E",              NULL}},
//   {"E",               {"E",             ">=",             "E",              NULL}},
//   {"E",               {"E",             "==",             "E",              NULL}},
//   {"E",               {"E",             "!=",             "E",              NULL}},
//   {"E",               {"E",             "??",             "E",              NULL}},
//   {"E",               {"[function-id]", "(",              "<argument-list>", ")"}},
//   {"<argument-list>", {"<argument>",    "<argument-list>", NULL,            NULL}},
//   {"<argument-list>", {NULL,            NULL,              NULL,            NULL}},
//   {"<argument>",      {"<expression>",  NULL,              NULL,            NULL}},
//   {NULL,              {NULL,            NULL,              NULL,            NULL}}
// };

Rule rules[] = {
  {T_NT,               {T_LPAR,           T_NT,              T_RPAR,            T_EMPTY}},
  {T_NT,               {T_TYPE_ID,        T_EMPTY,           T_EMPTY,           T_EMPTY}},
  {T_NT,               {T_NT,             T_PLUS,            T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_MINUS,           T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_MULTIPLY,        T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_DIVIDE,          T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_NOTNIL,          T_EMPTY,           T_EMPTY}},
  {T_NT,               {T_NT,             T_LESS,            T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_GREATER,         T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_LESS_EQUAL,      T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_GREATER_EQUAL,   T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_EQUAL,           T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_NOT_EQUAL,       T_NT,              T_EMPTY}},
  {T_NT,               {T_NT,             T_BINARY_OP,       T_NT,              T_EMPTY}},


  // {T_NT,               {"[function-id]", "(",              "<argument-list>", T_RBRACE}},
  // {"<argument-list>", {"<argument>",    "<argument-list>", T_EMPTY,            T_EMPTY}},
  // {"<argument-list>", {T_EMPTY,            T_EMPTY,              T_EMPTY,            T_EMPTY}},
  // {"<argument>",      {"<expression>",  T_EMPTY,             T_EMPTY,            T_EMPTY}},
  {T_EMPTY,              {T_EMPTY,            T_EMPTY,              T_EMPTY,            T_EMPTY}}
};

// int get_rule_index(Token tokens[]) {
int get_rule_index(token_type token_types[]) {

  
  int match = 0;  

  // for (int i = 0; i < 18; i++) {
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 4; j++) {
      // printf("rules[%d].right_side[%d]: %d\n", i, j, rules[i].right_side[j]);
      // printf("token_types[%d]: %d\n", 3-j, token_types[3-j]);
      // printf("\n\n\n");
      if (rules[i].right_side[j] == token_types[3-j]) {
        match++;
      }
    }

    if (match == 4) {
      return i;
    }
    else {
      match = 0;
    }
  }

  return -1;

}

void perform_rule(int rule_index, TokenStack *stack) {
  
  Token Etoken = init_token();
  Etoken.string_value->str = "E";
  Etoken.token_type = T_NT;

  printf("rule_index: %d\n", rule_index);

  if (rule_index == 1) {
    pop(stack);
    push(stack, Etoken);
  }

  if (rule_index == 6) {
    pop(stack);
    pop(stack);
    push(stack, Etoken);
  }

  if (rule_index == 0 ||
      rule_index == 2 ||
      rule_index == 3 ||
      rule_index == 4 ||
      rule_index == 5 ||
      rule_index == 7 ||
      rule_index == 8 ||
      rule_index == 9 ||
      rule_index == 10 ||
      rule_index == 11 ||
      rule_index == 12 ||
      rule_index == 13) {
    pop(stack);
    pop(stack);
    pop(stack);
    push(stack, Etoken);
  }

  // Token token;
  // token.token_type = T_TYPE_ID;
  // token.string_value->str = rules[rule_index].left_side;
  // push(stack, token);
}

void perform_reduce(TokenStack *stack, int *error) {

  token_type token_types[4] = {T_EMPTY, T_EMPTY, T_EMPTY, T_EMPTY};

  if (stack->top != -1)
    token_types[3] = stack->items[stack->top].token_type;

  if (stack->top > 0)
    token_types[2] = stack->items[stack->top - 1].token_type;

  if (stack->top > 1)
    token_types[1] = stack->items[stack->top - 2].token_type;

  if (stack->top > 2)
    token_types[0] = stack->items[stack->top - 3].token_type;

  // printf("top1: %s\n", tokens[0].string_value->str);
  // printf("top2: %s\n", tokens[1].string_value->str);
  // printf("top3: %s\n", tokens[2].string_value->str);
  // printf("top4: %s\n", tokens[3].string_value->str);


  int rule_index = get_rule_index(token_types);

  if (rule_index == -1) {
    *error = 1;
    return;
  }
  // printf("top4: %s\n", tokens[3].string_value->str);

  perform_rule(rule_index, stack);

  // while (top.token_type != T_LPAR) {
    // top = pop(stack);
  // }
  // pop(stack);
  // push(stack, top);
}


