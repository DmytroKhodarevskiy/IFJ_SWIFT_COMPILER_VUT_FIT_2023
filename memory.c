

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#include "memory.h"




void *safe_MemoryBlock(size_t size) {
    
}

void free_MemoryBlock(void* current) {
    
}

void free_all() {
    
}

void exitWithError(char* masssage, int ErrCode) {
    switch (ErrCode)
    {
    case ERR_NO:
        fprintf(stderr,"There aren't any errors");
        break;
    case ERR_LEX:
        fprintf(stderr,"Error in the program within the lexical analysis (incorrect structure of the current lexeme) line: %d",linenum);
        break;
    case ERR_SYNTAX:
        fprintf(stderr,"Error in the parsing program line: %d",linenum);
        break;
    case ERR_SEMANT_FUNC_ARG:
        fprintf(stderr,"Semantic error in the program - undefined function, variable redefinition line: %d",linenum);
        break;
    case ERR_SEMANT_PARAM:
        fprintf(stderr,"Wrong number/type of parameters when calling the function or wrong type of return value from function line: %d",linenum);
        break;
    case ERR_SEMANT_UNDF_VALUE:
        fprintf(stderr,"Semantic error in program - use of undefined or uninitialized for variable line: %d",linenum);
        break;
    case ERR_SEMANT_RETURN:
        fprintf(stderr,"Missing/overrun expression in the return statement from the function line: %d",linenum);
        break;
    case ERR_SEMANT_TYPE:
        fprintf(stderr,"Type compatibility semantic error in arithmetic, string, and relational expressions line: %d",linenum);
        break;
    case ERR_SEMANT_UNDF_TYPE:
        fprintf(stderr,"The variable or parameter type is not specified and cannot be inferred from the expression used line: %d",linenum);
        break;
    case ERR_SEMANT_OTHER:
        fprintf(stderr,"Other semantic errors: %d",linenum);
        break;
    case ERR_INTERNAL:
        fprintf(stderr,"Internal compiler error, i.e. not affected by the input program line: %d",linenum);
        break;
    }
    fprintf(stderr, "%s\n", masssage);
    free_all();
    exit(ErrCode);
}

int printDebugMessage(const char* messsage, ...) {
    #ifdef DEBUG_MSG
    va_list args;
    va_start(args, messsage);
    int ret = vfprintf(stderr, messsage, args);
    va_end(args);
    return ret;
    #else
    return 0;
    #endif
}

int printDebugMessage2(const char* messsage, ...) {
    #ifdef DEBUG_MSG
    va_list args;
    va_start(args, messsage);
    int ret = vfprintf(stderr, messsage, args);
    va_end(args);
    return ret;
    #else
    return 0;
    #endif
}