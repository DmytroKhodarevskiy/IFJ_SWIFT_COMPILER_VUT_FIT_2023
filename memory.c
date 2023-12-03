

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#include "memory.h"

MemoryBlockNode *memoryListHead;


void init_memory_list() {
    memoryListHead = NULL;
}

void *safeRealloc(void *ptr, size_t newSize) {
    void *newBlock = realloc(ptr, newSize);
    if (newBlock == NULL && newSize != 0) {
        // Reallocation failed
        exitWithError("Memory reallocation failed", ERR_INTERNAL);
    }
    return newBlock;
}

void printMemoryList() {
    printf("Memory listadwd:\n");
    MemoryBlockNode *current = memoryListHead;
    printf("Memory list:\n");
    while (current != NULL) {
        printf("Node: %p\n", current);
        printf("Memory block: %p\n", current->block);
        current = current->next;
    }
}

void* resizeMemoryBlock(void *block, size_t newSize) {
    MemoryBlockNode *current = memoryListHead;
    // while (current != NULL) {
    //     if (current->block == block) {
    //         // Found the block in the linked list
    //         // printf("Resizing memory block: %p\n", current->block);
    //         current->block = safeRealloc(current->block, newSize);
    //         // current. = newSize; // Update the size
    //         printf("Resized memory block: %p\n", current->block);
    //         return current->block;
    //     }
    //     current = current->next;
    // }

    while (current != NULL) {
        if (current->block == block) {
            // Found the block in the linked list
            void *newBlock = safeRealloc(current->block, newSize); // Resize the block
            if (newBlock != current->block) {
                // Update the block address in the linked list
                current->block = newBlock;
            }
            printf("Resized memory block: %p\n", current->block);
            return newBlock;
        }
        current = current->next;
    }

    // If the block was not found in the linked list
    exitWithError("Block not found in memory list", ERR_INTERNAL);
}


void *safe_MemoryBlock(size_t size) {
    void *block = malloc(size);
    if (block == NULL) {

        exitWithError("Memory allocation failed", ERR_INTERNAL);
    }

    MemoryBlockNode *node = (MemoryBlockNode *)malloc(sizeof(MemoryBlockNode));
    if (node == NULL) {

        exitWithError("Memory allocation failed", ERR_INTERNAL);
    }

    node->block = block;
    node->next = memoryListHead;
    memoryListHead = node;

    return block;
}

void free_all() {
    MemoryBlockNode *current = memoryListHead;
    while (current != NULL) {
        // printf("Freeing memory block: %p\n", current->block);
        // MemoryBlockNode *next = current->next;
        // printf("Freeing node: %p\n", current);
        // free(current->block);  
        // free(current);         
        // current = next;

        printf("Freeing memory block: %p\n", current->block);
        MemoryBlockNode *next = current->next;
        free(current->block);
        printf("Freeing node: %p\n", current);
        free(current);
        current = next;
    }
    memoryListHead = NULL;
}

void exitWithError(char* masssage, int ErrCode) {
    switch (ErrCode)
    {
    case ERR_LEX:
        fprintf(stderr,"\033[1m\033[31mError in the program within the lexical analysis (incorrect structure of the current lexeme) line:\033[0m \033[1m\033[33m%d\n",linenum);
        break;
    case ERR_SYNTAX:
        fprintf(stderr,"\033[1m\033[31mError in the parsing program line:\033[0m \033[1m\033[33m%d\n",linenum);
        break;
    case ERR_SEMANT_FUNC_ARG:
        fprintf(stderr,"\033[1m\033[31mSemantic error in the program - undefined function, variable redefinition line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_PARAM:
        fprintf(stderr,"\033[1m\033[31mWrong number/type of parameters when calling the function or wrong type of return value from function line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_UNDF_VALUE:
        fprintf(stderr,"\033[1m\033[31mSemantic error in program - use of undefined or uninitialized for variable line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_RETURN:
        fprintf(stderr,"\033[1m\033[31mMissing/overrun expression in the return statement from the function line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_TYPE:
        fprintf(stderr,"\033[1m\033[31mType compatibility semantic error in arithmetic, string, and relational expressions line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_UNDF_TYPE:
        fprintf(stderr,"\033[1m\033[31mThe variable or parameter type is not specified and cannot be inferred from the expression used line:\033[0m %d\n",linenum);
        break;
    case ERR_SEMANT_OTHER:
        fprintf(stderr,"\033[1m\033[31mOther semantic errors:\033[0m %d\n",linenum);
        break;
    case ERR_INTERNAL:
        fprintf(stderr,"\033[1m\033[31mInternal compiler error, i.e. not affected by the input program line:\033[0m %d\n",linenum);
        break;
    }
    fprintf(stderr, "\033[35m\033[1m%s\n\033[0m", masssage);
    // free_all();
    exit(ErrCode);
}

// int printDebugMessage(const char* messsage, ...) {
//     #ifdef DEBUG_MSG
//     va_list args;
//     va_start(args, messsage);
//     int ret = vfprintf(stderr, messsage, args);
//     va_end(args);
//     return ret;
//     #else
//     return 0;
//     #endif
// }

// int printDebugMessage2(const char* messsage, ...) {
//     #ifdef DEBUG_MSG
//     va_list args;
//     va_start(args, messsage);
//     int ret = vfprintf(stderr, messsage, args);
//     va_end(args);
//     return ret;
//     #else
//     return 0;
//     #endif
// }