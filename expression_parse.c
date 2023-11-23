#include "expression_parse.h"

#define size 9


int precedence_table[size][size] = {

//         +-      */      rel     !       ??      (       )       i       $
  {R,  S,  R,  S,  R,  S,  R,  S,  R},  // +-
  {R,  R,  R,  S,  R,  S,  R,  S,  R},  // */
  {S,  S,  E,  S,  S,  S,  R,  S,  R},  // rel
  {R,  R,  R,  E,  R,  E,  R,  E,  R},  // !
  {S,  S,  R,  S,  S,  S,  R,  S,  R},  // ??
  {S,  S,  S,  S,  S,  S,  EQ, S,  E},  // (
  {R,  R,  R,  R,  R,  E,  R,  E,  R},  // )
  {R,  R,  R,  R,  R,  E,  R,  E,  R},  // i
  {S,  S,  S,  S,  S,  S,  E,  S,  END} // $

};


bool is_nullable(DataType type){
  if(type == TYPE_INT_NULLABLE || type == TYPE_DOUBLE_NULLABLE || type == TYPE_STRING_NULLABLE) return true;
  else return false;
}

bool restricted_operations_with_operation(DataType expression_type, char *operation){
  if((strcmp(operation, "+") == 0) || (strcmp(operation, "-") == 0) || (strcmp(operation, "*") == 0) || (strcmp(operation, "/") == 0)){
      if(is_nullable(expression_type)){
          printf("Error: Cannot use %s operator on nullable type\n", operation);
          return false;
      }
  }
  return true;
}
token_type convert_symType_to_tokenType(DataType type){
        switch (type) {
        case TYPE_INT:
          return T_INT;
        case TYPE_DOUBLE:
          return T_DOUBLE;
        case TYPE_STRING:
          return T_SING_STRING;
        case TYPE_NIL:
          return T_KEYWORD;
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
        case T_KEYWORD:
          return TYPE_NIL;
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


void print_expression_type(DataType expression_type){
  switch (expression_type) {
    case TYPE_INT:
      printf("int\n");
      break;
    case TYPE_DOUBLE:
      printf("double\n");
      break;
    case TYPE_UNKNOWN:
      printf("undefined\n");
      break;
    case TYPE_STRING:
      printf("string\n");
      break;
    case TYPE_NIL:
        printf("nil\n");
        break;
    case TYPE_BOOL:
        printf("bool\n");
    default:
      printf("error\n");
      break;
  }
}

DataType get_token_type(Token op1, Token op3, int rule_type){

  switch (rule_type) {
  case 1:
    if((op1.token_type == T_INT && op3.token_type == T_DOUBLE) || (op1.token_type == T_DOUBLE && op3.token_type == T_INT)){
      return TYPE_DOUBLE;
    }

    if(op1.token_type != op3.token_type) {
      return TYPE_UNKNOWN;
    }
    else return convert_tokenType_to_symType(op1.token_type);
  case 2:
    if(op1.token_type != op3.token_type) {
      return TYPE_UNKNOWN;
    }
  return TYPE_BOOL;
case 3:

    if(op1.token_type == T_KEYWORD && op3.token_type == T_KEYWORD) return TYPE_NIL;
    else if (op1.token_type == T_KEYWORD) return convert_tokenType_to_symType(op3.token_type);
    else if (op3.token_type == T_KEYWORD) return convert_tokenType_to_symType(op1.token_type);
    else if(op1.token_type != T_KEYWORD && op3.token_type != T_KEYWORD) {
          if(op1.token_type != op3.token_type) {
            return TYPE_UNKNOWN;
          }
          else return convert_tokenType_to_symType(op1.token_type);
    }
default:
        return TYPE_UNKNOWN;

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
    case T_KEYWORD:
      if(token.token_type == T_KEYWORD && (strcmp(token.string_value->str, "nil") != 0)) return 8;
      else return 7;
    default:
      return 8;
  }
}


DataType parse_expression(SymTable *table, Token *token, int *error, FILE** file) {
  TokenStack stack;
  initializeStack(&stack);
  DataType expression_type = TYPE_UNKNOWN;
  int eol;
  bool EOL = false;
  int row;
  int column;
  while (true) {
     // print_stack(stack);

      if(EOL) column = 8;
      else column = get_index_from_token(*token);
      row = get_index_from_token(last_terminal(stack));


      Action_Letter action_letter = precedence_table[row][column];
      //printf("Action: %d row: %d column: %d\n", action_letter, row, column);
      if (action_letter == S) {
        insert_edge(&stack);
        push(&stack, *token);
        eol = fgetc(*file);
        ungetc(eol, *file);
        if(eol == '\n' || eol == '\r') EOL = true;
        else *token = get_token(*file);

      }

      else if (action_letter == R) {
        if(perform_reduce(table, &stack, count_of_token_before_edge(stack), &expression_type) == -1){
          freeStack(&stack);
          *error = 1;
          printf("Error: Invalid token\n");
          return expression_type;
        }
      }
      else if (action_letter == EQ) {
          push(&stack, *token);
          eol = fgetc(*file);
          ungetc(eol, *file);
          if(eol == '\n' || eol == '\r') EOL = true;
          else *token = get_token(*file);
      }
      else if (action_letter == E) {
            freeStack(&stack);
            printf("Error: Invalid token\n");
            *error = 1;
        return expression_type;
      }
      else if (action_letter == END) {
            //printf("Expression type: %d\n", expression_type);
            //print_expression_type(expression_type);
           // printf("Expression OK\n");
        freeStack(&stack);
        return expression_type;
      }
  }
}

int get_rule_index(SymTable *table,Token tokens[], int count, DataType *expression_type) {
  switch (count) {
    case 1:
      // E -> i
      //if(tokens[0].token_type == T_KEYWORD && strcmp(tokens[0].string_value->str, "nil") == 0);
//      if(tokens[0].token_type == T_TYPE_ID) tokens[0].token_type = convert_symType_to_tokenType(search_SymTable(table, tokens[0].string_value->str)->data.dtype);
//      if(tokens[0].token_type == T_TYPE_ID || tokens[0].token_type == T_INT || tokens[0].token_type == T_DOUBLE || tokens[0].token_type == T_KEYWORD || tokens[0].token_type == T_SING_STRING) {
//        *expression_type = convert_tokenType_to_symType(tokens[0].token_type);
//        return 1;
//      }
        //printf("Token: %s\n", tokens[0].string_value->str);
        if(tokens[0].token_type == T_TYPE_ID){
          *expression_type = search_SymTable(table, tokens[0].string_value->str)->data.dtype;
          if(is_nullable(*expression_type)){
              if(search_SymTable(table,tokens[0].string_value->str)->data.isNil) *expression_type = TYPE_NIL;
          }
          return 1;
        }
        if(tokens[0].token_type == T_TYPE_ID || tokens[0].token_type == T_INT || tokens[0].token_type == T_DOUBLE || (tokens[0].token_type == T_KEYWORD && strcmp(tokens[0].string_value->str, "nil") == 0) || tokens[0].token_type == T_SING_STRING) {
          *expression_type = convert_tokenType_to_symType(tokens[0].token_type);
          //print_expression_type(*expression_type);
          return 1;
        }
        else return -1;
    case 2:
        // E -> E!
        //print_expression_type(*expression_type);
        if(*expression_type == TYPE_NIL){
          *expression_type = TYPE_UNKNOWN;
            fprintf(stderr, "Error: Cannot use ! operator on nil\n");
            return -1;
        }
        if(tokens[0].grammar_token_type == T_NT && tokens[1].token_type == T_NOTNIL) {
            if (!is_nullable(*expression_type)) {
                fprintf(stderr, "Error: Cannot use ! operator on non-nullable type\n");
                return -1;
            }
            *expression_type -= 4;
            return 6;
        }
        else return -1;
    case 3:

        if(tokens[0].grammar_token_type == T_NT && tokens[2].grammar_token_type == T_NT)
        {
            restricted_operations_with_operation(*expression_type, tokens[1].string_value->str);
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
                    *expression_type = get_token_type(tokens[0], tokens[2], 3);
                    return 13;
            default:
                    return -1;
            }
        }
        // E -> (E)
        else if(tokens[0].token_type == T_LPAR && tokens[1].grammar_token_type == T_NT && tokens[2].token_type == T_RPAR){
//            *expression_type = *expression_type;
            return 0;
        }
        else return -1;
        default:
        return -1;
        }
}

void perform_rule(int rule_index, TokenStack *stack, DataType *expression_type) {

  Token Etoken = init_token();
  Etoken.string_value->str = "E";
  Etoken.grammar_token_type = T_NT;
  Etoken.token_type = convert_symType_to_tokenType(*expression_type);

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

int perform_reduce(SymTable *table,TokenStack *stack, int count, DataType *expression_type) {
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