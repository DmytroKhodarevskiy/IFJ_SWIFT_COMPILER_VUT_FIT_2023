#include "tokenizer.c"
#include "symtable.c"
#include <stdio.h>
#include <string.h>
#include "parser.h"






FILE* read_file(char* filename){
  FILE* file = fopen(filename, "r");
  if(file == NULL){
    printf("Error: File not found\n");
    exit(1);
  }
  return file;
}



void parsing() {
    FILE* file = read_file("test.txt");
    Token token = get_token(file);

    if (token.token_type == T_KEYWORD && strcmp(token.string_value->str, "func") == 0) {

        Token funcName = get_token(file);
        if (funcName.token_type != T_TYPE_ID) {
            printf("Error: Expected function name after 'func'\n");
            return;
        }


        Token openParen = get_token(file);
        if (openParen.token_type != T_LPAR) {
            printf("Error: Expected '(' after function name\n");
            return;
        }

        ListFuncParam* params = parseParamList(file);
        int paramCount = countParams(params);

        // Expecting ')'
        Token closeParen = get_token(file);
        if (closeParen.token_type != T_RPAR) {
            printf("Error: Expected ')' after parameter list\n");
            return;
        }

        // Parse return type after '->'
        Token arrow = get_token(file);
        if (arrow.token_type != T_ARROW) {
            printf("Error: Expected '->' after parameter list\n");
            return;
        }

        // Call a function to parse the return type
        DataType return_type = parseType(file);

        SymTable* table = create_SymTable();
        insert_FunctionSymTable(table, funcName.string_value->str, return_type, params, paramCount);



        print_SymTable(table);
        // Parse the statement list within the function body
        // Expecting '{'
        Token openBrace = get_token(file);
        if (openBrace.token_type != T_LBRACE) {
            printf("Error: Expected '{' before function body\n");
            return;
        }

    //     // Call a function to parse the statement list
    //     parseStmtList(file);

        // Expecting '}'
        Token closeBrace = get_token(file);
        if (closeBrace.token_type != T_RBRACE) {
            printf("Error: Expected '}' after function body\n");
            return;
        }

        printf("Function parsed successfully\n");
    } else {
        printf("Error: Expected function declaration\n");
    }
}


ListFuncParam* parseParamList(FILE* file) {
    ListFuncParam* paramList = NULL;
    bool firstParam = true;

    while (true) {
        Token token = peekNextToken(file);

        if (token.token_type == T_RPAR) {
            break;
        }

        if (!firstParam) {
            token = get_token(file);
            if (token.token_type != T_COMMA) {
                printf("Error: Expected a comma between parameters\n");
                return paramList; // Return the partially constructed list or handle the error
            }
        }

        ParamPrefix prefix = parsePrefixFunction(file);

        token = get_token(file);
        if (token.token_type != T_TYPE_ID) {
            printf("Error: Expected an identifier for parameter\n");
            return paramList; // Return the partially constructed list or handle the error
        }
        char paramName[256];
        strcpy(paramName, token.string_value->str);


        token = get_token(file);
        if (token.token_type != T_COLON) {
            printf("Error: Expected a colon after parameter name\n");
            return paramList; // Return the partially constructed list or handle the error
        }


        DataType type_of_param = parseType(file); 
        // Add the parameter to the list
        paramList = addParamToList(paramList, paramName, type_of_param);
        firstParam = false; 
         
    }
    return paramList;
}





// REWRITE THIS FUNCTION
ParamPrefix parsePrefixFunction(FILE* file) {
    Token token = peekNextToken(file); 
    if (token.token_type == T_TYPE_ID || token.token_type == T_UNDERSCORE_ID) {
       
        if (strcmp(token.string_value->str, "with") == 0) {
            
            get_token(file); 
            return PREFIX_WITH;
        } else if (strcmp(token.string_value->str, "of") == 0) {
            get_token(file); 
            return PREFIX_OF;
        } else if (strcmp(token.string_value->str, "by") == 0) {
            get_token(file);
            return PREFIX_BY;
        } else if (strcmp(token.string_value->str, "_") == 0) {
            get_token(file); 

            return PREFIX_UNDERSCORE;
        } else {
         
            return PREFIX_NONE;
        }
    } else {
        
        return PREFIX_NONE;
    }
}


DataType parseType(FILE* file) {
    Token token = get_token(file); 

    if (token.token_type == T_KEYWORD) {
        if (strcmp(token.string_value->str, "String") == 0) {
            return TYPE_STRING;
        } else if(strcmp(token.string_value->str, "Int") == 0) {
            return TYPE_INT;
        } else if(strcmp(token.string_value->str, "Double") == 0) {
            return TYPE_DOUBLE;
        } else if (strcmp(token.string_value->str, "String?") == 0) {
            return TYPE_STRING_NULLABLE;
        } else if (strcmp(token.string_value->str, "Int?") == 0) {
            return TYPE_INT_NULLABLE;
        } else if (strcmp(token.string_value->str, "Double?") == 0) {
            return TYPE_DOUBLE_NULLABLE;
        } else {
            printf("Error: Unknown type\n");
            //ADD ERROR
             return TYPE_DOUBLE_NULLABLE;
        }
    } else if (token.token_type == T_EMPTY) {
        printf("Handle something\n");
        return;
    } else {
        printf("Error: Expected a type\n");
        //ADD ERROR
        return;
        }
    }
