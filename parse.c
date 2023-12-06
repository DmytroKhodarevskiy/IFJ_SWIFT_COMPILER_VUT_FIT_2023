#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

SymTable *global_symtable;
SymStack stack;
bool WasReturn = false;

instr_node *main_gen = NULL;
instr_list_dynamic *instr_list = NULL;

Token current_token;
int PHASE = 1;

void Parse(FILE *file){

    // double a = 22.22;
    // fprintf(stderr, "float: %a\n", a);
    // PROGRAM(file);
    PHASE_FIRST(file);
    PHASE = 2;
    linenum = 0;
    rewind(file);
    fprintf(stderr, "FIRST PHASE DONE\n");


    // print_SymTable(global_symtable);
    // print_SymTable(&(stack.items[stack.top]));
    // print_SymTable(stack.items[stack.top]);

    // main_gen = init_instr_node();
    add_instr(&main_gen, "\n");
    main_gen->name_of_llist = "main";
    instr_list = init_instr_list_dynamic();
    Data data;
    generate_code(&main_gen, data, GEN_MAIN, 0, UNUSED);

    PHASE_SECOND(file);

    // generate_file();
    generate_header();
    print_list_names(instr_list);
    pop_all_lists_to_file(instr_list);
    pop_list_to_file(&main_gen);
    // print_SymTable(global_symtable);
    // print_SymTable(stack.items[1]);
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

    // print_SymTable(global_symtable);
    // printTree(global_symtable);

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


    while (current_token.token_type != T_EOF) {

        // char *str = current_token.string_value->str;

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
                // current_token = get_token(file); // get id

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
                    // add parameters to function symbol table and save it somewhere
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

            // if (current_token.token_type == T_KEYWORD &&
            //     strcmp(current_token.string_value->str, "return") &&
            //     // !(!strcmp(current_token.string_value->str, "var") || !strcmp(current_token.string_value->str, "let") ||
            //       // !strcmp(current_token.string_value->str, "func") )) {
            //       !strcmp(current_token.string_value->str, "func") ) {
            //   current_token = get_token(file); // get next token
            // }

            // if (current_token.token_type == T_KEYWORD && (!strcmp(current_token.string_value->str, "let") ||
            // !strcmp(current_token.string_value->str, "var")))  {
            // current_token = get_token(file); // skip var or let
            // }

            // else if (current_token.token_type == T_KEYWORD) {

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

    current_token = peekNextToken(file);

    // if (!(current_token.token_type == T_TYPE_ID ||
    // current_token.token_type == T_KEYWORD)) return;

    if (current_token.token_type == T_EOF || current_token.token_type == T_RBRACE) return;


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
            //check
            exitWithError("Syntax error: expected on if {\n", ERR_SYNTAX);
        }


        if (current_token.token_type != T_RBRACE) {

            SymTable *local_symtable = create_SymTable();
            local_symtable->name = "if";
            // s_push(&stack, *local_symtable);
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
                // s_push(&stack, *local_symtable);
                s_push(&stack, local_symtable);
                // Print_Sym_stack(&stack);
                STMT_LIST(file);
                // Print_Sym_stack(&stack);

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
    }

    else if (current_token.token_type == T_KEYWORD &&
             ((strcmp(str, "let") == 0) ||
              (strcmp(str, "var") == 0))) {  // let or var
        // get_token(file); // get let

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
        //Print_Sym_stack(&stack);
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
        // if (current_token.token_type == T_RBRACE){
        if (current_token.token_type != T_TYPE_ID && current_token.token_type != T_LPAR &&
            current_token.token_type != T_INT && current_token.token_type != T_DOUBLE &&
            current_token.token_type != T_SING_STRING &&  current_token.token_type != T_MUL_STRING &&
            current_token.token_type != T_EXPONENT_FLOAT && !(current_token.token_type == T_KEYWORD && !strcmp(current_token.string_value->str, "nil"))){
            exitWithError("Semantic error: return doesn't have an expression\n", ERR_SEMANT_RETURN);
            // return;
        }

        else {
            int error = 0;
            current_token = get_token(file); // get exp
            DataType type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);

            if (type == TYPE_UNKNOWN){
                //check?
                exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
            }


            // if (!is_compatible(node->data.returnType, type)) {
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

        WHILE_EXP(file);

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

        print_list_names(instr_list);

        instr_node *node = search_by_name_in_list(instr_list, func_name, main_gen);
        Data data;
        data.func_name = func_name;
        generate_code(&node, data, GEN_FUNC_START, 0, UNUSED);

        // instr_list[instr_list->size]->name_of_llist = func_name;

        SymTable *local_symtable = create_SymTable();


        strncpy(local_symtable->name, current_token.string_value->str, 255);
        local_symtable->name[255] = '\0';  // Ensure null-termination
        s_push(&stack, local_symtable);
        // Print_Sym_stack(&stack);

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

        // current_token = get_token(file); // get )
        if (current_token.token_type != T_RPAR){
            //check
            exitWithError("Syntax error: expected )\n", ERR_SYNTAX);
        }

        // current_token = get_token(file); // get -> or {
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

        // print_SymTable(stack.items[1]);


        s_pop(&stack);
        WasReturn = false;

        // data.op.id_name

        generate_code(&node, data, GEN_FUNC_END, 0, UNUSED);

        current_token = get_token(file); // get }
        fprintf(stderr, "current token: %s\n", current_token.string_value->str);
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

    // print_SymTable(stack.items[0]);
}

void ASSIGN_STMT_OR_FUNCALL(FILE *file){ //current token is id

    char *id_name = current_token.string_value->str;
    Token check_token = peekNextToken(file); // peek = or (
    // peekNextToken(file); // peek = or (


    // if (current_token.token_type == T_ASSIGN){
    if (check_token.token_type == T_ASSIGN){

        // SymTable *check_symtable = create_SymTable();
        // check_symtable = s_peek(&stack);
        // SymData *node_data = search_SymTable(check_symtable, id_name);
        // print_SymTable(stack.items[stack.top]);
        // Print_Sym_stack(&stack);
        // print_SymTable(stack.items[0]);
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
        //Print_Sym_stack(&stack);
        int error = 0;
        current_token = get_token(file); // get exp

        //        SymTable *check_symtable = create_SymTable();
        //        check_symtable = s_peek(&stack);
        //        // print_SymTable(global_symtable);
        //        Print_Sym_stack(&stack);
        //
        //        Data data;
        //        if (!strcmp(check_symtable->name, "global")) {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data->key;
        //            data.op.type = current_token.token_type;
        //            generate_code(&main_gen, data, GEN_MOVE, 0, GF);
        //        }
        //        else {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data->key;
        //            data.op.type = current_token.token_type;
        //            instr_node *node = search_by_name_in_list(instr_list, check_symtable->name, main_gen);
        //
        //            int deep = Get_deepness_of_var(&stack, node_data->key);
        //
        //            if (deep == 0)
        //                generate_code(&node, data, GEN_MOVE, deep, GF);
        //            else
        //                generate_code(&node, data, GEN_MOVE, deep, LF);
        //        }

        DataType type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
        if (type != TYPE_NIL) {
            node_data->data.isNil = false;
        }
        // else
        // node_data->data.isNil = true;

        if (!is_compatible(type_to_assign, type)) {
            //check
            exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
        }

        if (type == TYPE_UNKNOWN){
            //check?
            exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
        }
    }

    // TODO: check if function is declared
    // else if (current_token.token_type == T_LPAR) {
    else if (check_token.token_type == T_LPAR) {
        if (!strcmp(id_name, "write")) {
            current_token = get_token(file); // get (
            WRITE_CALLS(file);
            return;
        }

        // current_token = get_token(file); // get (
        // FUNC_CALLS(file);
        int error = 0;
        parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
        return;
    }

    else {
        //check
        current_token = get_token(file); // get exp
        int error = 0;
        parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
        // exitWithError("Syntax error: expected = or (\n", ERR_SYNTAX);
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
          current_token.token_type == T_TYPE_ID)) return;

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

    // PREFIX(file);

    // current_token = get_token(file); // get :
    // if (current_token.token_type != T_COLON){
    //check
    // exitWithError("Syntax error: expected :\n", ERR_SYNTAX);
    // }

    current_token = get_token(file); // get param


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

    // s_se


    // if (current_token.token_type != T_TYPE_ID){
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

// void MB_STMT_LET_VAR(FILE *file){ //current token is id
void MB_STMT_LET_VAR(FILE *file, bool changeable){ //current token is id
    SymData node_data;
    node_data = initSymData();

    // fseek(file, -strlen(current_token.string_value->str), SEEK_CUR);

    // node_data.canbeChanged = (strcmp(current_token.string_value->str, "var")) ? true : false;
    node_data.canbeChanged = changeable;

    // current_token = get_token(file); // get id
    node_data.name = current_token.string_value->str;

    SymTable *check_symtable = create_SymTable();
    check_symtable = s_peek(&stack);
    // print_SymTable(global_symtable);
    Print_Sym_stack(&stack);

    Data data;
    if (!strcmp(check_symtable->name, "global")) {
        data.op.id_name = node_data.name;
        generate_code(&main_gen, data, GEN_CREATE_ID, 0, GF);
    }
    else {
        data.op.id_name = node_data.name;
        instr_node *node = search_by_name_in_list(instr_list, check_symtable->name, main_gen);

        // int deep = Get_deepness_of_var(&stack, node_data.name);
        int deep = Get_deepness_current(&stack);

        generate_code(&node, data, GEN_CREATE_ID, deep, LF);
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
            node_data.isNil = true;
        }
        MB_ASSIGN_EXPR(file, type, &node_data);
        node_data.dtype = type;
    }
    else if (current_token.token_type == T_ASSIGN){
        current_token = get_token(file); // get exp
        int error = 0;
        //        Data data;
        //        if (!strcmp(check_symtable->name, "global")) {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data.name;
        //            data.op.type = current_token.token_type;
        //            generate_code(&main_gen, data, GEN_MOVE, 0, GF);
        //        }
        //        else {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data.name;
        //            data.op.type = current_token.token_type;
        //            instr_node *node = search_by_name_in_list(instr_list, check_symtable->name, main_gen);
        //            // int deep = Get_deepness_of_var(&stack, node_data.name);
        //            int deep = Get_deepness_current(&stack);
        //
        //            generate_code(&node, data, GEN_MOVE, deep, LF);
        //        }
        type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
        if (type == TYPE_UNKNOWN){
            exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
        }
        node_data.dtype = type;
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
    // node_data.isGlobal = false;
    node_data.isFunction = false;
    node_data.isDefined = true;
    insert_SymTable(check_symtable, node_data.name, node_data);

}

void MB_ASSIGN_EXPR(FILE *file, DataType type, SymData *node_data){ //current token is keyword
    current_token = peekNextToken(file); // check if next token is =


    if (current_token.token_type == T_ASSIGN){
        current_token = get_token(file); // get =
        current_token = get_token(file); // get exp

        //        SymTable *check_symtable = create_SymTable();
        //        check_symtable = s_peek(&stack);
        //        // print_SymTable(global_symtable);
        //        Print_Sym_stack(&stack);
        //
        //        Data data;
        //        if (!strcmp(check_symtable->name, "global")) {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data->name;
        //            data.op.type = current_token.token_type;
        //            generate_code(&main_gen, data, GEN_MOVE, 0, GF);
        //        }
        //        else {
        //            data.op.val = current_token.string_value->str;
        //            data.op.id_name = node_data->name;
        //            data.op.type = current_token.token_type;
        //            instr_node *node = search_by_name_in_list(instr_list, check_symtable->name, main_gen);
        //
        //            int deep = Get_deepness_current(&stack);
        //
        //            generate_code(&node, data, GEN_MOVE, deep, LF);
        //        }

        int error = 0;
        DataType exp_type = parse_expression(&stack, &current_token, &error, &file, main_gen, instr_list);
        if (type == TYPE_UNKNOWN){
            exitWithError("Syntax error: expression error\n", ERR_SYNTAX);
        }

        is_compatible(type, exp_type) ? 0 : exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);


        if (exp_type == TYPE_NIL) {
            node_data->isNil = true;
        }

        // if (type != typee){
        //check
        // exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
        // }
        // if (type != parse_expression(&stack, &current_token, &error, &file)){
        //check

        // exitWithError("Semantic error: type mismatch\n", ERR_SEMANT_TYPE);
        // }
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
    // char *str = current_token.string_value->str;

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

    // YEBKA PIZDEC, POTOM SDELAYEM NORMALNO
    else if (current_token.token_type == T_KEYWORD &&
             strcmp(current_token.string_value->str, "let") == 0)        {
        current_token = get_token(file); // get let


        current_token = get_token(file); // get id
        if (current_token.token_type != T_TYPE_ID){
            //check
            exitWithError("Syntax error: expected id on let\n", ERR_SYNTAX);
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