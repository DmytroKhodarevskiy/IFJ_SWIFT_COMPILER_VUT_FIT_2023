
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
     printf("token: %s\n", token.string_value->str);
    if (token.token_type != T_KEYWORD && token.token_type != T_TYPE_ID) {
        fprintf(stderr, "Error: Expected a keyword starting a statement\n");
        exit(2); // Syntactic analysis error
    }

    if(strcmp(token.string_value->str, "return") == 0){
        pars
    
        
    else if (!is_compatible(variableData->dtype, rhsType)){
        fprintf(stderr, "Error: Type mismatch in assignment\n");
        exit(7); 
    }

    variableData->isDefined = true;
    if (!variableData->dtype) {
        variableData->dtype = rhsType;
    }

}

bool isNullableType(DataType type) {
    return type == TYPE_INT_NULLABLE || type == TYPE_DOUBLE_NULLABLE || type == TYPE_STRING_NULLABLE;
}




void parseLetOrVarStatement(FILE* file, Token keywordToken) {
    SymData VariableData; // Initialize SymData
    bool typeSpecified = false;
    bool assignmentPresent = false;
    VariableData.canbeChanged = (strcmp(keywordToken.string_value->str, "var") == 0);

    Token identifierToken = get_token(file);

    if (identifierToken.token_type != T_TYPE_ID) {

        fprintf(stderr, "Error: Expected identifier after '%s'\n", keywordToken.string_value->str);
        exit(2); // Syntactic analysis error
    }  else if (search_SymTable(Globaltable, identifierToken.string_value->str) != NULL) {
        fprintf(stderr, "ERRor ID already:\n");
        exit(6);
    }

    VariableData.name = identifierToken.string_value->str; // Copy the variable name

    Token nextToken = peekNextToken(file); // Look ahead without consuming the token

    // Check if there is a type specifier
    if (nextToken.token_type == T_COLON) {
        get_token(file); // Consume the colon token
        VariableData.dtype = parseType(file); // Parse the type after the colon
        typeSpecified = true;
    }

    // Check for assignment
    nextToken = peekNextToken(file);
    if (nextToken.token_type == T_ASSIGN) {
        get_token(file); // Consume the assignment token
        assignmentPresent = true;

        Token assignToken = get_token(file); // Get the token for the assignment value
        DataType VariableAssignType;

        
       
        VariableAssignType = parse_expression(Globaltable, &assignToken, &error, &file);    
         
        fseek(file, -strlen(assignToken.string_value->str), SEEK_CUR);
      

       
        if(VariableAssignType == TYPE_NIL) {
            VariableData.isNil = true;
        } else {
            VariableData.isNil = false;
        }

        // Check type compatibility, allowing nil for nullable types
        if (!typeSpecified) {
            VariableData.dtype = VariableAssignType;
        } else if (!is_compatible(VariableData.dtype, VariableAssignType)) {
            fprintf(stderr, "Error: Type mismatch in assignment\n");
            exit(7); // Semantic error of type compatibility
        } else if (VariableAssignType == TYPE_NIL && !isNullableType(VariableData.dtype)) {
            fprintf(stderr, "Error: Cannot assign 'nil' to a non-nullable type\n");
            exit(7); // Semantic error of type compatibility
        }
    } 

    // Check if either type or assignment is specified
    if (!typeSpecified && !assignmentPresent) {
        fprintf(stderr, "Error: Variable declaration must specify a type or an initializer\n");
        exit(2); // Syntactic analysis error
    }

    // Update or insert the variable into the symbol table
    VariableData.isDefined = (search_SymTable(Globaltable, VariableData.name) != NULL);
    if (VariableData.isDefined) {
        updateSymData(Globaltable, VariableData.name, VariableData);
    } else {
        insert_SymTable(Globaltable, VariableData.name, VariableData);
    }

}






void parseWhileStatement(FILE* file) {
    Token hui = get_token(file);
    if (hui.token_type != T_LPAR) {
        fprintf(stderr,"Error: Expected '(' after 'while'\n");
        exit(1);
    }

    DataType WhileStatement = parse_expression(Globaltable, &hui,&error,&file);

    if(WhileStatement != TYPE_BOOL) {
        fprintf(stderr, "ERROR WHILE STATEMENT");
        exit(6);
    }


    if (hui.token_type != T_LBRACE) {
        fprintf(stderr,"Error: Expected '{' to start the while block\n");
        exit(1);
    }


    // Parse the statement list inside the while block
    Token check = peekNextToken(file);
    if (check.token_type != T_RBRACE) {
        parseStatementList(file);
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
        AVLNode* node = search_SymTable(Globaltable, idToken.string_value->str);
        if (node == NULL) {
            fprintf(stderr, "Error: Variable '%s' not declared\n", idToken.string_value->str);
            exit(5);
        }

        SymData variableData = getSymDataByKey(Globaltable, idToken.string_value->str);

        
        if (variableData.canbeChanged) {
            fprintf(stderr, "Error: Variable '%s' not a constant\n", idToken.string_value->str);
            exit(6);
        }


        //TODO SCOPE
        if(variableData.isNil) {
            parseElseBlock(file);
        } else {
           if(variableData.dtype == TYPE_INT_NULLABLE) {
               variableData.dtype = TYPE_INT;
           }  else if (variableData.dtype == TYPE_DOUBLE_NULLABLE) {
               variableData.dtype = TYPE_DOUBLE;
           } else if (variableData.dtype == TYPE_STRING_NULLABLE) {
               variableData.dtype = TYPE_STRING;
            }


        Token openParen = get_token(file);
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

    } else {
        Token openParen = get_token(file);
        if (openParen.token_type != T_LPAR) {
            fprintf(stderr, "Error: Expected '(' after 'if'\n");
            exit(1);
        }

        //parse if condition

        DataType IfCondition = parse_expression(Globaltable, &openParen, &error, &file);
        if(IfCondition != TYPE_BOOL) {
            fprintf(stderr,"ERROR IN IF STATEMENT\n");
            exit(6);
        }

        if (openParen.token_type != T_LBRACE) {
            fprintf(stderr,"Error: Expected '{' after if condition\n");
            exit(1);
        }

        Token check = peekNextToken(file);

        //parse if block
        if (check.token_type != T_RBRACE) {       
            parseStatementList(file);
            get_token(file);
        } else {
            get_token(file);
        }
    }



    Token elseToken = get_token(file);
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
    // Parse the statement list inside the else block
    if (check.token_type != T_RBRACE) {
        
        parseStatementList(file);
    }

    Token closeBrace = get_token(file);
    if (closeBrace.token_type != T_RBRACE) {
        fprintf(stderr, "Error: Expected '}' after else block\n");
        exit(1);
    }
}

void parseReturnStatement(FILE* file) {
    
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

    // Parse the function parameters
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

    while (statementToken.token_type != T_RBRACE) {
        
    statementToken = get_token(file);
    if (statementToken.token_type == T_EOF) {
        fprintf(stderr, "Error: Unexpected end of file within function body\n");
        exit(1);    
    }
    

    // Consume the token before passing to parseStatement or parseReturnStatement ???
    

    // Check if it's a return statement, check the return type for non-void functions
    if (strcmp(statementToken.string_value->str, "return") == 0) {
        
        hasReturnStatement = true;
        statementToken= get_token(file);

        
        DataType exprType = parse_expression(Globaltable, &statementToken ,&error, &file);
        fseek(file, -strlen(statementToken.string_value->str), SEEK_CUR);
        
       
        if (return_type != TYPE_VOID && exprType != return_type) {
            fprintf(stderr, "Error: Return type does not match function return type\n");
            exit(1);
        }
        } else {
        
        // Parse other statements
        printf("statement string: %s\n", statementToken.string_value->str);
        parseStatement(statementToken, file); 
        }
    }


    if (return_type != TYPE_VOID && !hasReturnStatement) {
        fprintf(stderr,"Error: Non-void function must have a return statement\n");
        exit(1);
    } else if (return_type == TYPE_VOID && hasReturnStatement) {
        fprintf(stderr,"Error: 'void' function should not have a return statement\n");
        exit(1);
    }

    // Expecting '}';
    if (statementToken.token_type != T_RBRACE) {
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
