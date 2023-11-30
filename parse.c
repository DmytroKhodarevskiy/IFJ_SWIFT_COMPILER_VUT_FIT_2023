#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
// #include "tokenizer.c"
// #include "memory.c"
// #include "symtable.c"
#include "expression_parse.c"
#include "symtable_stack.c" // krasava

SymTable *global_symtable;
SymTable *symbol_table;
Token current_token;
int PHASE = 1;

void Parse(FILE *file){
    // printf("Parsing...\n");
    // PROGRAM(file);
    PHASE_FIRST(file);
}

// Read Global Symbol Table and Functions Symbol Tables
void PHASE_FIRST(FILE *file){
    // printf("Parsing...\n");
    SymStack *stack;
    initializeStack(stack); 

    global_symtable = create_SymTable();
    global_symtable->name = "global";

    s_push(stack, *global_symtable);

    FILL_TREES(file, stack);

}

DataType get_type(char *str) {
  if (strcmp(str, "Int") == 0) {
    return TYPE_INT;
  }
  else if (strcmp(str, "Double") == 0) {
    return TYPE_DOUBLE;
  }
  else if (strcmp(str, "String") == 0) {
    return TYPE_STRING;
  }
  else if (strcmp(str, "Int?") == 0) {
    return TYPE_INT_NULLABLE;
  }
  else if (strcmp(str, "Double?") == 0) {
    return TYPE_DOUBLE_NULLABLE;
  }
  else if (strcmp(str, "String?") == 0) {
    return TYPE_STRING_NULLABLE;
  }
  else {
    return TYPE_UNKNOWN;
  }

}

bool is_compatible(DataType assignType, DataType variableType) {
  switch (assignType) {
    case TYPE_INT:
      return variableType == TYPE_INT;
    case TYPE_DOUBLE:
      return variableType == TYPE_DOUBLE ||  variableType == TYPE_INT;
    case TYPE_STRING:
      return variableType == TYPE_STRING;
    case TYPE_BOOL:
      return variableType == TYPE_BOOL;
    case TYPE_INT_NULLABLE:
      return variableType == TYPE_INT || variableType == TYPE_NIL;
    case TYPE_DOUBLE_NULLABLE:
      return variableType == TYPE_DOUBLE ||  variableType == TYPE_NIL  || variableType == TYPE_INT;
    case TYPE_STRING_NULLABLE:
      return variableType == TYPE_STRING || variableType == TYPE_NIL;
    default:
      return false;
  }
}

void FILL_TREES(FILE *file, SymStack *stack){
    // printf("FILLING TREES...\n");
    current_token = get_token(file); // get first token
    SymTable current_symtable = stack->items[stack->top];
    // printf("token: %s\n", current_token.string_value->str);

    while (current_token.token_type != T_EOF) {

      char *str = current_token.string_value->str;

      //current token is keyword let
      if (current_token.token_type == T_KEYWORD &&
          (!strcmp(current_token.string_value->str, "let") ||
           !strcmp(current_token.string_value->str, "var")) &&
          !strcmp(current_symtable.name, "global")) {  
            SymData node_data;

            // strcmp(current_token.string_value->str, "var") == 0 ? node_data.canbeChanged = true : node_data.canbeChanged = false;
            node_data.canbeChanged = (strcmp(current_token.string_value->str, "var") == 0) ? true : false;

            current_token = get_token(file); // get id

            char *id_name = current_token.string_value->str;

            if (search_SymTable(global_symtable, id_name) != NULL) {
              exitWithError("Syntax error: variable already declared\n", ERR_SEMANT_FUNC_ARG); 
            }

            node_data.name = current_token.string_value->str;
            node_data.isGlobal = true;
            node_data.isFunction = false;
            node_data.returnType = TYPE_UNKNOWN;
            node_data.paramTypes.next = NULL;
            node_data.local_SymTable = NULL;
            node_data.paramCount = 0;

            current_token = peekNextToken(file); // peek : or =

            if (current_token.token_type == T_COLON){
              current_token = get_token(file); // get :
              current_token = get_token(file); // get type

              node_data.dtype = get_type(current_token.string_value->str);
              if (node_data.dtype == TYPE_UNKNOWN){
                exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
              }

              current_token = peekNextToken(file); // peek =
              if (current_token.token_type == T_ASSIGN){
                current_token = get_token(file); // get =
                current_token = get_token(file); // get exp
                int error = 0;
                //change to symstack
                DataType type = parse_expression(stack, &current_token, &error, &file);
                if (type == TYPE_UNKNOWN){
                  exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
                }

                if (type == TYPE_NIL) {
                  node_data.isNil = true;
                }

                if (!is_compatible(node_data.dtype, type)) {
                  exitWithError("Syntax error: type mismatch\n", ERR_SEMANT_TYPE);
                }
                // if (type != node_data.dtype){
                // }
              }

              else {
                exitWithError("Syntax error: expected =\n", ERR_SYNTAX);
              }

            }

            else if (current_token.token_type == T_ASSIGN){
              current_token = get_token(file); // get =
              current_token = get_token(file); // get exp
              int error = 0;
              //change to symstack
              DataType type = parse_expression(stack, &current_token, &error, &file);
              if (type == TYPE_UNKNOWN){
                exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
              }

              if (type == TYPE_NIL) {
                node_data.isNil = true;
              }

              node_data.dtype = type;
            }

            else {
              exitWithError("Syntax error: expected : or =\n", ERR_SYNTAX);
            }

            insert_SymTable(global_symtable, id_name,node_data);

      }
    
      if (current_token.token_type == T_LBRACE) { // current token is {
        int count = 1;
        while (count != 0) {
          current_token = get_token(file);
          if (current_token.token_type == T_LBRACE) {
            count++;
          }
          else if (current_token.token_type == T_RBRACE) {
            count--;
          }

          if (current_token.token_type == T_EOF) {
            exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
          }
        }
      }
    }
} 













void PROGRAM(FILE *file){
    DECLARE_GLOBAL_FUNC(file);
}

void DECLARE_GLOBAL_FUNC(FILE *file){
  symbol_table = create_SymTable();
  //init the tree, set main return type
  STMT_LIST(file);
}

void STMT_LIST(FILE *file){
  // printf("STMT_LIST\n");
  // current_token= peekNextToken(file);
  
  // if (current_token.token_type == T_EOF ||
  //  current_token.token_type == T_RBRACE){
    // return;
  // }
  
  current_token = peekNextToken(file);
  // printf("token list asd awd: %s\n", current_token.string_value->str);
  
  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_KEYWORD)) return;

      // printf("token: %s\n", token.string_value->str);

  STMT(file);
  STMT_LIST(file);
}

void STMT(FILE *file){
  current_token = get_token(file); //if, while, return, let, var, id, func
  char *str = current_token.string_value->str;

  // printf("token_type: %d, token: %s\n", current_token.token_type, str);


  if (current_token.token_type == T_KEYWORD &&
      strcmp(str, "if") == 0){

          IF_EXP(file);

          current_token = get_token(file); // get {
          if (current_token.token_type != T_LBRACE){
            exitWithError("Syntax error: expected on if {\n", ERR_SYNTAX);
          }


          if (current_token.token_type != T_RBRACE) {
            
            STMT_LIST(file);

            current_token = get_token(file); // get }
            
            if (current_token.token_type != T_RBRACE){
              exitWithError("Syntax error: expected on if }\n", ERR_SYNTAX);
            }
          }

          else {
            current_token = get_token(file); // get }
            if (current_token.token_type != T_RBRACE){
              exitWithError("Syntax error: expected on if }\n", ERR_SYNTAX);
            }
          }


          current_token = get_token(file); // get else

          if (current_token.token_type == T_KEYWORD &&
              strcmp(current_token.string_value->str, "else") == 0){

                current_token = get_token(file); // get {
                if (current_token.token_type != T_LBRACE){
                  exitWithError("Syntax error: expected on else {\n", ERR_SYNTAX);
                }

                current_token = peekNextToken(file);
                if (current_token.token_type != T_RBRACE){
                  STMT_LIST(file);
                  if (current_token.token_type != T_RBRACE){
                    exitWithError("Syntax error: expected on else }\n", ERR_SYNTAX);
                  }
                  current_token = get_token(file); // get }
                }
                else {
                  current_token = get_token(file); // get }
                }

          }
          
          else {
            exitWithError("Syntax error: expected else\n", ERR_SYNTAX);
          }
      }

  else if (current_token.token_type == T_KEYWORD &&
           ((strcmp(str, "let") == 0) ||
            (strcmp(str, "var") == 0))) {  // let or var
          // get_token(file); // get let

          current_token = get_token(file); // get id
          if (current_token.token_type != T_TYPE_ID){
            exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
          }

          // printf("tokenawdawdawd: %s\n", current_token.string_value->str);

          MB_STMT_LET_VAR(file);
      }

  else if (current_token.token_type == T_KEYWORD &&
           (strcmp(str, "return") == 0)) { // return

            current_token = peekNextToken(file); // get EXP or }

            if (current_token.token_type == T_RBRACE){
              return;
            }

            else {
              EXP(file);
              // current_token = get_token(file); // get }
            }
      }

  else if (current_token.token_type == T_KEYWORD &&
           (strcmp(str, "while") == 0)) { // while

           WHILE_EXP(file);

           current_token = get_token(file); // get {

           if (current_token.token_type != T_LBRACE){
             exitWithError("Syntax error: expected on while {\n", ERR_SYNTAX);
           }

           current_token = peekNextToken(file);
           if (current_token.token_type != T_RBRACE){
             STMT_LIST(file);
             if (current_token.token_type != T_RBRACE){
               exitWithError("Syntax error: expected on while }\n", ERR_SYNTAX);
             }
             current_token = get_token(file); // get }
           }
           else {
             current_token = get_token(file); // get }
           }
      }  

  else if (current_token.token_type == T_KEYWORD &&
            (strcmp(str, "func") == 0)) { // func

              current_token = get_token(file); // get id
              if (current_token.token_type != T_TYPE_ID){
                exitWithError("Syntax error: expected function name\n", ERR_SYNTAX);
              }

              current_token = get_token(file); // get (
              if (current_token.token_type != T_LPAR){
                exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
              }

              PARAM_LIST(file);
              // WORKING HERE
              /////////////////
              /////////////////
              /////////////////

              // printf("token: %s\n", current_token.string_value->str);

              // current_token = get_token(file); // get )
              if (current_token.token_type != T_RPAR){
                exitWithError("Syntax error: expected )\n", ERR_SYNTAX);
              }

              // current_token = get_token(file); // get -> or {
              RETURN_TYPE(file);

              current_token = get_token(file); // get {
              if (current_token.token_type != T_LBRACE){
                exitWithError("Syntax error: expected {\n", ERR_SYNTAX);
              }

              STMT_LIST(file);

              // printf("tokenfuncccccc: %s\n", current_token.string_value->str);
              current_token = get_token(file); // get }
              if (current_token.token_type != T_RBRACE){
                exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
              }

              // printf("tokenfuncccccc: %s\n", current_token.string_value->str);
              // get

            }

  else if (current_token.token_type == T_TYPE_ID) { // id
            ASSIGN_STMT_OR_FUNCALL(file);        
        
          }

  else {
    // return;
    exitWithError("Syntax error: expected if, while, return, let, var, id, func\n", ERR_SYNTAX);
  }
}

void ASSIGN_STMT_OR_FUNCALL(FILE *file){ //current token is id
  current_token = peekNextToken(file); // peek = or (

  if (current_token.token_type == T_ASSIGN){
    current_token = get_token(file); // get =
    EXP(file);
  }

  else if (current_token.token_type == T_LPAR) {
    current_token = get_token(file); // get (
    FUNC_CALLS(file);
    return;
  } 

  else {
    exitWithError("Syntax error: expected = or (\n", ERR_SYNTAX);
  }
}

void FUNC_CALLS(FILE *file){ //current token is (
  // current_token = get_token(file); // get (
  if (current_token.token_type != T_LPAR){
    exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
  }

  ARG_LIST(file);

  current_token = get_token(file); // get )
  if (current_token.token_type != T_RPAR){
    exitWithError("Syntax error: expected ) or ,\n", ERR_SYNTAX);
  }
}

void ARG_LIST(FILE *file){ //current token is (
   current_token = peekNextToken(file);

  if (!(current_token.token_type == T_COLON ||
      current_token.token_type == T_TYPE_ID)) return;

  ARG(file);

  current_token = peekNextToken(file);
  if (current_token.token_type == T_COMMA){
    current_token = get_token(file); // get ,
    ARG_LIST(file);
  }

  else {
    // current_token = get_token(file); // get )
    return;
  }
}

void ARG(FILE *file){ //current token is (

  PREFIX(file);

  current_token = get_token(file); // get :
  if (current_token.token_type != T_COLON){
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  // printf("token befroe: %s\n", current_token.string_value->str);
  current_token = get_token(file); // get id
  // printf("token after: %s\n", current_token.string_value->str);
  if (current_token.token_type != T_TYPE_ID){
    exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
  }
}

void PREFIX(FILE *file){ //current token is (
  current_token = peekNextToken(file); // get id

  if (current_token.token_type == T_TYPE_ID) {
    current_token = get_token(file);
    return;
  }

  else 
    return;

}

void RETURN_TYPE(FILE *file) { // current token is )
  current_token = peekNextToken(file); // get -> or {

  if (current_token.token_type == T_ARROW) {
    current_token = get_token(file); // get ->
    TYPE(file);
  }

  else {
    return;
  }
}

void PARAM_LIST(FILE *file){ //current token is (

  current_token = peekNextToken(file);

  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)) return;

  PARAM(file);

  current_token = peekNextToken(file);
  if (current_token.token_type == T_COMMA){
    current_token = get_token(file); // get ,
    PARAM_LIST(file);
  }

  else {
    current_token = get_token(file); // get )
    return;
  }
}

void PARAM(FILE *file){ //current token is (

  PARAM_PREFIX(file);

  PARAM_NAME(file);

  current_token = get_token(file); // get :
  if (current_token.token_type != T_COLON){
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  TYPE(file);
}

void PARAM_NAME(FILE *file) { //current token is id (prefix)

  current_token = get_token(file); // get id
  if (current_token.token_type != T_TYPE_ID){
    exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
  }
}

void PARAM_PREFIX(FILE *file){ //current token is (

  current_token = get_token(file); // get id
  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)){
    exitWithError("Syntax error: expected id or _\n", ERR_SYNTAX);
  }

}

void MB_STMT_LET_VAR(FILE *file){ //current token is id

  // printf("HELLLLLPPPPPP\n");

  current_token = get_token(file); // get : or =

  if (current_token.token_type == T_COLON){
    TYPE(file);
    MB_ASSIGN_EXPR(file);
    // printf("token, var exp ended on is: %s\n", current_token.string_value->str);
  }

  else if (current_token.token_type == T_ASSIGN){
    EXP(file);
  }


  else {
    exitWithError("Syntax error: expected : or =\n", ERR_SYNTAX);
  }
}

void MB_ASSIGN_EXPR(FILE *file){ //current token is keyword
  current_token = peekNextToken(file); // check if next token is =

  if (current_token.token_type == T_ASSIGN){
    current_token = get_token(file); // get =
    EXP(file);
  }

  else {
    // printf("token, var exp ended on is: %s\n", current_token.string_value->str);
    return;
  }
}

void TYPE(FILE *file){ //current token is :
  current_token = get_token(file); // get type
  char *str = current_token.string_value->str;

  if (current_token.token_type == T_KEYWORD &&
       ((strcmp(str, "String") == 0) ||
       (strcmp(str, "Int") == 0) ||
       (strcmp(str, "Double") == 0) ||
       (strcmp(str, "String?") == 0) ||
       (strcmp(str, "Double?") == 0) ||
       (strcmp(str, "Int?") == 0))){

        return;
  }

  else {
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }
}

void WHILE_EXP(FILE *file){ //current token is while
  // current_token = get_token(file); // get (
  current_token = peekNextToken(file); // get (
  char *str = current_token.string_value->str;

  if (current_token.token_type == T_LPAR){
          // get_token(file); // get (
          EXP(file);
          // get_token(file); // get )
  }

  else {
    exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
  }
}

void IF_EXP(FILE *file) { //current token is if
  // current_token = get_token(file); // get ( or let
  current_token = peekNextToken(file); // get ( or let
  char *str = current_token.string_value->str;

  // printf("token: %s\n", str);

  if (current_token.token_type == T_LPAR) {
          // get_token(file); // get (
          EXP(file);
          // get_token(file); // get )
  }

  else if (current_token.token_type == T_KEYWORD &&
      strcmp(str, "let") == 0)        {
      current_token = get_token(file); // get let

      // printf("token: %s\n", current_token.string_value->str);

      current_token = get_token(file); // get id
      if (current_token.token_type != T_TYPE_ID){
        exitWithError("Syntax error: expected id on let\n", ERR_SYNTAX);
      }
      // printf("token: %s\n", current_token.string_value->str);

      // current_token = get_token(file); // get {
      // printf("token on iffffffffff: %s\n", current_token.string_value->str);
  }

  else {
    exitWithError("Syntax error: expected ( or let\n", ERR_SYNTAX);
  }
}

void EXP(FILE *file){ 
  // Token start_exp_token = get_token(file);
  current_token = get_token(file);

  // printf("EXP: %s\n", current_token.string_value->str);
  int error = 0;
  DataType type = parse_expression(symbol_table, &current_token, &error, &file);
  // printf("TYpe after exp: %d\n", current_token.token_type);
  if (type == TYPE_UNKNOWN){
    exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
  }
}