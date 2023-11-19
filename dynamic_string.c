#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10

Dynamic_string* createDynamicString() {
    Dynamic_string* dynamicString = (Dynamic_string*)malloc(sizeof(Dynamic_string));

    if (dynamicString == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    dynamicString->str = (char*)malloc(INITIAL_SIZE);
    
    if (dynamicString->str == NULL) {
        free(dynamicString);
        fprintf(stderr, "Memory allocation for string failed.\n");
        return NULL;
    }

    dynamicString->str[0] = '\0'; 
    dynamicString->length = 0;
    dynamicString->allocSize = INITIAL_SIZE;

    return dynamicString;
}


void appendToDynamicString(Dynamic_string* dynamicString, char content) {
    if (dynamicString == NULL) {
        printf("Invalid Dynamic_string pointer.\n");
        return;
    }

    if (dynamicString->length + 1 >= dynamicString->allocSize) {
    
        unsigned int newAllocSize = (dynamicString->allocSize * 2); 
        char* newStr = (char*)realloc(dynamicString->str, newAllocSize);

        if (newStr == NULL) {
            printf("Memory reallocation failed.\n");
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
