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

void convert_from_dynamic_string(char *s, char *string) {

  //printf("%s\n", s);

  //printf("%d\n", len);

  strcpy(string, s);

//   for (int i = 0; i < len; i++) {
//     string[i] = s[i];
//   }

    printf("%s ya loh\n", string);

//   dynamic_string_free(s);
}
// int main() {
//     // char* myString = createDynamicString("Hello ");

//     // if (myString != NULL) {
//     //     appendToDynamicString(&myString, "World!");
//     //     printf("%s\n", myString);

//     //     // Don't forget to free the memory when you're done
//     //     free(myString);
//     // }

//     return 0;
// }
