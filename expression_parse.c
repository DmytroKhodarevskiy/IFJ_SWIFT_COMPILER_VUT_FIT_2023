#include "expression_parse.h"

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

int count_of_token_before_edge(TokenStack stack){
    int count = 0;
    for(int i = stack.top; stack.items[i].token_type != T_RD_EDGE; i--){
      if(i == 0 && count == 0) return -1;
      count++;
    }
    return count;
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
    case T_SING_STRING:
      return 7;
    default:
      return 8;
  }
}


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