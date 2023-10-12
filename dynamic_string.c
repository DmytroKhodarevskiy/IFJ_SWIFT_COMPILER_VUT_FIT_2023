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

void appendToDynamicString(char** dynamicString, char content) {
    unsigned int currentLength = strlen(*dynamicString);

    // Check if more memory is needed
    if (currentLength + 1 >= strlen(*dynamicString)) {
        // If more memory is needed, reallocate
        *dynamicString = (char*)realloc(*dynamicString, (strlen(*dynamicString) + 1 + 1) * sizeof(char));

        if (*dynamicString == NULL) {
            printf("Memory reallocation failed.\n");
            return;
        }
    }

    // strcat(*dynamicString, content);
    (*dynamicString)[currentLength] = content;
    
    (*dynamicString)[currentLength + 1] = '\0';

}

void dynamic_string_free(char *s) {
  free(s);
}

char* convert_from_dynamic_string(char* dynamic_string) {
  char* string;
  strcpy(string, dynamic_string);
  return string;
}