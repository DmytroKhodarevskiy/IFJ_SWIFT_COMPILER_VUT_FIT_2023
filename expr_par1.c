#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "expression_parse.h"

#include "stack.c"
#include "tokenizer.c"

#define size 9

int precedence_table[size][size] = {

    //        +-       */    rel      !       ??      (       )        i       $
    {R,  S,  R,  S,  R,  S,  R,  S,  R},  // +-
    {R,  R,  R,  S,  R,  S,  R,  S,  R},  // */
    {S,  S,  E,  S,  R,  S,  R,  S,  R},  // rel
    {R,  R,  R,  E,  R,  E,  R,  E,  R},  // !
    {S,  S,  S,  S,  S,  S,  R,  S,  R},  // ??
    {S,  S,  S,  S,  S,  S,  EQ, S,  E}, // (
    {R,  R,  R,  R,  R,  E,  S,  E,  R}, // )
    {R,  R,  R,  R,  R,  E,  R,  E,  R}, // i
    {S,  S,  S,  S,  S,  S,  E,  S,  END}  // $

};
void print_stack(TokenStack stack) {
  for (int i = 0; i <= stack.top; i++) {
    if(stack.items[i].token_type == T_DOLLAR) printf("[$]\t");
    else if(stack.items[i].token_type == T_RD_EDGE) printf("[RD]\t");
    else printf("[%s]\t", stack.items[i].string_value->str);
  }
  printf("\n");
}
Token last_terminal(TokenStack stack) {
  //print_stack(stack);
  //printf("Token type: %s\n", stack.items[stack.top].string_value->str);
  for(int i = stack.top; i >= 0; i--){

    if(stack.items[i].token_type != T_NT ) return stack.items[i];
  }
  return stack.items[0];
}



void insert_edge(TokenStack *stack) {
  Token edgeToken = init_token();
  edgeToken.token_type = T_RD_EDGE;
  Token token;
  if(stack->items[stack->top].token_type == T_NT){
    token = pop(stack);
    push(stack, edgeToken);
    push(stack, token);
  }
  else{
    push(stack, edgeToken);
  }


}

int count_of_token_before_edge(TokenStack stack){
  int count = 0;
  for(int i = stack.top; stack.items[i].token_type != T_RD_EDGE; i--){
    count++;
    if(stack.items[i].token_type != T_DOLLAR && count == 0) return -1;

  }
  return count;
}

bool end_of_read(Token token, TokenStack stack) {
  if (token.token_type == T_LBRACE) {
    return true;
  }
  return false;
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
  case T_NT:
    return 7;
  case T_EOF:
  case T_DOLLAR:
    return 8;
  default:
    return 8;
  }
}

bool parse_expression(Token token, int *error, FILE** file) {
  TokenStack stack;
  initializeStack(&stack);
  Token Trow = init_token();
  Trow.token_type = T_DOLLAR;
  push(&stack, Trow);
  Token edgeToken = init_token();
  edgeToken.token_type = T_RD_EDGE;
  bool endRead = false;


  while (!endRead || stack.items[stack.top].token_type != T_NT || stack.top != 1) {
    if(!endRead) endRead = end_of_read(token, stack);
    //printf("End of read: %d\n", endRead);
    print_stack(stack);

    int column = get_index_from_token(token);

    int row = get_index_from_token(last_terminal(stack));
    // printf("Last terminal: %s\n", last_terminal(stack).string_value->str);

    Action_Letter action_letter = precedence_table[row][column];
    //printf("row: %d, column: %d, action_letter: %d\n", row, column, action_letter );
    //      print_stack(stack);
    //      printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
    //      if(action_letter == S)
    //        printf("Action: Shift\n");
    //      else if(action_letter == R)
    //      printf("Action: Reduce\n");
    //      else if(action_letter == EQ)
    //      printf("Action: Equal\n");
    //      else if(action_letter == E)
    //      printf("Action: Error\n");
    //
    //      printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);

    if (action_letter == S) {
      if(!endRead) {
        //printf("dfhdfhdfhdfhdfhdfhgdsgSHIFT\n");
        insert_edge(&stack);
        push(&stack, token);
      }
    }
    else if (action_letter == R) {
      // Token top = stack.items[stack.top];
      // while (top.token_type != T_LPAR) {
      // top = pop(&stack);
      // }
      //printf("dfhdfhdfhdfhdfhdfhgdsgREDUCE\n");
      if(perform_reduce(&stack, count_of_token_before_edge(stack)) == -1){
        *error = 1;
        return false;
      }

      if(!endRead && token.token_type != T_RPAR){
        print_stack(stack);
        insert_edge(&stack);
        push(&stack, token);}
    }

    else if (action_letter == EQ) {
      push(&stack, token);
      if(perform_reduce(&stack, count_of_token_before_edge(stack)) == -1){
        *error = 1;
        return false;
      }
    }

    else if (action_letter == E) {
      *error = 1;
      return false;
    }

    //      else if (action_letter == END) {
    //        print_stack(stack);
    //        return true;
    //      }

    //      if(endRead)
    //        printf("Action: End of read\n");
    if(!endRead || action_letter != S ) {
      if(token.token_type != T_RPAR || action_letter == EQ) token = get_token(*file);
    }
    //if(token.token_type == T_RPAR) printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
  }
  print_stack(stack);
  return true;
}

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

int get_rule_index(Token tokens[]) {
  int match = 0;

  //  printf("tokens: %s, %s, %s, %s\n", tokens[0].string_value->str, tokens[1].string_value->str, tokens[2].string_value->str, tokens[3].string_value->str);

  for (int i = 0; i < 14; i++) {

    for (int j = 0; j < 4; j++) {
      if (rules[i].right_side[j] == tokens[j].token_type) {
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

  if (rule_index == 1) {
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
    pop(stack);
    push(stack, Etoken);
  }
  if (rule_index == 6) {
    pop(stack);
    pop(stack);
    pop(stack);
    push(stack, Etoken);
  }

  //  Token token;
  //  token.token_type = T_TYPE_ID;
  //  token.string_value->str = rules[rule_index].left_side;
  //  push(stack, token);
}

//void perform_reduce(TokenStack *stack) {
//
//  Token top1, top2, top3, top4 = init_token();
//  top1.token_type = T_EMPTY;
//  top2.token_type = T_EMPTY;
//  top3.token_type = T_EMPTY;
//  top4.token_type = T_EMPTY;
//
//  if (stack->top == 0)
//      top1 = stack->items[stack->top];
//
//  if (stack->top > 0)
//      top2 = stack->items[stack->top - 1];
//
//  if (stack->top > 1)
//      top3 = stack->items[stack->top - 2];
//
//  if (stack->top > 2)
//      top4 = stack->items[stack->top - 3];
//
//
//  Token tokens[4] = {top1, top2, top3, top4};
//
//  int rule_index = get_rule_index(tokens);
//  printf("Rule index: %d\n", rule_index);
//
//  perform_rule(rule_index, stack);
//
//  // while (top.token_type != T_LPAR) {
//  // top = pop(stack);
//  // }
//  // pop(stack);
//  // push(stack, top);
//}

int perform_reduce(TokenStack *stack, int count) {
  //printf("Count: %d\n", count);


  //print_stack(*stack);
  Token tops[4];
  tops[0] = init_token();
  tops[1] = init_token();
  tops[2] = init_token();
  tops[3] = init_token();
  if(count == -1) return -1;
  else if (count == 1){
    //      if(stack->items[stack->top].token_type == T_TYPE_ID || stack->items[stack->top].token_type == T_DOUBLE || stack->items[stack->top].token_type == T_INT){
    tops[0] = stack->items[stack->top];
    //      }
    //      else return -1;
  }
  else if (count == 2){

    tops[0] = stack->items[stack->top - 1];
    tops[1] = stack->items[stack->top];
  }
  else if (count == 3){
    tops[0] = stack->items[stack->top- 2];
    tops[1] = stack->items[stack->top - 1];
    tops[2] = stack->items[stack->top];
  }

  int rule_index = get_rule_index(tops);
  //printf("Rule index: %d\n", rule_index);

  if(rule_index == -1) return -1;
  else perform_rule(rule_index, stack);
  // print_stack(*stack);

  //  int rule_index = get_rule_index(tokens);
  //  printf("Rule index: %d\n", rule_index);
  //  if(rule_index == -1){
  //      return -1;
  //  }
  //
  //
  //  perform_rule(rule_index, stack);

  // while (top.token_type != T_LPAR) {
  // top = pop(stack);
  // }
  // pop(stack);
  // push(stack, top);
  return 0;
}