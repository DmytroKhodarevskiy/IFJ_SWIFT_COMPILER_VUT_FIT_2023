#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "expression_parse.c"



// #include "expression_parse.c"

#include "parser.c"


//FILE* read_file(char* filename){
//  FILE* file = fopen(filename, "r");
//  if(file == NULL){
//    printf("Error: File not found\n");
//    exit(1);
//  }
//  return file;
//}
// int main(int argc, char* argv[]){
int main(){

    parsing();
//  FILE* file = read_file("test.txt");
//
//    Token token = get_token( file);
//    bool lex_ok = true;
//    int error_exp = 0;
//
//     if (get_index_from_token(token) != 8) lex_ok = parse_expression(&token, &error_exp, &file);
//
//     if(lex_ok) printf("Expression OK\n");
//     else printf("Expression Error\n");

  



  return 0;
}
