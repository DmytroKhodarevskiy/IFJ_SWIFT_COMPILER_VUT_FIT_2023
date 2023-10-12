#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10

char* createDynamicString() {
    int bufferSize = INITIAL_SIZE;
    char* dynamicString = (char*)malloc(bufferSize * sizeof(char));

    if (dynamicString == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    return dynamicString;
}

void appendToDynamicString(char** dynamicString, const char* content) {
    int currentLength = strlen(*dynamicString);

    // Check if more memory is needed
    if (currentLength + strlen(content) >= strlen(*dynamicString)) {
        // If more memory is needed, reallocate
        *dynamicString = (char*)realloc(*dynamicString, (strlen(*dynamicString) + strlen(content) + 1) * sizeof(char));

        if (*dynamicString == NULL) {
            printf("Memory reallocation failed.\n");
            return;
        }
    }

    strcat(*dynamicString, content);
}

int main() {
    char* myString = createDynamicString("Hello ");

    if (myString != NULL) {
        appendToDynamicString(&myString, "World!");
        printf("%s\n", myString);

        // Don't forget to free the memory when you're done
        free(myString);
    }

    return 0;
}
