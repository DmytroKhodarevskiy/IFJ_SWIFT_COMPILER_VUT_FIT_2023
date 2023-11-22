#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>

#define ERR_NO                 0
#define ERR_LEX                1 
#define ERR_SYNTAX             2 
#define ERR_SEMANT_FUNC_ARG    3
#define ERR_SEMANT_PARAM       4 
#define ERR_SEMANT_UNDF_VALUE  5
#define ERR_SEMANT_RETURN      6
#define ERR_SEMANT_TYPE        7
#define ERR_SEMANT_UNDF_TYPE   8
#define ERR_SEMANT_OTHER       9
#define ERR_INTERNAL           99
//#define DEBUG_MSG       1

typedef struct MemoryBlockNode {
    void *block;  // Pointer to the allocated memory block
    struct MemoryBlockNode *next;  // Pointer to the next node in the list
} MemoryBlockNode;

MemoryBlockNode *memoryListHead = NULL;

extern int linenum;
extern int prgPass;

void *safe_MemoryBlock(size_t size);
void free_all();
void exitWithError(char* messsage, int ErrCode);
int printDebugMessage(const char* message, ...);
int printDebugMessage2(const char* message, ...);

#endif /* memory_h */