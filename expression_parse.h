#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include <stdio.h>
#include "stack.h"

typedef struct RULE {
  token_type left_side;
  token_type right_side[4];
} Rule;

typedef enum{
  S, // shift
  R, // reduce
  EQ, // equal
  E, // error
  END, // END
} Action_Letter;

void perform_reduce(TokenStack *stack, int *error);



#endif // _EXPR_PARSER_H
