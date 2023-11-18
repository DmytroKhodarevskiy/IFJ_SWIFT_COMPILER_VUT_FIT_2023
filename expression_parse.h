#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include <stdio.h>
#include "stack.h"

typedef struct {
  token_type left_side;
  token_type right_side[4]; // Assuming the maximum size of right_side array is 4
} Rule;

typedef enum{
  S, // shift
  R, // reduce
  EQ, // equal
  E, // error
  END, // END
} Action_Letter;

int perform_reduce(TokenStack *stack, int count);




#endif // _EXPR_PARSER_H
