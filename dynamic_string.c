
// #include "memory.h"
#include "dynamic_string.h"

#define INITIAL_SIZE 10

Dynamic_string* createDynamicString() {
    Dynamic_string* dynamicString = (Dynamic_string*)malloc(sizeof(Dynamic_string));
    // Dynamic_string* dynamicString = (Dynamic_string*)safe_MemoryBlock(sizeof(Dynamic_string));

    // printf("DYNAMIC STRING POINTER: %p\n", dynamicString);

    if (dynamicString == NULL) {
        exitWithError("Memory allocation failed.\n", ERR_INTERNAL);
        // printf("Memory allocation failed.\n");
        return NULL;
    }

    // dynamicString->str = (char*)safe_MemoryBlock(INITIAL_SIZE);
    dynamicString->str = (char*)malloc(INITIAL_SIZE);
    
    // printf("DYNAMIC STRING STRING POINTER: %p\n", dynamicString->str);

    if (dynamicString->str == NULL) {
        free(dynamicString);
        exitWithError("Memory allocation for string failed.\n", ERR_INTERNAL);
        // fprintf(stderr, "Memory allocation for string failed.\n");
        return NULL;
    }

    dynamicString->str[0] = '\0'; 
    dynamicString->length = 0;
    dynamicString->allocSize = INITIAL_SIZE;

    return dynamicString;
}


void appendToDynamicString(Dynamic_string* dynamicString, char content) {
    if (dynamicString == NULL) {
        // printf("Invalid Dynamic_string pointer.\n");
        exitWithError("Invalid Dynamic_string pointer.\n", ERR_INTERNAL);
        return;
    }

    if (dynamicString->length + 1 >= dynamicString->allocSize) {
    
        unsigned int newAllocSize = (dynamicString->allocSize * 2); 
        char* newStr = (char*)realloc(dynamicString->str, newAllocSize);

        // char* newStr = resizeMemoryBlock(dynamicString->str, newAllocSize);
        // printf("POINTER: %p\n", dynamicString->str);
        // printMemoryList();
        // char *newStr = (char*)resizeMemoryBlock(dynamicString->str, newAllocSize);
        // char *newStr = (char*)resizeMemoryBlock(dynamicString->str, newAllocSize);
        // printf("RESIZED POINTER: %p\n", newStr);
        // dynamicString->str = newStr;

        if (newStr == NULL) {
            exitWithError("Memory reallocation failed.\n", ERR_INTERNAL);
            // printf("Memory reallocation failed.\n");
            return;
        }

        dynamicString->str = newStr;
        dynamicString->allocSize = newAllocSize;
    }

    dynamicString->str[dynamicString->length] = content;
    dynamicString->length++;


    dynamicString->str[dynamicString->length] = '\0';
}




void deleteLastCharacter(Dynamic_string* dynamicString) {
    if (dynamicString == NULL || dynamicString->length == 0) {
        return;
    }

    dynamicString->str[dynamicString->length - 1] = '\0'; 
    dynamicString->length--;
}





void dynamic_string_free(Dynamic_string* dynamicString) {
    if (dynamicString != NULL) {
        if (dynamicString->str != NULL) {
            free(dynamicString->str);
        }
        free(dynamicString); 
    }
}
