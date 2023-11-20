#ifndef parser_h
#define parser_h



typedef enum {
    PREFIX_NONE,
    PREFIX_WITH,
    PREFIX_OF,
    PREFIX_BY,
    PREFIX_UNDERSCORE
} ParamPrefix;




void parsing();
void parseStatement(Token token, FILE* file);
void parseLetOrVarStatement(FILE* file, Token keywordToken);
void parseWhileStatement(FILE* file);
void parseIfStatement(FILE* file);
void parseElseBlock(FILE* file);
void parseFunction(FILE* file);
ListFuncParam* parseParamList(FILE* file);
DataType parseType(FILE* file);
ParamPrefix parsePrefixFunction(FILE* file);






#endif