#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression_parse.c"


FILE* read_file(char* filename){
  FILE* file = fopen(filename, "r");
  if(file == NULL){
    printf("Error: File not found\n");
    exit(1);
  }
  return file;
}

// int main(int argc, char* argv[]){
int main(){

  FILE* file = read_file("test.txt");
  int error = 0;

  while (true){

    if (feof(file)){
      break;
    }


    Token token = get_token(file);
    bool lex_ok;

    if (token.token_type == T_TYPE_ID || token.token_type == T_LPAR){
      lex_ok = parse_expression(token, &error, &file);

      printf("Syntax analysis: ");

      if (lex_ok) printf("Syntax analysis OK\n");
      else printf("Syntax analysis failed\n");
    }

    if (token.token_type == T_ERR){
      printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
      printf("Error: Invalid token\n");
      exit(1);
    }

    // printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
  }

  return 0;
}
