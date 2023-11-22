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
    globalFuncData.paramTypes.dataType = FUNC; // No parameters
    globalFuncData.paramTypes.name = NULL;
    globalFuncData.paramTypes.prefix = PREFIX_DEFAULT;
    globalFuncData.paramCount = 0; // No parameters
    globalFuncData.local_SymTable = NULL; // Initially, no local symbol table
    globalFuncData.isNil = false;

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


void parseStatementList(FILE* file) {
    while (true) {
        Token nextToken = peekNextToken(file);
        
        if (nextToken.token_type == T_RBRACE || nextToken.token_type == T_EOF) {
            break;
        }

        // Parse individual statement
        Token statementToken = get_token(file);
        parseStatement(statementToken, file);
    }
}


void parseStatement(Token token, FILE* file) {
    printf("Token type: %d, Token value: %s\n", token.token_type, token.string_value->str);
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
    } else if (strcmp(token.string_value->str, "let") == 0 || strcmp(token.string_value->str, "var") == 0) {
        parseLetOrVarStatement(file, token);
    } else {
        fprintf(stderr, "Error: Unexpected keyword '%s'\n", token.string_value->str);
        exit(1);
    }
}




void parseLetOrVarStatement(FILE* file, Token keywordToken) {
    SymData VariableData;
    bool typeSpecified = false;
    VariableData.canbeChanged = (strcmp(keywordToken.string_value->str, "var") == 0);

    Token identifierToken = get_token(file);
    if (identifierToken.token_type != T_TYPE_ID) {
        fprintf(stderr, "Error: Expected identifier after '%s'\n", keywordToken.string_value->str);
        exit(1);
    }

    VariableData.name = identifierToken.string_value->str; // Copy the variable name

    Token nextToken = peekNextToken(file); // Look ahead without consuming the token

    // Check if there is a type specifier or an assignment
    if (nextToken.token_type == T_COLON) {
        get_token(file); // Consume the colon token
        VariableData.dtype = parseType(file); // Parse the type after the colon
        typeSpecified = true;
    }
    VariableData.isDefined = (search_SymTable(Globaltable, VariableData.name) != NULL);
    insert_SymTable(Globaltable, VariableData.name, VariableData); // Insert the variable into the symbol table

    nextToken = peekNextToken(file);
    if (nextToken.token_type == T_ASSIGN) {

        get_token(file); 
        Token assignToken = peekNextToken(file); 
        DataType VariableAssignType = parse_expression(Globaltable, &assignToken, &error, &file);

        if(!typeSpecified) {
            VariableData.dtype = VariableAssignType;
        } else if (VariableAssignType != VariableData.dtype) {
            fprintf(stderr, "Error: Type mismatch in assignment\n");
            exit(1);
        }
    } 
}







void parseWhileStatement(FILE* file) {
    Token hui = get_token(file);
    if (hui.token_type != T_LPAR) {
        fprintf(stderr,"Error: Expected '(' after 'while'\n");
        exit(1);
    }

    if(!parse_expression(Globaltable, &hui,&error,&file)) {
        fprintf(stderr,"ERRRRRRORRRRR EXPRESSSSION ALERT ALERT ALERT\n");
        exit(1);
    }


    if (hui.token_type != T_LBRACE) {
        fprintf(stderr,"Error: Expected '{' to start the while block\n");
        exit(1);
    }

    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        Token state_while = get_token(file);
        parseStatement(state_while, file);
    }

    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        fprintf(stderr,"Error: Expected '}' to close the while block\n");
        exit(1);
    }
}



void parseIfStatement(FILE* file) {

    Token nextToken = peekNextToken(file);
    
    // Check if the next token is 'let', indicating the 'if let a' syntax
    if (nextToken.token_type == T_KEYWORD && strcmp(nextToken.string_value->str, "let") == 0) {
        get_token(file); // Consume 'let'
        Token idToken = get_token(file);
        if (idToken.token_type != T_TYPE_ID) {
            fprintf(stderr, "Error: Expected identifier after 'let'\n");
            exit(1);
        }

    } else {
        Token openParen = get_token(file);
        if (openParen.token_type != T_LPAR) {
            fprintf(stderr, "Error: Expected '(' after 'if'\n");
            exit(1);
        }

        if(!parse_expression(Globaltable, &openParen,&error,&file)) {
            fprintf(stderr,"ERRRRRRORRRRR EXPRESSSSION ALERT ALERT ALERT\n");
            exit(1);
        }

        if (openParen.token_type != T_LBRACE) {
            fprintf(stderr,"Error: Expected '{' after if condition\n");
            exit(1);
        }

        Token check = peekNextToken(file);
        if (check.token_type != T_RBRACE) {

            parseStatementList(file);
            get_token(file);
        } else {
            get_token(file);
        }
    }



    Token elseToken = get_token(file);
    printf("Token type: %s, Token value: %s\n", tokenTypeNames[elseToken.token_type], elseToken.string_value->str);
    if (elseToken.token_type != T_KEYWORD || strcmp(elseToken.string_value->str, "else") != 0) {
        fprintf(stderr,"Error: 'else' clause is required after 'if'\n");
        exit(1);
    }



    parseElseBlock(file);
}


void parseElseBlock(FILE* file) {
    Token openBrace = get_token(file);
    if (openBrace.token_type != T_LBRACE) {
        fprintf(stderr, "Error: Expected '{' after 'else'\n");
        exit(1);
    }

    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        parseStatementList(file);
    }

    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        fprintf(stderr, "Error: Expected '}' after else block\n");
        exit(1);
    }
}



void parseFunction(FILE* file) {
    Token funcName = get_token(file);
    bool hasReturnStatement = false;
    if (funcName.token_type != T_TYPE_ID) {
        fprintf(stderr,"Error: Expected function name after 'func'\n");
        exit(1);
    }

    Token openParen = get_token(file);
    if (openParen.token_type != T_LPAR) {
        fprintf(stderr,"Error: Expected '(' after function name\n");
        exit(1);
    }

    ListFuncParam* params = parseParamList(file);
    int paramCount = countParams(params);

 
    DataType return_type = TYPE_VOID;

    Token arrowToken = peekNextToken(file);
    if (arrowToken.token_type == T_ARROW) {
        get_token(file);

        return_type = parseType(file);
    }

    insert_FunctionSymTable(Globaltable, funcName.string_value->str, return_type, params, paramCount);


    // Expecting '{'
    Token openBrace = get_token(file);
    if (openBrace.token_type != T_LBRACE) {
        fprintf(stderr,"Error: Expected '{' before function body\n");
        exit(1);
    }
// Parse the function body
    Token statementToken;
    while ((statementToken = peekNextToken(file)).token_type != T_RBRACE) {
    if (statementToken.token_type == T_EOF) {
        fprintf(stderr, "Error: Unexpected end of file within function body\n");
        exit(1);
    }

    // Consume the token before passing to parseStatement or parseReturnStatement
    get_token(file);

    // Check if it's a return statement, check the return type for non-void functions
    if (strcmp(statementToken.string_value->str, "return") == 0) {
        hasReturnStatement = true;
        // Here, you would parse the return statement
        // DataType exprType = parseReturnType(file);
        // if (return_type != TYPE_VOID && exprType != return_type) {
        //     fprintf(stderr, "Error: Return type does not match function return type\n");
        //     exit(1);
        // }
    } else {
        // Parse other statements
        parseStatement(statementToken, file); // Assuming parseStatement handles various statements
        }
    }


    if (return_type != TYPE_VOID && !hasReturnStatement) {
        fprintf(stderr,"Error: Non-void function must have a return statement\n");
        exit(1);
    } else if (return_type == TYPE_VOID && hasReturnStatement) {
        fprintf(stderr,"Error: 'void' function should not have a return statement\n");
        exit(1);
    }

    // Expecting '}'
    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        fprintf(stderr,"Error: Expected '}' after function body\n");
        exit(1);
    }

    printf("Function parsed successfully\n");
}


ListFuncParam* parseParamList(FILE* file) {
    ListFuncParam* paramList = NULL;

    while (true) {
        // Check for end of parameter list
        Token nextToken = peekNextToken(file);
        if (nextToken.token_type == T_RPAR) {
            get_token(file); // Consume the right parenthesis
            break;
        }

        // Parse the prefix
        Token prefixToken = get_token(file); // Consume the prefix token
        ParamPrefix prefix = parsePrefixFunction(prefixToken, file); // Duplicate the prefix string
        // Parse the parameter name
        Token nameToken = get_token(file);
        if (nameToken.token_type != T_TYPE_ID) {
            fprintf(stderr,"Error: Expected parameter name in parameter list\n");
            exit(1);
        }
        char paramName[256];
        strcpy(paramName, nameToken.string_value->str);
        // Expecting ':'
        Token colonToken = get_token(file);
        if (colonToken.token_type != T_COLON) {
            fprintf(stderr,"Error: Expected ':' after parameter name\n");
            exit(1);
        }

        // Parse the type
        DataType paramType = parseType(file);

        // Add the parameter to the list
        paramList = addParamToList(paramList, paramName, paramType, prefix);

        // Check for comma or end of list
        nextToken = peekNextToken(file);
        if (nextToken.token_type == T_COMMA) {
            get_token(file); // Consume the comma
        } else if (nextToken.token_type != T_RPAR) {
            fprintf(stderr,"Error: Expected ',' or ')' after parameter definition\n");
            exit(1);
        }
    }

    return paramList;
}






// REWRITE THIS FUNCTION
ParamPrefix parsePrefixFunction(Token PrefixToken, FILE* file) {
    Token token = peekNextToken(file); 
    if (token.token_type == T_TYPE_ID || token.token_type == T_UNDERSCORE_ID) {
        if (PrefixToken.token_type == T_UNDERSCORE_ID) {
            return PREFIX_UNDERSCORE;
    }   else {
        return PREFIX_DEFAULT;
    }
} else {
    fprintf(stderr,"ERRRRRRRRRRRRRRRROR NO PREFIX\n");
    exit(1);
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
            fprintf(stderr,"Error: Unknown type\n");
            exit(1);
        }
    } else {
        fprintf(stderr,"Error: Expected a type\n");
        exit(1);
        }
    }
