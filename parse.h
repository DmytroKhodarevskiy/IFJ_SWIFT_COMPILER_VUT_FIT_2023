#include <stdio.h>
#include <stdlib.h>

// void GLOBAL_STATEMENT_LIST();
void GLOBAL_STATEMENT(FILE *file); //for every function
void DECLARE_GLOBAL_FUNC(FILE *file);
void STMT_LIST(FILE *file);
void STMT(FILE *file);
void IF_EXP(FILE *file);
void EXP(FILE *file);
void PROGRAM(FILE *file);
void MB_STMT_LET_VAR(FILE *file);
void TYPE(FILE *file);
void MB_ASSIGN_EXPR(FILE *file);