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
ListFuncParam* parseParamList(FILE* file);
DataType parseType(FILE* file);
ParamPrefix parsePrefixFunction(FILE* file);






#endif