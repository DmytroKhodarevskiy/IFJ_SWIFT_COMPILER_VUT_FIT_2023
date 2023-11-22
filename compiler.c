#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "expression_parse.c"

#include "parser.c"
#include "codegenerator.c"


// int main(int argc, char* argv[]){
int main(){

    // parsing();

    // if (get_index_from_token(token) != 8){
    //   lex_ok = parse_expression(&token, &error, &file);
    //   else {
    //   }
    //     printf("Lexical analysis failed\n");
    //   if (lex_ok) printf("Lexical analysis OK\n");
    // }

    generate_file();

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


    // c = a + b
    Data data = init_data();
    instr_node *head = NULL;
    // data.op2.id_name = token_b.string_value;

    data.func_name = "foo";
    generate_code(&head, data, GEN_FUNC_START);

    data.op1.id_name = "a";
    data.op1.int_val = 56;
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_MOVE);
    generate_code(&head, data, GEN_PUSH);

    data.op1.id_name = "b";
    data.op1.int_val = 7;
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_MOVE);
    generate_code(&head, data, GEN_PUSH);

    generate_code(&head, data, GEN_ADD);

    data.op1.id_name = "c";
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_ASSIGN);  
    generate_code(&head, data, GEN_WRITE);

    generate_code(&head, data, GEN_FUNC_END);

    generate_code(&head, data, GEN_MAIN);

    pop_list_to_file(&head);

  return 0;
}
