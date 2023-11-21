
// #include "tokenizer.c"
#include "symtable.c"
#include "expression_parse.c"
#include <stdio.h>
#include <string.h>
#include "parser.h"



SymTable* Globaltable;
int error = 0;

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
    Globaltable = create_SymTable();


    // Define and insert the global abstract function
    SymData globalFuncData;
    globalFuncData.name = "Program"; // Give a name to the abstract function
    globalFuncData.dtype = FUNC; // Mark it as a function type
    globalFuncData.isDefined = true; // It is defined as it acts as a placeholder
    globalFuncData.canbeChanged = false; // It cannot be changed
    globalFuncData.isGlobal = true; // It is global
    globalFuncData.isFunction = true; // It is a function
    globalFuncData.returnType = TYPE_VOID; // Set return type as void
    globalFuncData.paramTypes.next = NULL; // No parameters
    globalFuncData.paramCount = 0; // No parameters
    globalFuncData.local_SymTable = NULL; // Initially, no local symbol table

    insert_SymTable(Globaltable, globalFuncData.name, globalFuncData);

     while (true) {
        Token token = get_token(file);

        if (token.token_type == T_EOF || token.token_type == T_ERR) {
            break;
        }

           parseStatement(token, file);
    }

    printTree(Globaltable);
    print_SymTable(Globaltable);
}





void parseStatement(Token token, FILE* file) {
    if (token.token_type != T_KEYWORD) {
        printf("Error: Expected a keyword starting a statement\n");
        return;
    }

    if (strcmp(token.string_value->str, "func") == 0) {
        parseFunction(file);
    } else if (strcmp(token.string_value->str, "if") == 0) {
        parseIfStatement(file);
    } else if (strcmp(token.string_value->str, "while") == 0) {
        parseWhileStatement(file);
    } else if (strcmp(token.string_value->str, "let") == 0) {
        parseLetOrVarStatement(file, token);
    } else {
        printf("Error: Unexpected keyword '%s'\n", token.string_value->str);
    }
}




void parseLetOrVarStatement(FILE* file, Token keywordToken) {
    bool isLetStatement = (strcmp(keywordToken.string_value->str, "let") == 0);

    Token identifierToken = get_token(file);
    if (identifierToken.token_type != T_TYPE_ID) {
        printf("Error: Expected identifier after '%s'\n", keywordToken.string_value->str);
        return;
    }

    Token nextToken = get_token(file);

    if (nextToken.token_type == T_COLON) {
        DataType declaredType = parseType(file);

        // TODO: Add logic to handle the new variable with its type in the symbol table
        // If it's a let statement, the variable is immutable
    } else if (nextToken.token_type == T_ASSIGN) {
        // parse_expression(file);

        // TODO: Add logic to handle the new variable and its assigned value in the symbol table
    } else {
        printf("Error: Expected ':' or '=' after identifier in '%s' statement\n", keywordToken.string_value->str);
    }
}












void parseWhileStatement(FILE* file) {
    Token hui = get_token(file);
    if (hui.token_type != T_LPAR) {
        printf("Error: Expected '(' after 'while'\n");
        return;
    }

    if(!parse_expression(&hui,&error,&file)) {
        printf("ERRRRRRORRRRR EXPRESSSSION ALERT ALERT ALERT\n");
        return;
    }


    if (hui.token_type != T_LBRACE) {
        printf("Error: Expected '{' to start the while block\n");
        return;
    }

    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        Token state_while = get_token(file);
        parseStatement(state_while, file);
    }

    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        printf("Error: Expected '}' to close the while block\n");
        return;
    }
}



void parseIfStatement(FILE* file) {
    Token openParen = get_token(file);
    if (openParen.token_type != T_LPAR) {
        printf("Error: Expected '(' after 'if'\n");
        return;
    }

    if(!parse_expression(&openParen,&error,&file)) {
        printf("ERRRRRRORRRRR EXPRESSSSION ALERT ALERT ALERT\n");
        return;
    }

    if (openParen.token_type != T_LBRACE) {
        printf("Error: Expected '{' after if condition\n");
        return;
    }

    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        Token state_if = get_token(file);
        parseStatement(state_if, file);
    } else {
        get_token(file);
    }

    Token elseToken = get_token(file);
    if (elseToken.token_type != T_KEYWORD || strcmp(elseToken.string_value->str, "else") != 0) {
        printf("Error: 'else' clause is required after 'if'\n");
        return;
    }

    parseElseBlock(file);
}


void parseElseBlock(FILE* file) {
    Token openBrace = get_token(file);
    if (openBrace.token_type != T_LBRACE) {
        printf("Error: Expected '{' after 'else'\n");
        return;
    }

    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        Token state_else = get_token(file);
        parseStatement(state_else, file);
    }

    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        printf("Error: Expected '}' after else block\n");
        return;
    }
}



void parseFunction(FILE* file) {
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

    DataType return_type = parseType(file);

    //insert_FunctionSymTable(Globaltable, funcName.string_value->str, return_type, params, paramCount);


    // Expecting '{'
    Token openBrace = get_token(file);
    if (openBrace.token_type != T_LBRACE) {
        printf("Error: Expected '{' before function body\n");
        return;
    }

    // Call a function to parse the statement list
    // parseStmtList(file);

    // Expecting '}'
    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        printf("Error: Expected '}' after function body\n");
        return;
    }

    printf("Function parsed successfully\n");
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
        return 1;
    } else {
        printf("Error: Expected a type\n");
        //ADD ERROR
        return 1;
        }
    }
