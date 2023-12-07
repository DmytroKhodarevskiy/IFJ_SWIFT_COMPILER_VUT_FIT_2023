#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

SymTable *global_symtable;
SymStack stack;
bool WasReturn = false;
bool Was_if_let = false;
AVLNode *node_if_let = NULL;

instr_node *main_gen = NULL;
instr_node *declares = NULL;
instr_list_dynamic *instr_list = NULL;

Token current_token;
int PHASE = 1;


void insert_include_functions_sym_table() {

  // Int2Double
  ListFuncParam intParam;
  intParam.name = "term";
  intParam.prefix = PREFIX_UNDERSCORE;
  intParam.prefixName = NULL;
  intParam.dataType = TYPE_INT;
  intParam.next = NULL;
  insert_FunctionSymTable(global_symtable, "Int2Double", TYPE_DOUBLE, &intParam, 1);
  
   // Double2Int 
   ListFuncParam doubleParam;
   doubleParam.name = "term";
   doubleParam.prefix = PREFIX_UNDERSCORE;
   doubleParam.prefixName = NULL;
   doubleParam.dataType = TYPE_DOUBLE;
   doubleParam.next = NULL;
   insert_FunctionSymTable(global_symtable, "Double2Int", TYPE_INT, &doubleParam, 1);

   // length
   ListFuncParam stringParam;
   stringParam.name = "s";
   stringParam.prefix = PREFIX_UNDERSCORE;
   stringParam.prefixName = NULL;
   stringParam.dataType = TYPE_STRING;
   stringParam.next = NULL;
   insert_FunctionSymTable(global_symtable,"length", TYPE_INT, &stringParam, 1);


   // ord 
   ListFuncParam stringParamOrd;
   stringParamOrd.name = "c";
   stringParamOrd.prefix = PREFIX_UNDERSCORE;
   stringParamOrd.prefixName = NULL;
   stringParamOrd.dataType = TYPE_STRING;
   stringParamOrd.next = NULL;
   insert_FunctionSymTable(global_symtable, "ord", TYPE_INT , &stringParamOrd, 1);

   // chr
   ListFuncParam intParamChr;
   intParamChr.name = "i";
   intParamChr.prefix = PREFIX_UNDERSCORE;
   intParamChr.prefixName = NULL;
   intParamChr.dataType = TYPE_INT;
   intParamChr.next = NULL;
   insert_FunctionSymTable(global_symtable, "chr", TYPE_STRING, &intParamChr, 1);

   // ReadInt 
    ListFuncParam intParamReadInt;
    intParamReadInt.name = "Int";
    intParamReadInt.prefix = PREFIX_UNDERSCORE;
    intParamReadInt.prefixName = NULL;
    intParamReadInt.dataType = TYPE_INT;
    intParamReadInt.next = NULL;
    insert_FunctionSymTable(global_symtable, "readInt", TYPE_INT_NULLABLE, &intParamReadInt, 0);

  // ReadDouble
    ListFuncParam doubleParamReadDouble;
    doubleParamReadDouble.name = "Double";
    doubleParamReadDouble.prefix = PREFIX_UNDERSCORE;
    doubleParamReadDouble.prefixName = NULL;
    doubleParamReadDouble.dataType = TYPE_DOUBLE;
    doubleParamReadDouble.next = NULL;
    insert_FunctionSymTable(global_symtable, "readDouble", TYPE_DOUBLE_NULLABLE, &doubleParamReadDouble, 0);


  // ReadString
    ListFuncParam stringParamReadString;
    stringParamReadString.name = "String";
    stringParamReadString.prefix = PREFIX_UNDERSCORE;
    stringParamReadString.prefixName = NULL;
    stringParamReadString.dataType = TYPE_STRING;
    stringParamReadString.next = NULL;
    insert_FunctionSymTable(global_symtable, "readString", TYPE_STRING_NULLABLE, &stringParamReadString, 0);

  //substr addParamToList
  ListFuncParam* paramaterListSubStr = NULL;
  paramaterListSubStr = addParamToList(paramaterListSubStr, "s", TYPE_STRING, PREFIX_UNDERSCORE, NULL);

  paramaterListSubStr = addParamToList(paramaterListSubStr, "i", TYPE_INT, PREFIX_UNDERSCORE, NULL);

  paramaterListSubStr = addParamToList(paramaterListSubStr, "j", TYPE_INT , PREFIX_UNDERSCORE, NULL);

  insert_FunctionSymTable(global_symtable, "substr", TYPE_STRING, paramaterListSubStr, 3);


}


void Parse(FILE *file){

    PHASE_FIRST(file);
    PHASE = 2;
    linenum = 0;
    rewind(file);
    fprintf(stderr, "FIRST PHASE DONE\n");

    add_instr(&main_gen, "\n");

    add_instr(&declares, "LABEL **main_declares**\n");

    main_gen->name_of_llist = "global";
    instr_list = init_instr_list_dynamic();
    Data data;
    generate_code(&main_gen, data, GEN_MAIN, 0, UNUSED);

    generate_code(&main_gen, data, GEN_IF_START, 0, UNUSED);

    PHASE_SECOND(file);


    add_instr(&declares, "JUMP **main_declares_return**\n");

    generate_header();

    instr_node *builtin = NULL;
    generate_code(&builtin, data, GEN_BUILTIN, 0, UNUSED);
    pop_list_to_file(&builtin);

    generate_code(&main_gen, data, GEN_EXIT, 0, UNUSED);

    pop_all_lists_to_file(instr_list);

    pop_list_to_file(&main_gen);

    pop_list_to_file(&declares);
}

void PHASE_SECOND(FILE *file) {
  PROGRAM(file);
}

// Read Global Symbol Table and Functions Symbol Tables
void PHASE_FIRST(FILE *file){

    s_initializeStack(&stack); 

    global_symtable = create_SymTable();
    global_symtable->name = "global";

    s_push(&stack, global_symtable);

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

    SymTable current_symtable = *(stack->items[stack->top]);
    insert_include_functions_sym_table();

    while (current_token.token_type != T_EOF) {

      if ((current_token.token_type == T_KEYWORD && (!strcmp(current_token.string_value->str, "func") ||
          !strcmp(current_token.string_value->str, "return"))) || 
                                                     current_token.token_type == T_LBRACE ||
                                                      current_token.token_type == T_RBRACE) {

        if (current_token.token_type == T_KEYWORD &&
            !strcmp(current_token.string_value->str, "return")) {
              //check
              exitWithError("Syntax error: return outside function\n", ERR_SYNTAX);
            }


        if (current_token.token_type == T_KEYWORD &&
            !strcmp(current_token.string_value->str, "func")) {

              current_token = get_token(file); // get id

              if (current_token.token_type != T_TYPE_ID){
              //check
                exitWithError("Syntax error: expected function name\n", ERR_SYNTAX);
              }

              char *id_name = current_token.string_value->str;

              
              if (search_SymTable(global_symtable, id_name) != NULL) {
              //check
                exitWithError("Semantic error: function already declared\n", ERR_SEMANT_FUNC_ARG); 
              }

              current_token = get_token(file); // get (
              Token check_token = peekNextToken(file); // peek type or )
              DataType return_type = TYPE_VOID;
              ListFuncParam *params = NULL;

                int param_cnt = 0;
              if (check_token.token_type != T_RPAR) {
                PARAM_LIST_FIRST(file, &params, &param_cnt);
              }



              current_token = get_token(file); // get )

              current_token = peekNextToken(file); // peek -> or {

              if (current_token.token_type == T_ARROW) {
                current_token = get_token(file); // get ->
                current_token = get_token(file); // get type

                return_type = get_type(current_token.string_value->str);
                if (return_type == TYPE_UNKNOWN){
              //check
                  exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
                }

              }

              else if (current_token.token_type == T_LBRACE){
                return_type = TYPE_VOID;
              }

              else {
              //check
                exitWithError("Syntax error: expected -> or {\n", ERR_SYNTAX);
              }


              current_token = get_token(file); // get {
              if (current_token.token_type != T_LBRACE){
              //check
                exitWithError("Syntax error: expected {\n", ERR_SYNTAX);
              }
              insert_FunctionSymTable(global_symtable, id_name, return_type,
                                    params, param_cnt);
            }

        //skip all non global scopes
        if (current_token.token_type == T_LBRACE) { // current token is {
          int count = 1;
          while (count != 0) {

            current_token = get_token(file); // get next token after {
            if (current_token.token_type == T_LBRACE) {
              count++;
            }
            else if (current_token.token_type == T_RBRACE) {
              count--;
            }

            if (current_token.token_type == T_EOF) {
              //check
              exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
            }

          }

          current_token = get_token(file); // get next token

        }
      
        if (current_token.token_type == T_RBRACE) { // current token is } 
          current_token = get_token(file); // get next token
        }


        }

      else {
        current_token = get_token(file); // get next token
      }


    }
} 

void PROGRAM(FILE *file){
    DECLARE_GLOBAL_FUNC(file);
}

void DECLARE_GLOBAL_FUNC(FILE *file){
  STMT_LIST(file);
}

void STMT_LIST(FILE *file){
  
  current_token = peekNextToken(file);
  
  if (current_token.token_type == T_EOF || current_token.token_type == T_RBRACE) return;

  // fprintf(stderr, "LINENUM: %d\n", linenum);

  STMT(file);
  STMT_LIST(file);
}

void STMT(FILE *file){
  current_token = get_token(file); //if, while, return, let, var, id, func
  char *str = current_token.string_value->str;

  if (current_token.token_type == T_KEYWORD &&
      strcmp(str, "if") == 0){

          IF_EXP(file);

          SymData *Name = s_getFirstFunctionSymData(&stack);
          int deep = Get_deepness_current(&stack);

          static int ifelse_cnt = 0;
          Data data;
          data.ifelse_cnt = ifelse_cnt;

          if(Name == NULL && stack.top == 0) {
            generate_code(&main_gen, data, GEN_IF_CHECK, 0, GF);
          }

          else {

            if (Name == NULL && stack.top != 0) {
              generate_code(&main_gen, data, GEN_IF_CHECK, deep, LF);
            }
            else {
              instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
              generate_code(&node, data, GEN_IF_CHECK, deep, UNUSED);
            }
          }

          ifelse_cnt++;

          current_token = get_token(file); // get {
          if (current_token.token_type != T_LBRACE){
              //check
            exitWithError("Syntax error: expected on if {\n", ERR_SYNTAX);
          }

          if (current_token.token_type != T_RBRACE) {
            


            SymTable *local_symtable = create_SymTable();
            local_symtable->name = "if";

            s_push(&stack, local_symtable);

            STMT_LIST(file);

            current_token = get_token(file); // get }

            s_pop(&stack);
            
            if (current_token.token_type != T_RBRACE){
              //check
              exitWithError("Syntax error: expected on if }\n", ERR_SYNTAX);
            }
          }

          else {
            current_token = get_token(file); // get }
            if (current_token.token_type != T_RBRACE){
              //check
              exitWithError("Syntax error: expected on if }\n", ERR_SYNTAX);
            }
          }

          if (Name == NULL) {
            generate_code(&main_gen, data, GEN_IF_END, deep, UNUSED);
            generate_code(&main_gen, data, GEN_ELSE_START, deep, UNUSED);
          }
          else {
            instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
            generate_code(&node, data, GEN_IF_END, deep, UNUSED);
            generate_code(&node, data, GEN_ELSE_START, deep, UNUSED);
          }


          current_token = get_token(file); // get else


          if (current_token.token_type == T_KEYWORD &&
              strcmp(current_token.string_value->str, "else") == 0){

                current_token = get_token(file); // get {
                if (current_token.token_type != T_LBRACE){
              //check
                  exitWithError("Syntax error: expected on else {\n", ERR_SYNTAX);
                }

                current_token = peekNextToken(file);
                if (current_token.token_type != T_RBRACE){

                  SymTable *local_symtable = create_SymTable();
                  local_symtable->name = "else";
                  s_push(&stack, local_symtable);     
                  STMT_LIST(file);

                  s_pop(&stack);

                  if (current_token.token_type != T_RBRACE){
              //check
                    exitWithError("Syntax error: expected on else }\n", ERR_SYNTAX);
                  }
                  current_token = get_token(file); // get }
                }
                else {
                  current_token = get_token(file); // get }
                }

          }
          
          else {
              //check
            exitWithError("Syntax error: expected else\n", ERR_SYNTAX);
          }

          if (Name == NULL) {
            generate_code(&main_gen, data, GEN_ELSE_IF_END, deep, UNUSED);
          }
          else {
            instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
            generate_code(&node, data, GEN_ELSE_IF_END, deep, UNUSED);
          }

          if (Was_if_let) {
            Was_if_let = false;
            DataType type = node_if_let->data.dtype;
            switch (type) {
              case TYPE_INT:
                node_if_let->data.dtype = TYPE_INT_NULLABLE;
                break;
              case TYPE_DOUBLE:
                node_if_let->data.dtype = TYPE_DOUBLE_NULLABLE;
                break;
              case TYPE_STRING:
                node_if_let->data.dtype = TYPE_STRING_NULLABLE;
                break;
              default:
                break;
            }
          }

      }

  else if (current_token.token_type == T_KEYWORD &&
           ((strcmp(str, "let") == 0) ||
            (strcmp(str, "var") == 0))) {  // let or var

          bool changeable = (strcmp(str, "var") == 0) ? true : false;

          current_token = get_token(file); // get id
          if (current_token.token_type != T_TYPE_ID){
              //check
            exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
          }


          MB_STMT_LET_VAR(file, changeable);
      }

  else if (current_token.token_type == T_KEYWORD &&
           (strcmp(str, "return") == 0)) { //// return
            WasReturn = true;
            SymData *func_name_sym = s_getFirstFunctionSymData(&stack);
            char* func_name = func_name_sym->name;
            AVLNode *node = search_SymTable(global_symtable, func_name);
            if (node->data.isDefined == false){
              //check
              exitWithError("Semantic error: function is not defined\n", ERR_SEMANT_FUNC_ARG);
            }

            if (node->data.returnType == TYPE_VOID){
              //check
              exitWithError("Semantic error: return in void function\n", ERR_SEMANT_RETURN);
            }

            current_token = peekNextToken(file); // get EXP or }
            if (current_token.token_type != T_TYPE_ID && current_token.token_type != T_LPAR &&
                current_token.token_type != T_INT && current_token.token_type != T_DOUBLE &&
                current_token.token_type != T_SING_STRING &&  current_token.token_type != T_MUL_STRING &&
                current_token.token_type != T_EXPONENT_FLOAT && !(current_token.token_type == T_KEYWORD && !strcmp(current_token.string_value->str, "nil"))){
              exitWithError("Semantic error: return doesn't have an expression\n", ERR_SEMANT_RETURN);
            }

            else {
              int error = 0;
              current_token = get_token(file); // get exp
              DataType type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);

              if (type == TYPE_UNKNOWN){
                //check?
                exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
              }


                //check
              if (node->data.returnType != type && (type == TYPE_NIL && 
                                                  node->data.returnType != TYPE_INT_NULLABLE &&
                                                  node->data.returnType != TYPE_DOUBLE_NULLABLE && 
                                                  node->data.returnType != TYPE_STRING_NULLABLE)) 
                exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
            }
      }

  else if (current_token.token_type == T_KEYWORD &&
           (strcmp(str, "while") == 0)) { // while

            SymData *Name = s_getFirstFunctionSymData(&stack);
            int deep = Get_deepness_current(&stack);

            static int while_cnt = 0;
            Data data;
            data.while_cnt = while_cnt;
           
           if(Name == NULL && stack.top == 0) {
              generate_code(&main_gen, data, GEN_WHILE_START, 0, GF);
              }

            else {

              if (Name == NULL && stack.top != 0) {
                generate_code(&main_gen, data, GEN_WHILE_START, deep, LF);
              }
              else {
                instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
                generate_code(&node, data, GEN_WHILE_START, deep, UNUSED);
              }
            }
           
           WHILE_EXP(file);


            if(Name == NULL && stack.top == 0) {
              generate_code(&main_gen, data, GEN_WHILE_CHECK, 0, GF);
              }

            else {

              if (Name == NULL && stack.top != 0) {
                generate_code(&main_gen, data, GEN_WHILE_CHECK, deep, LF);
              }
              else {
                instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
                generate_code(&node, data, GEN_WHILE_CHECK, deep, UNUSED);
              }
            }

           current_token = get_token(file); // get {

           if (current_token.token_type != T_LBRACE){
                //check
             exitWithError("Syntax error: expected on while {\n", ERR_SYNTAX);
           }

           current_token = peekNextToken(file);
           if (current_token.token_type != T_RBRACE){

              SymTable *local_symtable = create_SymTable();
              local_symtable->name = "while";
              s_push(&stack, local_symtable);

              STMT_LIST(file);

              s_pop(&stack);

              if (current_token.token_type != T_RBRACE){
                //check
                exitWithError("Syntax error: expected on while }\n", ERR_SYNTAX);
              }
              current_token = get_token(file); // get }
           }
           else {
             current_token = get_token(file); // get }
           }

           if (Name == NULL) {
              generate_code(&main_gen, data, GEN_WHILE_END, deep, UNUSED);
            }
            else {
              instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
              generate_code(&node, data, GEN_WHILE_END, deep, UNUSED);
            }

            while_cnt++;
      }  

  else if (current_token.token_type == T_KEYWORD &&
            (strcmp(str, "func") == 0)) { // funcq

              current_token = get_token(file); // get id

              char *func_name = current_token.string_value->str;
              if (current_token.token_type != T_TYPE_ID){
                //check
                exitWithError("Syntax error: expected function name\n", ERR_SYNTAX);
              }

              add_new_linked_list(instr_list, func_name);

              // print_list_names(instr_list);

              instr_node *node = search_by_name_in_list(instr_list, func_name, main_gen);
              Data data;
              data.func_name = func_name;
              generate_code(&node, data, GEN_FUNC_START, 0, UNUSED);

              SymTable *local_symtable = create_SymTable();

              strncpy(local_symtable->name, current_token.string_value->str, 255);
              local_symtable->name[255] = '\0';  // Ensure null-termination
              s_push(&stack, local_symtable);

              current_token = get_token(file); // get (
              if (current_token.token_type != T_LPAR){
                //check
                exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
              }

              Token check_token = peekNextToken(file); // peek type or )
              
              if (check_token.token_type != T_RPAR) {
                PARAM_LIST(file);
              }
              else
                current_token = get_token(file); // get )

              if (current_token.token_type != T_RPAR){
                //check
                exitWithError("Syntax error: expected )\n", ERR_SYNTAX);
              }

              RETURN_TYPE(file);

              current_token = get_token(file); // get {
              if (current_token.token_type != T_LBRACE){
                //check
                exitWithError("Syntax error: expected {\n", ERR_SYNTAX);
              }

              STMT_LIST(file);

              DataType rettype = search_SymTable(global_symtable, func_name)->data.returnType;
              if (!WasReturn && rettype != TYPE_VOID) {
                //check
                exitWithError("Semantic error: function doesn't have return\n", ERR_SEMANT_RETURN);
              }

              s_pop(&stack);
              WasReturn = false;

              generate_code(&node, data, GEN_FUNC_END, 0, UNUSED);
              char *func_name_dec = malloc(MAX_LINE_LENGTH);
              if (func_name_dec == NULL)
                exitWithError("Error: malloc failed\n", ERR_INTERNAL);
              snprintf(func_name_dec, MAX_LINE_LENGTH, "JUMP **%s_declares_return**\n", func_name);
              add_instr(&node->declarations, func_name_dec);

              current_token = get_token(file); // get }
              if (current_token.token_type != T_RBRACE){
                //check
                exitWithError("Syntax error: expected }\n", ERR_SYNTAX);
              }

            }

  else if (current_token.token_type == T_TYPE_ID) { // id
            ASSIGN_STMT_OR_FUNCALL(file);        
        
          }

  else {
                //check
    exitWithError("Syntax error: expected if, while, return, let, var, id, func\n", ERR_SYNTAX);
  }

}

void ASSIGN_STMT_OR_FUNCALL(FILE *file){ //current token is id

  char *id_name = current_token.string_value->str;
  Token check_token = peekNextToken(file); // peek = or (

  if (check_token.token_type == T_ASSIGN){

    AVLNode *node_data = s_search_symtack(&stack, id_name);

    if (node_data == NULL) {
      //check
      exitWithError("Semantic error: variable not declared\n", ERR_SEMANT_FUNC_ARG); 
    }
    if (node_data->data.isDefined == false){
      //check
      exitWithError("Semantic error: variable is not defined\n", ERR_SEMANT_FUNC_ARG);
    }


    if (!node_data->data.canbeChanged) {
      //check
      exitWithError("Semantic error: variable is not assignable\n", ERR_SEMANT_FUNC_ARG); 
    }


    DataType type_to_assign = node_data->data.dtype;

    current_token = get_token(file); // get =
    int error = 0;
    current_token = get_token(file); // get exp

    SymTable *check_symtable = create_SymTable();
    check_symtable = s_peek(&stack);

    SymData *node_func;
    Data data;
    instr_node *node;
    Frame frame;
    int deep;
    data.op.id_name = id_name;

    if (stack.top != 0) {
      node_func = s_getFirstFunctionSymData(&stack);
      if (node_func != NULL) {
        node = search_by_name_in_list(instr_list, node_func->name, main_gen);
      }
      else {
        node = main_gen;
      }
      deep = Get_deepness_of_var(&stack, id_name);
      if (deep == 0)
        frame = GF;
      else
        frame = LF;
    }
    else {
      deep = 0;
      frame = GF;
      node = main_gen;
    }

    DataType type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
    if (type != TYPE_NIL) {
      node_data->data.isNil = false;
    }

    generate_code(&node, data, GEN_ASSIGN, deep, frame);

    if (!is_compatible(type_to_assign, type)) {
      //check
      exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
    }

    if (type == TYPE_UNKNOWN){
      //check?
      exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
    }
  }

  else if (check_token.token_type == T_LPAR) {
    
    if (!strcmp(id_name, "write")) {
      
      current_token = get_token(file); // get (
      WRITE_CALLS(file);
      return;
    }

    int error = 0;
    parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
    return;
  } 

  else {
                //check
    current_token = get_token(file); // get exp
                int error = 0;
    parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
  }
}

void WRITE_CALLS(FILE *file){ //current token is (



  if (current_token.token_type != T_LPAR){
                //check
    exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
  }

  ARG_WRITE_LIST(file);

  current_token = get_token(file); // get )
  if (current_token.token_type != T_RPAR){
                //check
    exitWithError("Syntax error: expected ) or ,\n", ERR_SYNTAX);
  }
}

void FUNC_CALLS(FILE *file){ //current token is (
  if (current_token.token_type != T_LPAR){
                //check
    exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
  }

  ARG_LIST(file);

  current_token = get_token(file); // get )
  if (current_token.token_type != T_RPAR){
                //check
    exitWithError("Syntax error: expected ) or ,\n", ERR_SYNTAX);
  }
}

void ARG_WRITE_LIST(FILE *file){ //current token is (
   current_token = peekNextToken(file);

  if (!(current_token.token_type == T_INT ||
      current_token.token_type == T_DOUBLE ||
      current_token.token_type == T_SING_STRING ||
      current_token.token_type == T_MUL_STRING ||
      current_token.token_type == T_EXPONENT_INT ||
      current_token.token_type == T_EXPONENT_FLOAT ||
      current_token.token_type == T_TYPE_ID)) exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  
  ARG_WRITE(file);

  current_token = peekNextToken(file);
  if (current_token.token_type == T_COMMA){
    current_token = get_token(file); // get ,
    ARG_WRITE_LIST(file);
  }

  else {
    return;
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
    return;
  }
}

void ARG_WRITE(FILE *file){ //current token is (

  current_token = get_token(file); // get param
  DataType actual_argument_type = TYPE_UNKNOWN;
  // char *Name = s_getFirstFunctionSymData(&stack)->name;
  SymData *Name = s_getFirstFunctionSymData(&stack);
  instr_node *node_inst;
  if (Name != NULL) 
    node_inst = search_by_name_in_list(instr_list, Name->name, main_gen);
  else
    node_inst = main_gen;
  Data data = init_data();
  if(current_token.token_type == T_TYPE_ID){
    AVLNode *node = s_search_symtack(&stack, current_token.string_value->str);
    if(peekNextToken(file).token_type == T_COLON){
      exitWithError("Semantic error: Variable has no prefix\n", ERR_SEMANT_TYPE);
    }
    if(node == NULL) {
      exitWithError("Semantic error: undefined variable\n", ERR_SEMANT_UNDF_VALUE);
    }
    if(node->data.isFunction) {
      exitWithError("Semantic error: Cannot use function as argument\n", ERR_SEMANT_TYPE);
    }
    data.op.id_name = current_token.string_value->str;
    int depth = Get_deepness_of_var(&stack, current_token.string_value->str);
    if (depth == 0)
      generate_code(&node_inst, data, GEN_WRITE, depth, GF);
    else
      generate_code(&node_inst, data, GEN_WRITE, depth, LF);
  }
  else{
    data.op.val = current_token.string_value->str;
    data.op.type = convert_tokenType_to_symType(current_token.token_type);
    generate_code(&node_inst, data, GEN_WRITE, 0, UNUSED);
  }

  if (s_search_symtack(&stack, current_token.string_value->str) == NULL &&
       current_token.token_type == T_TYPE_ID) {
    //check
    exitWithError("Semantic error: variable not declared\n", ERR_SEMANT_FUNC_ARG); 
  }

  if (current_token.token_type != T_INT &&
      current_token.token_type != T_DOUBLE &&
      current_token.token_type != T_SING_STRING &&
      current_token.token_type != T_MUL_STRING &&
      current_token.token_type != T_EXPONENT_INT &&
      current_token.token_type != T_EXPONENT_FLOAT &&
      current_token.token_type != T_TYPE_ID){
                //check
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }


}

void ARG(FILE *file){ //current token is (

  PREFIX(file);

  current_token = get_token(file); // get :
  if (current_token.token_type != T_COLON){
                //check
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  current_token = get_token(file); // get id

  if (!(current_token.token_type == T_TYPE_ID || 
      current_token.token_type == T_INT ||
      current_token.token_type == T_DOUBLE ||
      current_token.token_type == T_SING_STRING ||
      current_token.token_type == T_MUL_STRING ||
      current_token.token_type == T_EXPONENT_INT ||
      current_token.token_type == T_EXPONENT_FLOAT ||
      (current_token.token_type == T_KEYWORD && !strcmp(current_token.string_value->str, "nil")))){

                //check
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
    return;
  }
}

void PARAM_FIRST(FILE *file, ListFuncParam **params){ //current token is (

  char* param_name;
  char *prefix = "_";
  DataType param_type;
  ParamPrefix param_prefix;

  current_token = get_token(file); // get id or _

  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)){
                //check
    exitWithError("Syntax error: expected id or _\n", ERR_SYNTAX);
  }

  if (current_token.token_type == T_TYPE_ID) {
   prefix = current_token.string_value->str;
   param_prefix = PREFIX_DEFAULT;
  }  
  else {
    param_prefix = PREFIX_UNDERSCORE;
    prefix = current_token.string_value->str;
  }

  current_token = get_token(file); // get id
  if (current_token.token_type != T_TYPE_ID){
                //check
    exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
  }

  param_name = current_token.string_value->str;

  current_token = get_token(file); // get :
  if (current_token.token_type != T_COLON){
                //check
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  current_token = get_token(file); // get type
  param_type = get_type(current_token.string_value->str);
  if (param_type == TYPE_UNKNOWN){
                //check
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }
  
  *params = addParamToList(*params, param_name, param_type, param_prefix, prefix);

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
                //check
    exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
  }

  DataType type;

  TYPE(file, &type);
}

void PARAM_NAME(FILE *file) { //current token is id (prefix)

  current_token = get_token(file); // get id
  if (current_token.token_type != T_TYPE_ID){
                //check
    exitWithError("Syntax error: expected id\n", ERR_SYNTAX);
  }
}

void PARAM_PREFIX(FILE *file){ //current token is (

  current_token = get_token(file); // get id
  if (!(current_token.token_type == T_TYPE_ID ||
      current_token.token_type == T_UNDERSCORE_ID)){
                //check
    exitWithError("Syntax error: expected id or _\n", ERR_SYNTAX);
  }

}

void MB_STMT_LET_VAR(FILE *file, bool changeable){ //current token is id
  SymData node_data;
  node_data = initSymData();

  node_data.canbeChanged = changeable;

  fprintf(stderr, "CURRENT TOKEN: %s\n", current_token.string_value->str);

  node_data.name = current_token.string_value->str;

  SymTable *check_symtable = create_SymTable();
  check_symtable = s_peek(&stack);

  Data data;

  SymData *Name = s_getFirstFunctionSymData(&stack);

  if (Name == NULL && stack.top == 0)  {
    data.op.id_name = node_data.name;
    generate_code(&main_gen, data, GEN_CREATE_ID, 0, GF);
  }
  else {

    if (Name == NULL && stack.top != 0) {
      int deepness = Get_deepness_current(&stack);
      data.op.id_name = node_data.name;
      bool is_declared;
      char *id_name_dec;
      int length = snprintf(NULL, 0, "%s_%d", node_data.name, deepness) + 1; // +1 for null terminator
      id_name_dec = malloc(length); // Allocate memory

      if (id_name_dec != NULL) {
        sprintf(id_name_dec, "%s_%d", node_data.name, deepness);
        is_declared = is_substr_in_list(declares, id_name_dec);

        free(id_name_dec); // Don't forget to free the allocated memory
      } else {
        // Handle memory allocation error
        exitWithError("Memory allocation error\n", ERR_INTERNAL);
      }

      if (!is_declared) 
        generate_code(&declares, data, GEN_CREATE_ID, deepness, LF);
    }
    else {
    data.op.id_name = node_data.name;
    instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);

    Print_Sym_stack(&stack);
    int deep = Get_deepness_current(&stack);

    generate_code(&node->declarations, data, GEN_CREATE_ID, deep, LF);
    }
  }

  if (search_SymTable(check_symtable, current_token.string_value->str) != NULL) {
                //check
      exitWithError("Semantic error: variable already declared\n", ERR_SEMANT_FUNC_ARG); 
  }
 
  current_token = get_token(file); // get : or =

  DataType type;

  if (current_token.token_type == T_COLON){
    TYPE(file, &type);
    if (type == TYPE_DOUBLE_NULLABLE || type == TYPE_INT_NULLABLE || type == TYPE_STRING_NULLABLE){
      //check
      // node_data.isNil = true;
      node_data.isNil = false;
    }
    MB_ASSIGN_EXPR(file, type, &node_data);
    node_data.dtype = type;
  }
  else if (current_token.token_type == T_ASSIGN){
    current_token = get_token(file); // get exp
    int error = 0;

    SymData *node_func;
    Data data;
    instr_node *node;
    Frame frame;
    int deep;
    data.op.id_name = node_data.name;

    if (stack.top != 0) {
      node_func = s_getFirstFunctionSymData(&stack);
      if (node_func != NULL) {
        node = search_by_name_in_list(instr_list, node_func->name, main_gen);
      }
      else {
        node = main_gen;
      }
      deep = Get_deepness_current(&stack);
      frame = LF;
    }
    else {
      deep = 0;
      frame = GF;
      node = main_gen;
    }

    if (main_gen == NULL)
      fprintf(stderr, "STACK IS NULL\n");
    if (instr_list == NULL)
      fprintf(stderr, "INSTR LIST IS NULL\n");

    fprintf(stderr, "linenum on exp: %d\n", linenum);

    type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
    if (type == TYPE_UNKNOWN){
      exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
    }
    node_data.dtype = type;
    fprintf(stderr, "linenum after exp: %d\n", linenum);

    generate_code(&node, data, GEN_ASSIGN, deep, frame);
  }


  else {
                //check
    exitWithError("Syntax error: expected : or =\n", ERR_SYNTAX);
  }

  if (!strcmp(check_symtable->name, "global")) {
    node_data.isGlobal = true;
  }

  else {
    node_data.isGlobal = false;
  }
  node_data.isFunction = false;
  node_data.isDefined = true;
  insert_SymTable(check_symtable, node_data.name, node_data);

}

void MB_ASSIGN_EXPR(FILE *file, DataType type, SymData *node_data){ //current token is keyword
  current_token = peekNextToken(file); // check if next token is =


  if (current_token.token_type == T_ASSIGN){
    current_token = get_token(file); // get =
    current_token = get_token(file); // get exp

    SymTable *check_symtable = create_SymTable();
    check_symtable = s_peek(&stack);

    SymData *node_func;
    Data data;
    instr_node *node;
    Frame frame;
    int deep;
    data.op.id_name = node_data->name;

    if (stack.top != 0) {
      node_func = s_getFirstFunctionSymData(&stack);
      if (node_func != NULL) {
        node = search_by_name_in_list(instr_list, node_func->name, main_gen);
      }
      else {
        node = main_gen;
      }
      deep = Get_deepness_current(&stack);
      frame = LF;
    }
    else {
      deep = 0;
      frame = GF;
      node = main_gen;
    }

  int error = 0;

  fprintf(stderr, "currenttoken: %s\n", current_token.string_value->str);

  DataType exp_type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
  if (type == TYPE_UNKNOWN){
    exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
  }

  fprintf(stderr, "currenttoken: %s\n", current_token.string_value->str);
  is_compatible(type, exp_type) ? 0 : exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
  
  generate_code(&node, data, GEN_ASSIGN, deep, frame);
  
  if (exp_type == TYPE_NIL) {
    fprintf(stderr, "IS NIL\n");
    node_data->isNil = true;
  }

  }

  else {
    return;
  }
}

void TYPE(FILE *file, DataType *type){ //current token is :
  current_token = get_token(file); // get type
  char *str = current_token.string_value->str;
  *type = get_type(str);
  if (*type == TYPE_UNKNOWN){
                //check
    exitWithError("Syntax error: expected correct type\n", ERR_SYNTAX);
  }

}

void WHILE_EXP(FILE *file){ //current token is while
  current_token = peekNextToken(file); // get (

  if (current_token.token_type == T_LPAR){
          // get_token(file); // get (
          EXP(file);
          // get_token(file); // get )
  }

  else {
                //check
    exitWithError("Syntax error: expected (\n", ERR_SYNTAX);
  }
}

void IF_EXP(FILE *file) { //current token is if
  current_token = peekNextToken(file); // get ( or let

  if (current_token.token_type == T_LPAR) {
          EXP(file);
  }

  // IF LET TODO
  else if (current_token.token_type == T_KEYWORD &&
      strcmp(current_token.string_value->str, "let") == 0)        {
      current_token = get_token(file); // get let

      Was_if_let = true;

      current_token = get_token(file); // get id
      if (current_token.token_type != T_TYPE_ID){
                //check
        exitWithError("Syntax error: expected id on let\n", ERR_SYNTAX);
      }

      bool is_nil = false;
      AVLNode *var = s_search_symtack(&stack, current_token.string_value->str);

      fprintf(stderr, "VAR: %s\n", var->data.name);
      fprintf(stderr, "VAR IS FUNCTION: %d\n", var->data.isFunction);
      fprintf(stderr, "VAR IS NIL: %d\n", var->data.isNil);
      fprintf(stderr, "VAR IS DEFINED: %d\n", var->data.isDefined);
      fprintf(stderr, "VAR CAN BE CHANGED: %d\n", var->data.canbeChanged);
      fprintf(stderr, "VAR IS GLOBAL: %d\n", var->data.isGlobal);


      node_if_let = var;
      if (var == NULL) {
        //check
        exitWithError("Semantic error: variable not declared\n", ERR_SEMANT_UNDF_VALUE); 
      }

      else {
        if (var->data.isFunction) {
          //check
          exitWithError("Semantic error: variable is function\n", ERR_SEMANT_TYPE); 
        }

        if (var->data.isNil) {
          is_nil = true;
        }
        else {

          DataType type = var->data.dtype;
          fprintf(stderr, "TYPE BEFORE: %d\n", type);
          switch (type) {
            case TYPE_INT_NULLABLE:
              var->data.dtype = TYPE_INT;
              break;
            case TYPE_DOUBLE_NULLABLE:
              var->data.dtype = TYPE_DOUBLE;
              break;
            case TYPE_STRING_NULLABLE:
              var->data.dtype = TYPE_STRING;
              break;
            default:
              break;
          }
          fprintf(stderr, "TYPE AFTER: %d\n", var->data.dtype);
        }
      }

      SymData *Name = s_getFirstFunctionSymData(&stack);
      Data data;
      if (Name == NULL && stack.top == 0) {
        data.op.id_name = current_token.string_value->str;
        generate_code(&main_gen, data, GEN_IF_LET, 0, GF);
        //check
      }

      else {
        if (Name == NULL && stack.top != 0) {
          generate_code(&main_gen, data, GEN_IF_LET, Get_deepness_current(&stack), LF);
        }
        else {
          instr_node *node = search_by_name_in_list(instr_list, Name->name, main_gen);
          generate_code(&node, data, GEN_IF_LET, Get_deepness_current(&stack), LF);
        }
      }


  }

  else {
                //check
    exitWithError("Syntax error: expected ( or let\n", ERR_SYNTAX);
  }
}

// ONLY IN IFS, WHILES
void EXP(FILE *file){ 
  current_token = get_token(file);

  int error = 0;
  DataType type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
  if (type != TYPE_BOOL) {
    exitWithError("Semantic error: cant use non bool expressions in conditions\n", ERR_SEMANT_TYPE);
  }

}
