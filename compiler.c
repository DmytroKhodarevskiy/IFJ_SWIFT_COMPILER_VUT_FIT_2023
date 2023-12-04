
//#include "expression_parse.c"



// #include "expression_parse.c"
// #include "memory.h"
// #include "parse.h"
// #include "parse.c"
#include "compiler.h"
// #include "codegenerator.h"

int linenum = 1;

// int main(){
int main(int argc, char* argv[]){

    FILE *file;

    // If there is an argument (file name), open that file.
    if (argc > 1) {
        file = fopen(argv[1], "r"); // Opens the file for reading.
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
    } else {
        // If no file is specified, read from stdin.
        file = stdin;
    }

    // printf("file: %s\n", argv[1]);

    // FIRST PHASE
    Parse(file);

    







    // Token token;
    // // int error = 0;
    // // int lex_ok = 1;
    // while (token.token_type != T_EOF) {
    //     token = init_token();
    //     token = get_token(file);

    //     printf("TOKEN TYPE: %d\n", token.token_type);
    //     printf("TOKEN STRING: %s\n", token.string_value->str);

    //     if (token.token_type == T_ERR) {
    //         exitWithError("Lexical analysis failed\n", ERR_LEX);
    //         break;
    //     }
    // }
    
    // printf("Lexical analysis OK\n");
    // if (get_index_from_token(token) != 8){
    //   lex_ok = parse_expression(&token, &error, &file);
    //   else {
    //   }
    //     printf("Lexical analysis failed\n");
    //   if (lex_ok) printf("Lexical analysis OK\n");
    // }

    // generate_file();

    // int a = 5;
    // int b = 6;
    // Token token_a;
    // Token token_b;
    // token_a = init_token();
    // token_b = init_token();

    // token_a.token_type = T_INT;
    // token_b.token_type = T_INT;

    // token_a.int_value = 55;
    // token_b.int_value = 6;

    // token_a.string_value->str = "a";
    // token_b.string_value->str = "b";


    // // c = a + b
    // Data data = init_data();
    // instr_node *head = NULL;

    // Operand op1, op2;

    // op1.id_name = "param1";
    // op2.id_name = "param2";

    // op1.int_val = 55;
    // op2.int_val = 66;
    // // data.op2.id_name = token_b.string_value;
    // // *data.func_param = op1;
    // // *(data.func_param+1) = op2;
    // data.func_param = malloc(2 * sizeof(Operand));
    // // if (data.func_param == NULL) {
    // //     // Handle allocation error
    // // }

    // data.func_param[0] = op1;
    // // printf("data.func_param[0].id_naaaaaaame: %s\n", data.func_param[0].id_name);
    // data.func_param[1] = op2;

    // data.func_name = "foo";
    // generate_code(&head, data, GEN_FUNC_START, UNUSED);

    // data.op1.id_name = "a";
    // data.op1.int_val = 6;
    // generate_code(&head, data, GEN_CREATE_ID, LF);
    // generate_code(&head, data, GEN_MOVE_INT, LF);
    // generate_code(&head, data, GEN_PUSH, LF);

    // data.op1.id_name = "b";
    // data.op1.int_val = 7;
    // generate_code(&head, data, GEN_CREATE_ID, LF);
    // generate_code(&head, data, GEN_MOVE_INT, LF);
    // generate_code(&head, data, GEN_PUSH, LF);

    // generate_code(&head, data, GEN_ADD, UNUSED);

    // data.op1.id_name = "c";
    // generate_code(&head, data, GEN_CREATE_ID, LF);
    // generate_code(&head, data, GEN_ASSIGN, LF);  
    // generate_code(&head, data, GEN_WRITE, LF);

    // generate_code(&head, data, GEN_FUNC_END, UNUSED);


    // // print_list(head);

    // // printf("=====================================\n");
    // generate_code(&head, data, GEN_MAIN, UNUSED);

    // data.op1.id_name = "global_var";
    // data.op1.int_val = 1337;
    // generate_code(&head, data, GEN_CREATE_ID, GF);
    // generate_code(&head, data, GEN_MOVE_INT, GF);
    // generate_code(&head, data, GEN_PUSH, GF);

    // generate_code(&head, data, GEN_WRITE, GF);

    // generate_code(&head, data, GEN_FUNC_CALL, UNUSED);

    // // print_list(head);

    // pop_list_to_file(&head);

    // fclose(file);
    // free_all();
    // printMemoryList();
    // exitWithError("", ERR_OK);
}
