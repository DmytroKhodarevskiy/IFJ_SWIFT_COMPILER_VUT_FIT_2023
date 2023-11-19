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
  {R,  R,  R,  R,  R,  E,  R,  E,  R}, // )
  {R,  R,  R,  R,  R,  E,  R,  E,  R}, // i
  {S,  S,  S,  S,  S,  S,  E,  S,  END}  // $

};

/**
 * Prints the current state of the token stack.
 *
 * @param stack The token stack to be printed.
 */
void print_stack(TokenStack stack) {
  printf("[$]\t");
  for (int i = 0; i <= stack.top; i++) {
    if(stack.items[i].token_type == T_RD_EDGE) printf("[RD]\t");
    else printf("[%s]\t", stack.items[i].string_value->str);
  }
  printf("\n");
}

/**
 * Finds the last terminal token in the stack.
 *
 * @param stack The stack from which to find the last terminal token.
 * @return The last terminal token if found; otherwise, a token indicating an empty or error state.
 */
Token last_terminal(TokenStack stack) {
  Token token = init_token();
  token.token_type = T_EMPTY;
  if(isEmpty(&stack)) {
    return token;
  }
  for(int i = stack.top; i >= 0; i--){
    if(stack.items[i].token_type != T_NT ) return stack.items[i];
  }
  return token;
}

/**
 * Inserts a reduction edge token in the stack. If the top token is non-terminal,
 * the edge token is inserted before it; otherwise, it's placed on top of the stack.
 *
 * @param stack A pointer to the stack where the edge token will be inserted.
 */
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

/**
 * Counts the number of tokens before encountering a reduction edge token in the stack.
 *
 * @param stack The stack to be scanned for the count.
 * @return The number of tokens before the reduction edge token; -1 if edge not found.
 */
int count_of_token_before_edge(TokenStack stack){
    int count = 0;
    for(int i = stack.top; stack.items[i].token_type != T_RD_EDGE; i--){
      if(i == 0 && count == 0) return -1;
      count++;
    }
    return count;
}

/**
 * Maps a given token to its corresponding index in the precedence table.
 *
 * @param token The token to be mapped.
 * @return The index of the token in the precedence table.
 */
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
    default:
      return 8;
  }
}

/**
 * Checks if the end of the input stream (represented by a specific token) is reached.
 *
 * @param token The token to check for the end-of-stream condition.
 * @return `true` if the end of the stream is reached; `false` otherwise.
 */
bool end_of_read(Token token) {
  if (get_index_from_token(token) == 8) {
      return true;
  }
  return false;
}

/**
 * Parses an expression using an operator precedence parsing method.
 *
 * @param token The current token to be parsed.
 * @param error A pointer to an integer where the error state will be stored (0 for no error, 1 for error).
 * @param file A pointer to the file stream from which tokens are read.
 * @return `true` if parsing is successful; `false` if an error occurs.
 */
bool parse_expression(Token token, int *error, FILE** file) {
  TokenStack stack;
  initializeStack(&stack);

  while (!end_of_read(token) || stack.items[stack.top].token_type != T_NT || stack.top != 0) {
      print_stack(stack);

      int column = get_index_from_token(token);
      int row = get_index_from_token(last_terminal(stack));

      Action_Letter action_letter = precedence_table[row][column];

      if (action_letter == S) {
        insert_edge(&stack);
        push(&stack, token);
        token = get_token(*file);
      }
      else if (action_letter == R) {
        if(perform_reduce(&stack, count_of_token_before_edge(stack)) == -1){
          *error = 1;
          return false;
        }
      }
      else if (action_letter == EQ) {
          push(&stack, token);
          token = get_token(*file);
      }
      else if (action_letter == E) {
        *error = 1;
        return false;
      }
  }
  print_stack(stack);
  return true;
}

/**
 * Determines the index of the grammar rule to be applied based on the given tokens.
 *
 * @param tokens An array of tokens to be analyzed.
 * @param count The number of tokens in the array.
 * @return The index of the grammar rule; -1 if no rule matches.
 */
int get_rule_index(Token tokens[], int count) {
  switch (count) {
    case 1:
      // E -> i
      if(tokens[0].token_type == T_TYPE_ID || tokens[0].token_type == T_INT || tokens[0].token_type == T_DOUBLE || tokens[0].token_type == T_FLOAT) return 1;
      else return -1;
    case 2:
        // E -> E!
        if(tokens[0].token_type == T_NT && tokens[1].token_type == T_NOTNIL) return 6;
        else return -1;
    case 3:
        if(tokens[0].token_type == T_NT && tokens[2].token_type == T_NT)
        {
          switch (tokens[1].token_type) {
            // E -> E + E
            case T_PLUS:
                    return 2;
            // E -> E - E
            case T_MINUS:
                    return 3;
            // E -> E * E
            case T_MULTIPLY:
                    return 4;
            // E -> E / E
            case T_DIVIDE:
                    return 5;
            // E -> E < E
            case T_LESS:
                    return 7;
            // E -> E > E
            case T_GREATER:
                    return 8;
            // E -> E <= E
            case T_LESS_EQUAL:
                    return 9;
            // E -> E >= E
            case T_GREATER_EQUAL:
                    return 10;
            // E -> E == E
            case T_EQUAL:
                    return 11;
            // E -> E != E
            case T_NOT_EQUAL:
                    return 12;
            // E -> E ?? E
            case T_BINARY_OP:
                    return 13;
            default:
                    return -1;
            }
        }
        // E -> (E)
        else if(tokens[0].token_type == T_LPAR && tokens[1].token_type == T_NT && tokens[2].token_type == T_RPAR) return 0;
        else return -1;
        default:
        return -1;
        }
}

/**
 * Applies a grammar rule to the token stack based on the rule index.
 *
 * @param rule_index The index of the grammar rule to be applied.
 * @param stack A pointer to the token stack to which the rule will be applied.
 */
void perform_rule(int rule_index, TokenStack *stack) {

  Token Etoken = init_token();
  Etoken.string_value->str = "E";
  Etoken.token_type = T_NT;

  if (rule_index == 1) {
      pop(stack);
      pop(stack);
      push(stack, Etoken);
  }
  else if (rule_index == 6) {
      pop(stack);
      pop(stack);
      pop(stack);
      push(stack, Etoken);
  }
  else{
      pop(stack);
      pop(stack);
      pop(stack);
      pop(stack);
      push(stack, Etoken);
  }

}

/**
 * Performs a reduce operation on the stack based on the given count of tokens.
 *
 * @param stack A pointer to the stack on which the reduce operation will be performed.
 * @param count The number of tokens to be considered for the reduction.
 * @return 0 if the reduction is successful; -1 if an error occurs or no rule applies.
 */
int perform_reduce(TokenStack *stack, int count) {
  Token tops[3];
  tops[0] = init_token();
  tops[1] = init_token();
  tops[2] = init_token();
  if(count == -1) return -1;
  else if (count == 1){
    tops[0] = stack->items[stack->top];
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

  int rule_index = get_rule_index(tops, count);

  if(rule_index == -1) return -1;
  else perform_rule(rule_index, stack);
  return 0;
}