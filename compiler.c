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
    Token token_a;
    Token token_b;
    token_a = init_token();
    token_b = init_token();

    token_a.token_type = T_INT;
    token_b.token_type = T_INT;

    token_a.int_value = 55;
    token_b.int_value = 6;

    token_a.string_value->str = "a";
    token_b.string_value->str = "b";


    // c = a + b
    Data data = init_data();
    // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAPOKAMAKLONILSIATIHORAZRIAZHAIUSTVOL\n");
    instr_node *head = NULL;
    // data.op2.id_name = token_b.string_value;
    data.op1.id_name = token_a.string_value->str;
    data.op1.int_val = token_a.int_value;
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_MOVE);
    generate_code(&head, data, GEN_PUSH);

    // printf("head: %s", head->instr);
    // printf("head: %s", head->next->instr);
    // printf("head: %s", head->next->next->instr);

    data.op1.id_name = token_b.string_value->str;
    data.op1.int_val = token_b.int_value;
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_MOVE);
    generate_code(&head, data, GEN_PUSH);

    // printf("asdassssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss%s\n", data.op1.id_name);
    generate_code(&head, data, GEN_ADD);

    data.op1.id_name = "c";
    generate_code(&head, data, GEN_CREATE_ID);
    generate_code(&head, data, GEN_ASSIGN);  
    generate_code(&head, data, GEN_WRITE);

    pop_list_to_file(&head);

  return 0;
}
