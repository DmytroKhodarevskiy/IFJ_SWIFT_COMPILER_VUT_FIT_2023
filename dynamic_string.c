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
    free(dynamicString); // Free the Dynamic_string struct if the string allocation fails.
    fprintf(stderr, "Memory allocation for string failed.\n");
    return NULL;
  }

  dynamicString->str[0] = '\0'; // Initialize the string with a null terminator.
  dynamicString->length = 0;
  dynamicString->allocSize = INITIAL_SIZE;

  return dynamicString;
}

void appendToDynamicString(Dynamic_string* dynamicString, char content) {
  if (dynamicString == NULL) {
    printf("Invalid Dynamic_string pointer.\n");
    return;
  }

  // Check if more memory is needed
  if (dynamicString->length + 1 >= dynamicString->allocSize) {
    // If more memory is needed, reallocate
    unsigned int newAllocSize = (dynamicString->allocSize * 2); // You can adjust the resizing strategy as needed.
    char* newStr = (char*)realloc(dynamicString->str, newAllocSize);

    if (newStr == NULL) {
      printf("Memory reallocation failed.\n");
      return;
    }

    dynamicString->str = newStr;
    dynamicString->allocSize = newAllocSize;
  }

  // Append the character to the string
  dynamicString->str[dynamicString->length] = content;
  dynamicString->length++;

  // Null-terminate the string
  dynamicString->str[dynamicString->length] = '\0';
}

void dynamic_string_free(Dynamic_string* dynamicString) {
  if (dynamicString != NULL) {
    if (dynamicString->str != NULL) {
      free(dynamicString->str); // Free the character array.
    }
    free(dynamicString); // Free the Dynamic_string structure.
  }
}