#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

typedef struct {
    char* str;
    unsigned int length;
    unsigned int allocSize;
} Dynamic_string;

/**
 * @brief Creates and initializes a dynamic string.
 *
 * This function allocates memory for a new dynamic string and its underlying
 * character array. It initializes the string's length and allocated size. In
 * case of memory allocation failure, it exits with an error message.
 *
 * @return Dynamic_string* Pointer to the newly created dynamic string.
 */
Dynamic_string* createDynamicString();

/**
 * @brief Appends a character to a dynamic string.
 *
 * This function adds a single character to the end of a dynamic string. If the
 * dynamic string's buffer is full, it reallocates memory to expand the buffer.
 * In case of an invalid string pointer or memory reallocation failure, it exits
 * with an error message.
 *
 * @param dynamicString Pointer to the dynamic string to be modified.
 * @param content The character to append to the string.
 */
void appendToDynamicString(Dynamic_string* dynamicString, char content);


/**
 * @brief Deletes the last character from a dynamic string.
 *
 * This function removes the last character from a dynamic string. If the string
 * is empty or the pointer is NULL, the function does nothing.
 *
 * @param dynamicString Pointer to the dynamic string to be modified.
 */
void deleteLastCharacter(Dynamic_string* dynamicString);

/**
 * @brief Frees the memory allocated for a dynamic string.
 *
 * This function deallocates the memory used by a dynamic string's character
 * array and the dynamic string itself. If the pointer to the dynamic string
 * is NULL, the function does nothing.
 *
 * @param dynamicString Pointer to the dynamic string to be freed.
 */
void dynamic_string_free(Dynamic_string* dynamicString);

#endif