#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include "symtable_stack.h"
#include "expression_parse.h"
#include "memory.h"

#ifndef PARSE_H
#define PARSE_H

void Parse(FILE *file);
// void GLOBAL_STATEMENT_LIST();
void GLOBAL_STATEMENT(FILE *file); //for every function
void DECLARE_GLOBAL_FUNC(FILE *file);
void STMT_LIST(FILE *file);
void STMT(FILE *file);
void IF_EXP(FILE *file);
void EXP(FILE *file);
void PROGRAM(FILE *file);
// void MB_STMT_LET_VAR(FILE *file);
void MB_STMT_LET_VAR(FILE *file, bool changeable);
// void TYPE(FILE *file);
void TYPE(FILE *file, DataType *type);
// void MB_ASSIGN_EXPR(FILE *file);
void MB_ASSIGN_EXPR(FILE *file, DataType type);
void WHILE_EXP(FILE *file);
void PARAM_LIST(FILE *file);
void PARAM(FILE *file);
void PARAM_PREFIX(FILE *file);
void PARAM_NAME(FILE *file);
void RETURN_TYPE(FILE *file);
void ASSIGN_STMT_OR_FUNCALL(FILE *file);
void ARG_LIST(FILE *file);
void FUNC_CALLS(FILE *file);
void ARG(FILE *file);
void PREFIX(FILE *file);

void PHASE_FIRST(FILE *file);
bool is_compatible(DataType assignType, DataType variableType);
DataType get_type(char *str);
void FILL_TREES(FILE *file, SymStack *stack); 

//apwdawpdawdmpaw
void PARAM_LIST_FIRST(FILE *file, ListFuncParam **params, int *param_cnt);
//wadawdawdawdawd
void PARAM_FIRST(FILE *file, ListFuncParam **params);
void PHASE_SECOND(FILE *file);

void ARG_WRITE_LIST(FILE *file);
void ARG_WRITE(FILE *file);
void WRITE_CALLS(FILE *file);



#endif // PARSE_H