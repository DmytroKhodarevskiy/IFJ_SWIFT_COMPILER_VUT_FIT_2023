#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "symtable.c"

#include "expression_parse.h"

#include "stack.c"
#include "tokenizer.c"

#define size 9


int precedence_table[size][size] = {

// +-  */  rel  !  ??  (    )  i   $
  {R,  S,  R,  S,  R,  S,  R,  S,  R},  // +-
  {R,  R,  R,  S,  R,  S,  R,  S,  R},  // */
  {S,  S,  E,  S,  R,  S,  R,  S,  R},  // rel
  {R,  R,  R,  E,  R,  E,  R,  E,  R},  // !
  {S,  S,  S,  S,  S,  S,  R,  S,  R},  // ??
  {S,  S,  S,  S,  S,  S,  EQ, S,  E},  // (
  {R,  R,  R,  R,  R,  E,  R,  E,  R},  // )
  {R,  R,  R,  R,  R,  E,  R,  E,  R},  // i
  {S,  S,  S,  S,  S,  S,  E,  S,  END} // $

};

//void int2Double(Token *token){
//  token->token_type = T_DOUBLE;
//}

token_type convert_symType_to_tokenType(DataType type){
        switch (type) {
        case TYPE_INT:
          return T_INT;
        case TYPE_DOUBLE:
          return T_DOUBLE;
        case TYPE_STRING:
          return T_SING_STRING;
        default:
            return T_EMPTY;
        }
}

DataType convert_tokenType_to_symType(token_type type){
        switch (type) {
        case T_INT:
          return TYPE_INT;
        case T_DOUBLE:
          return TYPE_DOUBLE;
        case T_SING_STRING:
          return TYPE_STRING;
        default:
            return TYPE_UNKNOWN;
        }
}

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
void print_expression_type(token_type expression_type){
  switch (expression_type) {
    case T_INT:
      printf("int\n");
      break;
    case T_DOUBLE:
      printf("double\n");
      break;
    case T_FLOAT:
      printf("float\n");
      break;
    case T_EMPTY:
      printf("empty\n");
      break;
    case T_SING_STRING:
      printf("string\n");
      break;
    default:
      printf("error\n");
      break;
  }
}

token_type get_token_type(Token op1, Token op3, int rule_type){
  Token EmptyToken = init_token();

  switch (rule_type) {
  case 1:
    if((op1.token_type == T_INT && op3.token_type == T_DOUBLE) || (op1.token_type == T_DOUBLE && op3.token_type == T_INT)){
      return T_DOUBLE;
    }
    if(op1.token_type != op3.token_type) {
      return T_EMPTY;
    }
    else return op1.token_type;
  case 2:
    if((op1.token_type == T_INT && op3.token_type == T_DOUBLE)){
      //TODO: convert int to double
      return T_INT;
    }
    if(op1.token_type != op3.token_type) {
      return T_EMPTY;
    }
    else return T_INT;;
case 3:

    default:
        return T_EMPTY;

  }
}



/**
 * Finds the last terminal token in the stack.
 *
 * @param stack The stack from which to find the last terminal token.
 * @return The last terminal token if found; otherwise, a token indicating an empty or error state.
 */
Token last_terminal(TokenStack stack) {
  Token token = init_token();
  if(isEmpty(&stack)) {
    return token;
  }
  for(int i = stack.top; i >= 0; i--){
    if(stack.items[i].grammar_token_type != T_NT ) return stack.items[i];
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

  if(stack->items[stack->top].grammar_token_type == T_NT){
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
    case T_SING_STRING:
      return 7;
    default:
      return 8;
  }
}


//**
//    * @brief Function determines correct token return type when performing all operations
//    *
//    * @param top pointer to top item on expression stack
//    * @param third pointer to third item on expression stack
//    * @param operation token type of currrent operation
//    * @param table pointer to symbol table
//    * @return correct token type from all operations
//    */
//    tTokenType getResultType(tSymTable *table, tExpression *top, tExpression *third, tTokenType operation)
//{
//  tTokenType retType;
//  switch (operation)
//  {
//  case tDiv:
//
//      // Converts null constants to 0.0, if second operand is null, throws error.
//      if (isNull(table, top))
//        errorExit("Division by zero.\n", CERR_SEM_OTHER);
//      if (isNull(table, third))
//        convertNullToFloat(table, third);
//      if (isInt(table, top) && !isNonTerminal(top))
//        convertIntToFloat(table, top);
//      if (isInt(table, third) && !isNonTerminal(third))
//        convertIntToFloat(table, third);
//
//      retType = tTypeFloat;
//      break;
//
//  case tPlus:
//  case tMinus:
//  case tMul:
//
//      // Converts null constants to 0 or 0.0 depending on type of the other operand. If both are null, int is returned.
//      if (isNull(table, top) && isNull(table, third))
//      {
//        convertNullToInt(table, top);
//        convertNullToInt(table, third);
//      }
//      else if (isNull(table, top))
//      {
//        if (isInt(table, third))
//          convertNullToInt(table, top);
//        else
//          convertNullToFloat(table, top);
//      }
//      else if (isNull(table, third))
//      {
//        if (isInt(table, top))
//          convertNullToInt(table, third);
//        else
//          convertNullToFloat(table, third);
//      }
//
//      retType = intOrFloat(table, top, third);
//      if (retType == tNone)
//        errorExit("Result type tNone, getResType.\n", CERR_INTERNAL);
//      break;
//
//  case tConcat:
//      retType = tTypeString;
//      break;
//
//  case tLess:
//  case tLessEq:
//  case tMore:
//  case tMoreEq:
//  case tIdentical:
//  case tNotIdentical:
//
//      retType = tTypeInt;
//      break;
//
//  default:
//      // Should never get here.
//      retType = tNone;
//  }
//  return retType;
//}

/**
 * Parses an expression using an operator precedence parsing method.
 *
 * @param token The current token to be parsed.
 * @param error A pointer to an integer where the error state will be stored (0 for no error, 1 for error).
 * @param file A pointer to the file stream from which tokens are read.
 * @return `true` if parsing is successful; `false` if an error occurs.
 */
DataType parse_expression(SymTable *table, Token *token, int *error, FILE** file) {

  TokenStack stack;
  initializeStack(&stack);
  token_type expression_type = T_EMPTY;

  while (true) {
      //print_stack(stack);
      int column = get_index_from_token(*token);
      int row = get_index_from_token(last_terminal(stack));

      Action_Letter action_letter = precedence_table[row][column];

      if (action_letter == S) {
        insert_edge(&stack);
        push(&stack, *token);
        *token = get_token(*file);
      }

      else if (action_letter == R) {
        if(perform_reduce(table, &stack, count_of_token_before_edge(stack), &expression_type) == -1){
          *error = 1;
          printf("Error: Invalid token\n");
          return convert_tokenType_to_symType(expression_type);
        }
      }
      else if (action_letter == EQ) {
          push(&stack, *token);
          *token = get_token(*file);
      }
      else if (action_letter == E) {
        printf("Error: Invalid token\n");
        *error = 1;
        return convert_tokenType_to_symType(expression_type);
      }
      else if (action_letter == END) {
        print_expression_type(expression_type);
        printf("Expression OK\n");
        return convert_tokenType_to_symType(expression_type);
      }
  }
}

/**
 * Determines the index of the grammar rule to be applied based on the given tokens.
 *
 * @param tokens An array of tokens to be analyzed.
 * @param count The number of tokens in the array.
 * @return The index of the grammar rule; -1 if no rule matches.
 */
int get_rule_index(SymTable *table,Token tokens[], int count, token_type *expression_type) {
  switch (count) {
    case 1:
      // E -> i
      if(tokens[0].token_type == T_KEYWORD && strcmp(tokens[0].string_value->str, "nil") == 0)
      if(tokens[0].token_type == T_TYPE_ID) tokens[0].token_type = convert_symType_to_tokenType(search_SymTable(table, tokens[0].string_value->str)->data.dtype);
      if(tokens[0].token_type == T_TYPE_ID || tokens[0].token_type == T_INT || tokens[0].token_type == T_DOUBLE || tokens[0].token_type == T_FLOAT || tokens[0].token_type == T_SING_STRING) {
        *expression_type = tokens[0].token_type;
        return 1;
      }
      else return -1;
    case 2:
        // E -> E!
        if(tokens[0].grammar_token_type == T_NT && tokens[1].token_type == T_NOTNIL) return 6;
        else return -1;
    case 3:

        if(tokens[0].grammar_token_type == T_NT && tokens[2].grammar_token_type == T_NT)
        {
          switch (tokens[1].token_type) {
            // E -> E + E
            case T_PLUS:
            *expression_type = get_token_type(tokens[0], tokens[2], 1);
                    return 2;
            // E -> E - E
            case T_MINUS:
                    *expression_type = get_token_type(tokens[0], tokens[2], 1);
                    return 3;
            // E -> E * E
            case T_MULTIPLY:
                    *expression_type = get_token_type(tokens[0], tokens[2], 1);
                    return 4;
            // E -> E / E
            case T_DIVIDE:
                    *expression_type = get_token_type(tokens[0], tokens[2], 1);
                    return 5;
            // E -> E < E
            case T_LESS:
                    *expression_type = get_token_type(tokens[0], tokens[2], 2);
                    return 7;
            // E -> E > E
            case T_GREATER:
                    *expression_type = get_token_type(tokens[0], tokens[2], 2);
                    return 8;
            // E -> E <= E
            case T_LESS_EQUAL:
                    *expression_type =  get_token_type(tokens[0], tokens[2], 2);
                    return 9;
            // E -> E >= E
            case T_GREATER_EQUAL:
                    *expression_type = get_token_type(tokens[0], tokens[2], 2);
                    return 10;
            // E -> E == E
            case T_EQUAL:
                    *expression_type = get_token_type(tokens[0], tokens[2], 2);
                    return 11;
            // E -> E != E
            case T_NOT_EQUAL:
                    *expression_type = get_token_type(tokens[0], tokens[2], 2);
                    return 12;
            // E -> E ?? E
            case T_BINARY_OP:
                    return 13;
            default:
                    return -1;
            }
        }
        // E -> (E)
        else if(tokens[0].token_type == T_LPAR && tokens[1].grammar_token_type == T_NT && tokens[2].token_type == T_RPAR){
            *expression_type = tokens[1].token_type;
            return 0;
        }
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
void perform_rule(int rule_index, TokenStack *stack, token_type *expression_type) {

  Token Etoken = init_token();
  Etoken.string_value->str = "E";
  Etoken.grammar_token_type = T_NT;
  Etoken.token_type = *expression_type;

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
int perform_reduce(SymTable *table,TokenStack *stack, int count, token_type *expression_type) {
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
    tops[0] = stack->items[stack->top - 2];
    tops[1] = stack->items[stack->top - 1];
    tops[2] = stack->items[stack->top];
  }

  int rule_index = get_rule_index(table,tops, count, expression_type);

  if(rule_index == -1) {
    return -1;
  } 
  else perform_rule(rule_index, stack, expression_type);
  return 0;
}