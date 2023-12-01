#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
// #include "tokenizer.c"
// #include "memory.c"
// #include "symtable.c"
#include "expression_parse.c"

SymTable *symbol_table;
Token current_token;

void Match(FILE *file, Token token){
    Token token_to_match = peekNextToken(file);
    if (token_to_match.token_type != token.token_type){
        exitWithError("Syntax error: token type mismatch\n", ERR_SYNTAX);
    }
    else {
        get_token(file);
    }
}

void Parse(FILE *file){
    // printf("Parsing...\n");
    PROGRAM(file);
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