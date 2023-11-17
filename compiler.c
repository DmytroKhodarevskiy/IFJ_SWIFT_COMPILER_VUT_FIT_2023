#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression_parse.c"
// #include "tokenizer.c"


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
  bool lex_ok = false;

  while (true){
    if (feof(file)){
      break;
    }


    Token token = get_token(file);
    // printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
    bool lex_ok;

    if (token.token_type == T_TYPE_ID){
      lex_ok = parse_expression(token, &error, file);
      token = get_token(file);

      printf("%d\n", lex_ok);
    }

 
    
    // break;

    if (token.token_type == T_ERR){
      printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
      printf("Error: Invalid token\n");
      exit(1);
    }



    printf("Token type: %s, Token value: %s\n", tokenTypeNames[token.token_type], token.string_value->str);
  }
  
  printf("%daaaaaaaa\n", lex_ok);


  if (lex_ok) printf("Lexical analysis OK\n");
  else printf("Lexical analysis failed\n");

  return 0;
}
