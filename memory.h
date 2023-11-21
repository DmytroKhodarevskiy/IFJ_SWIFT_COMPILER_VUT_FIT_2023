#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>

#define MST_NO                 0
#define MST_LEX                1 
#define MST_SYNTAX             2 
#define MST_SEMANT_FUNC_ARG    3
#define MST_SEMANT_PARAM       4 
#define MST_SEMANT_UNDF_VALUE  5
#define MST_SEMANT_RETURN      6
#define MST_SEMANT_TYPE        7
#define MST_SEMANT_UNDF_TYPE   8
#define MST_SEMANT_OTHER       9
#define MST_INTERNAL           99

extern int srcLine;
extern int prgPass;

typedef struct {
    void** data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray allocatedMemoryArray;


void initializeDynamicArray();
void resizeDynamicArray();
void addToDynamicArray(void* ptr);
void removeFromDynamicArray(void* ptr);
void deallocateAllMemoryDynamicArray();
void *safe_MemoryBlock(size_t size);
void free_MemoryBlock(void* ptr);
void free_all();
void exitWithError(char* msg, int errCode);
int printDebugMessage(const char* fmt, ...);
int printDebugMessage2(const char* fmt, ...);

#endif /* memory_h */