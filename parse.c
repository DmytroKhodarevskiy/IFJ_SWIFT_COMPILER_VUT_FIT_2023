#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
// #include "tokenizer.c"
// #include "memory.h"
// #include "symtable.c"
// #include "expression_parse.c"
// #include "symtable_stack.c" // krasava

SymTable *global_symtable;
// SymTable *symbol_table;
SymStack stack;

Token current_token;
int PHASE = 1;

void Parse(FILE *file){
    // printf("Parsing...\n");
    // PROGRAM(file);
    PHASE_FIRST(file);
    PHASE = 2;
    linenum = 0;
    rewind(file);

    printf("FIRST PHASE DONE\n");

    printf("-----------------------------------------------------------------------------------------------\n");
    print_SymTable(global_symtable);
    printf("-----------------------------------------\n");
    print_SymTable(&(stack.items[stack.top]));
    printf("-----------------------------------------------------------------------------------------------\n");


    PHASE_SECOND(file);
    // print_SymTable(global_symtable);
}

void PHASE_SECOND(FILE *file) {
  PROGRAM(file);
}

// Read Global Symbol Table and Functions Symbol Tables
void PHASE_FIRST(FILE *file){
    // SymStack stack;


    s_initializeStack(&stack); 


    global_symtable = create_SymTable();
    global_symtable->name = "global";

    s_push(&stack, *global_symtable);

    FILL_TREES(file, &stack);

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
    current_token = get_token(file); // get first token

    SymTable current_symtable = stack->items[stack->top];


    while (current_token.token_type != T_EOF) {

      char *str = current_token.string_value->str;

      if (current_token.token_type == T_KEYWORD || current_token.token_type == T_LBRACE ||
          current_token.token_type == T_RBRACE) {
        //current token is keyword let or var, in global scope
        if (current_token.token_type == T_KEYWORD &&
            (!strcmp(current_token.string_value->str, "let") ||
            !strcmp(current_token.string_value->str, "var")) &&
            !strcmp(current_symtable.name, "global")) {  
              SymData node_data;

              node_data = initSymData();

              // strcmp(current_token.string_value->str, "var") == 0 ? node_data.canbeChanged = true : node_data.canbeChanged = false;
              node_data.canbeChanged = (!strcmp(current_token.string_value->str, "var")) ? true : false;

              current_token = get_token(file); // get id
              if (current_token.token_type != T_TYPE_ID){
                exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
              }

              char *id_name = current_token.string_value->str;

              if (search_SymTable(global_symtable, id_name) != NULL) {
                exitWithError("Semantic error: variable already declared\n", ERR_SEMANT_FUNC_ARG); 
              }

              node_data.name = id_name;
              node_data.isGlobal = true;
              node_data.isFunction = false;
              // node_data.local_SymTable = NULL;
              node_data.isDefined = true;

              current_token = peekNextToken(file); // peek : or =



              if (current_token.token_type == T_COLON) {
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
                  // DataType type = parse_expression(stack, &current_token, &error, &file);
                  // printf("token: %s\n", current_token.string_value->str);
                  DataType type = parse_expression(stack, &current_token, &error, &file);
                  if (type == TYPE_UNKNOWN){
                    exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
                  }

                  if (type == TYPE_NIL) {
                    node_data.isNil = true;
                  }

                  if (!is_compatible(node_data.dtype, type)) {
                    exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
                  }
                  // if (type != node_data.dtype){
                  // }

                  // node_data.dtype = type;
                }

                // else {
                  // exitWithError("Syntax error: expected =\n", ERR_SYNTAX);
                // }

                else {
                  node_data.isNil = false;

                }
                
              }

              else if (current_token.token_type == T_ASSIGN){
                current_token = get_token(file); // get =
                current_token = get_token(file); // get exp
                int error = 0;
                //change to symstack
                // DataType type = parse_expression(stack, &current_token, &error, &file);

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

              insert_SymTable(global_symtable, id_name, node_data);
              current_token = get_token(file); // get next token
        }

        if (current_token.token_type == T_KEYWORD &&
            !strcmp(current_token.string_value->str, "func")) {
              // SymData node_data;
              // current_token = get_token(file); // get func


              current_token = get_token(file); // get id
              if (current_token.token_type != T_TYPE_ID){
                exitWithError("Syntax error: expected function name\n", ERR_SYNTAX);
              }

              char *id_name = current_token.string_value->str;

              if (search_SymTable(global_symtable, id_name) != NULL) {
                exitWithError("Semantic error: function already declared\n", ERR_SEMANT_FUNC_ARG); 
              }

              // node_data.name = current_token.string_value->str;
              // node_data.isGlobal = true;
              // node_data.isFunction = true;
              // node_data.returnType = TYPE_UNKNOWN;
              // node_data.paramTypes.next = NULL;
              // node_data.local_SymTable = NULL;
              // node_data.paramCount = 0;

              current_token = get_token(file); // get (

              ListFuncParam *params = NULL;
              int param_cnt = 0;
              DataType return_type = TYPE_UNKNOWN;
              // add parameters to function symbol table and save it somewhere
              PARAM_LIST_FIRST(file, &params, &param_cnt);

              current_token = get_token(file); // get )

              current_token = peekNextToken(file); // peek -> or {

              if (current_token.token_type == T_ARROW) {
                current_token = get_token(file); // get ->
                current_token = get_token(file); // get type

                return_type = get_type(current_token.string_value->str);
                if (return_type == TYPE_UNKNOWN){
                  exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
                }

              }

              else if (current_token.token_type == T_LBRACE){
                return_type = TYPE_VOID;
              }

              else {
                exitWithError("Syntax error: expected -> or {\n", ERR_SYNTAX);
              }

              current_token = get_token(file); // get {
              if (current_token.token_type != T_LBRACE){
                exitWithError("Syntax error: expected {\n", ERR_SYNTAX);
              }


              //skip function definition for now
              // if (current_token.token_type == T_LBRACE) {
              //   int count = 1;
              //   while (count != 0) {
              //     // current_token = get_token(file); // get {
              //     if (current_token.token_type == T_LBRACE) {
              //       count++;
              //     }
              //     else if (current_token.token_type == T_RBRACE) {
              //       count--;
              //     }

              //     if (current_token.token_type == T_EOF) {
              //       exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
              //     }

              //     current_token = get_token(file); // get next
              //   }
              // }

              // else {
              //   exitWithError("Syntax error: expected {\n", ERR_SYNTAX);
              // }
              // printf("WHILE END\n");

              // printf("token: %s\n", current_token.string_value->str);

              // insert_SymTable(global_symtable, id_name, node_data);
              insert_FunctionSymTable(global_symtable, id_name, return_type,
                                    params, param_cnt);

              // else if (current_token.token_type == T_LBRACE) {
              //   current_token = get_token(file); // get {
              //   SymTable *local_symtable = create_SymTable();
              //   local_symtable->name = "local";
              //   s_push(stack, *local_symtable);

              //   current_symtable = s_peek(stack);
              // print_SymTable(global_symtable);
            }

        //skip all non global scopes
        if (current_token.token_type == T_LBRACE) { // current token is {
          int count = 1;
          while (count != 0) {
            // SymTable *local_symtable = create_SymTable();
            // local_symtable->name = "local";
            // s_push(stack, *local_symtable);

            // current_symtable = s_peek(stack);
            // current_symtable = s_peek(stack);

            current_token = get_token(file); // get next token after {
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

          current_token = get_token(file); // get next token

        }
      
        if (current_token.token_type == T_RBRACE)  // current token is }
          current_token = get_token(file); // get next token

        // else {
          // exitWithError("Syntax error: expected let, var, func, { or }\n", ERR_SYNTAX);
        // }
        }

      else {
        current_token = get_token(file); // get next token
      }
    }
    // print_SymTable(global_symtable);
    // printTree(global_symtable);
} 

void PROGRAM(FILE *file){
    DECLARE_GLOBAL_FUNC(file);
}

void DECLARE_GLOBAL_FUNC(FILE *file){
  // symbol_table = create_SymTable();
  //init the tree, set main return type
  STMT_LIST(file);
}

void STMT_LIST(FILE *file){
  // current_token= peekNextToken(file);
  
  // if (current_token.token_type == T_EOF ||
  //  current_token.token_type == T_RBRACE){
    // return;
  // }
  
  current_token = peekNextToken(file);
  
  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_KEYWORD)) return;


  STMT(file);
  STMT_LIST(file);
}

void STMT(FILE *file){
  current_token = get_token(file); //if, while, return, let, var, id, func
  char *str = current_token.string_value->str;

  if (current_token.token_type == T_KEYWORD &&
      strcmp(str, "if") == 0){

          IF_EXP(file);

          current_token = get_token(file); // get {
          if (current_token.token_type != T_LBRACE){
            exitWithError("Syntax error: expected on if {\n", ERR_SYNTAX);
          }


          if (current_token.token_type != T_RBRACE) {
            
            SymTable *local_symtable = create_SymTable();
            local_symtable->name = "if";
            s_push(&stack, *local_symtable);            

            STMT_LIST(file);

            current_token = get_token(file); // get }

            s_pop(&stack);
            
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

                  SymTable *local_symtable = create_SymTable();
                  local_symtable->name = "else";
                  s_push(&stack, *local_symtable);     

                  STMT_LIST(file);

                  s_pop(&stack);

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

              SymTable *local_symtable = create_SymTable();
              local_symtable->name = "while";
              s_push(&stack, *local_symtable);

              STMT_LIST(file);

              s_pop(&stack);

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
            (strcmp(str, "func") == 0)) { // funcq

              current_token = get_token(file); // get id
              if (current_token.token_type != T_TYPE_ID){
                exitWithError("Syntax error: expected function name\n", ERR_SYNTAX);
              }

              char *symtable_name = current_token.string_value->str;

              current_token = get_token(file); // get (
              if (current_token.token_type != T_LPAR){
                exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
              }

              PARAM_LIST(file);

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

              SymTable *local_symtable = create_SymTable();
              local_symtable->name = symtable_name;
              s_push(&stack, *local_symtable);
              print_SymTable(local_symtable);

              // printf("token on stmt list: %s\n", current_token.string_value->str);

              STMT_LIST(file);

              s_pop(&stack);

              current_token = get_token(file); // get }
              // printf("tokenfuncccccc: %s\n", current_token.string_value->str);
              if (current_token.token_type != T_RBRACE){
                exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
              }

              print_SymTable(local_symtable);
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

  current_token = get_token(file); // get id
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

  DataType type;

  if (current_token.token_type == T_ARROW) {
    current_token = get_token(file); // get ->
    TYPE(file, &type);
  }

  else {
    return;
  }
}

void PARAM_LIST_FIRST(FILE *file, ListFuncParam **params, int *param_cnt){ //current token is (

  current_token = peekNextToken(file);

  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)) return;

  PARAM_FIRST(file, params);
  (*param_cnt)++;

  current_token = peekNextToken(file);
  if (current_token.token_type == T_COMMA){
    current_token = get_token(file); // get ,
    PARAM_LIST_FIRST(file, params, param_cnt);
  }

  else {
    // current_token = get_token(file); // get )
    return;
  }
}

void PARAM_FIRST(FILE *file, ListFuncParam **params){ //current token is (

  char* param_name;
  DataType param_type;
  ParamPrefix param_prefix;

  // PARAM_PREFIX(file);
  current_token = get_token(file); // get id or _

  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)){
    exitWithError("Syntax error: expected id or _\n", ERR_SYNTAX);
  }

  if (current_token.token_type == T_TYPE_ID) {
    // (*params)->prefix = PREFIX_DEFAULT;
    param_prefix = PREFIX_DEFAULT;
  }  
  else {
    // (*params)->prefix = PREFIX_UNDERSCORE;
    param_prefix = PREFIX_UNDERSCORE;
  }

  // PARAM_NAME(file);
  current_token = get_token(file); // get id
  if (current_token.token_type != T_TYPE_ID){
    exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
  }

  // (*params)->name = current_token.string_value->str;
  param_name = current_token.string_value->str;

  current_token = get_token(file); // get :
  if (current_token.token_type != T_COLON){
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  // TYPE(file);
  current_token = get_token(file); // get type
  // (*params)->dataType = parseType(current_token);
  // param_type = parseType(current_token);
  param_type = get_type(current_token.string_value->str);
  if (param_type == TYPE_UNKNOWN){
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }
  
  *params = addParamToList(*params, param_name, param_type, param_prefix);

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

  DataType type;

  TYPE(file, &type);
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
  // printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
  SymData node_data;
  node_data = initSymData();

  fseek(file, -strlen(current_token.string_value->str), SEEK_CUR);

  node_data.canbeChanged = (!strcmp(current_token.string_value->str, "var")) ? true : false;

  current_token = get_token(file); // get id
  node_data.name = current_token.string_value->str;

  SymTable *check_symtable = create_SymTable();
  Print_Sym_stack(&stack);
  check_symtable = s_peek(&stack);
  // printf("check_symtable: %s\n", check_symtable->name);
  print_SymTable(check_symtable);
  // printf("GLOBALLLLLLLLLLLLLLLLLLLL\n");
  print_SymTable(global_symtable);
  if (!(!strcmp(check_symtable->name, "global"))) {
    if (search_SymTable(check_symtable, current_token.string_value->str) != NULL) {

      exitWithError("Semantic error: variable already declared\n", ERR_SEMANT_FUNC_ARG); 
    }
    // else
      // insert_SymTable(check_symtable, current_token.string_value->str, initSymData());
  }
  else {

    current_token = get_token(file); // get : or =
    if (current_token.token_type == T_COLON){
      current_token = get_token(file); // get type
      current_token = get_token(file); // get =
      int error = 0;
      parse_expression(&stack, &current_token, &error, &file);
    }

    else if (current_token.token_type == T_ASSIGN){
      // printf("token: %s\n", current_token.string_value->str);
      current_token = get_token(file); // get exp
      int error = 0;
      parse_expression(&stack, &current_token, &error, &file);
    }

    return;
  }


  current_token = get_token(file); // get : or =

  DataType type;

  if (current_token.token_type == T_COLON){
    TYPE(file, &type);
    MB_ASSIGN_EXPR(file, type);
    node_data.dtype = type;
  }

  else if (current_token.token_type == T_ASSIGN){
    // EXP(file);
    current_token = get_token(file); // get exp
    int error = 0;
    type = parse_expression(&stack, &current_token, &error, &file);
    node_data.dtype = type;
  }


  else {
    exitWithError("Syntax error: expected : or =\n", ERR_SYNTAX);
  }

  // node_data.name = current_token.string_value->str;
  node_data.isGlobal = false;
  node_data.isFunction = false;
  // printf("token, var exp ended on is: %s\n", current_token.string_value->str);
  insert_SymTable(check_symtable, node_data.name, node_data);
  // printf("INSERTED\n");
  // insert_SymTable(check_symtable, current_token.string_value->str, initSymData());
  print_SymTable(check_symtable);

}

void MB_ASSIGN_EXPR(FILE *file, DataType type){ //current token is keyword
  current_token = peekNextToken(file); // check if next token is =

  if (current_token.token_type == T_ASSIGN){
    current_token = get_token(file); // get =
    // EXP(file);
    int error = 0;
    if (type != parse_expression(&stack, &current_token, &error, &file)){
      exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
    }
  }

  else {
    // printf("token, var exp ended on is: %s\n", current_token.string_value->str);
    return;
  }
}

void TYPE(FILE *file, DataType *type){ //current token is :
  current_token = get_token(file); // get type
  char *str = current_token.string_value->str;
  *type = get_type(str);
  if (*type == TYPE_UNKNOWN){
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }

  // if (current_token.token_type == T_KEYWORD &&
  //      ((strcmp(str, "String") == 0) ||
  //      (strcmp(str, "Int") == 0) ||
  //      (strcmp(str, "Double") == 0) ||
  //      (strcmp(str, "String?") == 0) ||
  //      (strcmp(str, "Double?") == 0) ||
  //      (strcmp(str, "Int?") == 0))){

  //       return;
  // }

  // else {
  //   exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  // }
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
  current_token = peekNextToken(file); // get ( or let
  char *str = current_token.string_value->str;


  if (current_token.token_type == T_LPAR) {
          // get_token(file); // get (
          EXP(file);
          // get_token(file); // get )
  }

  // YEBKA PIZDEC, POTOM SDELAYEM NORMALNO
  else if (current_token.token_type == T_KEYWORD &&
      strcmp(str, "let") == 0)        {
      current_token = get_token(file); // get let


      current_token = get_token(file); // get id
      if (current_token.token_type != T_TYPE_ID){
        exitWithError("Syntax error: expected id on let\n", ERR_SYNTAX);
      }

      // current_token = get_token(file); // get {
  }

  else {
    exitWithError("Syntax error: expected ( or let\n", ERR_SYNTAX);
  }
}

// ONLY IN IFS, WHILES
void EXP(FILE *file){ 
  // Token start_exp_token = get_token(file);
  current_token = get_token(file);

  int error = 0;
  // DataType type = parse_expression(symbol_table, &current_token, &error, &file);
  DataType type = parse_expression(&stack, &current_token, &error, &file);
  if (type != TYPE_BOOL) {
    exitWithError("Semantic error: cant use non bool expressions in conditions\n", ERR_SEMANT_TYPE);
  }
  // if (type == TYPE_UNKNOWN){
    // exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
  // }
}