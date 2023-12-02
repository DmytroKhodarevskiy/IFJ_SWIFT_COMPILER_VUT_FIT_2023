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

Dynamic_string* createDynamicString();
void appendToDynamicString(Dynamic_string* dynamicString, char content);
void deleteLastCharacter(Dynamic_string* dynamicString);
void dynamic_string_free(Dynamic_string* dynamicString);

#endif