

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#include "memory.h"

void initializeDynamicArray() {
    allocatedMemoryArray.capacity = 10; 
    allocatedMemoryArray.size = 0;
    allocatedMemoryArray.data = (void**) malloc(allocatedMemoryArray.capacity * sizeof(void*));
    if (allocatedMemoryArray.data == NULL) {
        exitWithError("Failed to initialize memory array", MST_INTERNAL);
    }
}

void resizeDynamicArray() {
    allocatedMemoryArray.capacity *= 2;
    allocatedMemoryArray.data = (void**) realloc(allocatedMemoryArray.data, allocatedMemoryArray.capacity * sizeof(void*));
    if (allocatedMemoryArray.data == NULL) {
        exitWithError("Failed to resize memory array", MST_INTERNAL);
    }
}

void addToDynamicArray(void* ptr) {
    if (allocatedMemoryArray.size == allocatedMemoryArray.capacity) {
        resizeDynamicArray();
    }
    allocatedMemoryArray.data[allocatedMemoryArray.size++] = ptr;
}

void removeFromDynamicArray(void* ptr) {
    for (size_t i = 0; i < allocatedMemoryArray.size; i++) {
        if (allocatedMemoryArray.data[i] == ptr) {
            allocatedMemoryArray.data[i] = allocatedMemoryArray.data[--allocatedMemoryArray.size];
            break;
        }
    }
}

void deallocateAllMemoryDynamicArray() {
    for (size_t i = 0; i < allocatedMemoryArray.size; i++) {
        free(allocatedMemoryArray.data[i]);
    }
    free(allocatedMemoryArray.data);
    allocatedMemoryArray.size = 0;
    allocatedMemoryArray.capacity = 0;
}


void *safe_MemoryBlock(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        exitWithError("Failed to allocate memory", MST_INTERNAL);
    }
    addToDynamicArray(ptr);
    return ptr;
}

void free_MemoryBlock(void* ptr) {
    if (ptr != NULL) {
        removeFromDynamicArray(ptr);
        free(ptr);
    }
}

void free_all() {
    deallocateAllMemoryDynamicArray();
}

void exitWithError(char* msg, int errCode) {
    fprintf(stderr, "Error: %s\n", msg);
    free_all();
    exit(errCode);
}

int printDebugMessage(const char* fmt, ...) {
    #ifdef DEBUG_MSG
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(stderr, fmt, args);
    va_end(args);
    return ret;
    #else
    return 0;
    #endif
}

int printDebugMessage2(const char* fmt, ...) {
    #ifdef DEBUG_MSG
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(stderr, fmt, args);
    va_end(args);
    return ret;
    #else
    return 0;
    #endif
}