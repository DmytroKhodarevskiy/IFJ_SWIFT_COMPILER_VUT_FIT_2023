#include <stdio.h>

#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

typedef struct RULE {
  char *left_side;
  char *right_side;
} Rule;

typedef enum{
  S, // shift
  R, // reduce
  EQ, // equal
  E, // error
  END, // END
} Action_Letter;




#endif // _EXPR_PARSER_H
