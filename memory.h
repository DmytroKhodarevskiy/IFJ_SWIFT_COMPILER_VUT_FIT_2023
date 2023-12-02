#include <stdio.h>

#ifndef _MEMORY_H
#define _MEMORY_H


#define ERR_OK                 0
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

// MemoryBlockNode *memoryListHead = NULL;
// #define MemoryBlockNode *memoryListHead = NULL;



extern int linenum;
/**
 * @brief Initializes the memory management linked list.
 *
 * This function sets the head of the memory management linked list to NULL,
 * indicating an empty list at initialization.
 */
void init_memory_list();

/**
 * @brief Safely reallocates memory and handles allocation errors.
 *
 * This function attempts to reallocate a memory block to a new size. If the
 * reallocation fails and the new size is not zero, it exits with an error message.
 *
 * @param ptr Pointer to the memory block to be reallocated.
 * @param newSize The new size for the memory block.
 * @return void* Pointer to the reallocated memory block.
 */
void *safeRealloc(void *ptr, size_t newSize);

/**
 * @brief Prints the current state of the memory management list.
 *
 * This function iterates through the memory management linked list and prints
 * the address of each node and its associated memory block.
 */
void printMemoryList();

/**
 * @brief Resizes a memory block managed by the memory list.
 *
 * This function looks for a memory block in the linked list and resizes it.
 * It updates the block's address in the list if needed and handles block not
 * found or reallocation errors by exiting with an error message.
 *
 * @param block Pointer to the memory block to be resized.
 * @param newSize The new size for the memory block.
 * @return void* Pointer to the resized memory block.
 */
void* resizeMemoryBlock(void *block, size_t newSize);


/**
 * @brief Safely allocates a memory block and adds it to the memory list.
 *
 * This function allocates a new memory block and a corresponding node in the
 * memory management list. It handles allocation errors by exiting with an error
 * message.
 *
 * @param size The size of the memory block to be allocated.
 * @return void* Pointer to the newly allocated memory block.
 */
void *safe_MemoryBlock(size_t size);


/**
 * @brief Frees all memory blocks managed by the memory list.
 *
 * This function iterates through the memory management linked list, freeing
 * each memory block and its corresponding node. It resets the head of the list
 * to NULL after completion.
 */
void free_all();

/**
 * @brief Exits the program with an error message for specific error codes.
 *
 * This function prints an error message specific to various error codes
 * and exits the program. It uses ANSI escape codes for formatted output.
 *
 * @param message The error message to be printed.
 * @param ErrCode The error code determining the type of error.
 */
void exitWithError(char* message, int ErrCode);
// int printDebugMessage(const char* message, ...);
// int printDebugMessage2(const char* message, ...);

#endif /* memory_h */