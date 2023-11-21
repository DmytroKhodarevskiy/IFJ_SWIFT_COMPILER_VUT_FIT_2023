#include <stdio.h>
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256 // Adjust as needed

typedef struct instr_node {
  char *instr;
  struct instr_node *next;
} instr_node;

typedef enum {
  GEN_CREATE_ID, // id
  GEN_ADD, // +
  GEN_SUB, // -
  GEN_BEGIN_EXPR, // begin expr
  GEN_PUSH,
  GEN_ASSIGN,
  GEN_WRITE,
  GEN_MOVE
} gencode;

typedef struct Operand {
  char *id_name;
  int int_val;
  double double_val;
} Operand;

typedef struct Data {
  Operand op1;
  Operand op2;
  char *func_name;
  char *func_param[];
} Data;


