#ifndef parser_h
#define parser_h








void parsing();
void parseStatement(Token token, FILE* file);
void parseStatementList(FILE* file);
void parseAssignmentStatement(FILE* file, Token variableToken);
void parseLetOrVarStatement(FILE* file, Token keywordToken);
void parseWhileStatement(FILE* file);
void parseIfStatement(FILE* file);
void parseElseBlock(FILE* file);
void parseFunction(FILE* file);
ListFuncParam* parseParamList(FILE* file);
DataType parseType(FILE* file);
ParamPrefix parsePrefixFunction(Token PrefixToken, FILE* file);






#endif