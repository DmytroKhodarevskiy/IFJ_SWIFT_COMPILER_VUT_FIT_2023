#include <stdio.h>
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256 // Adjust as needed

typedef struct instr_node {
  char *instr;
  struct instr_node *next;
} instr_node;

typedef enum {
  GEN_CREATE_ID, // define global id, id_name must be in data.op1.id_name
  GEN_ADD, // add two values from stack (top two) and push result to stack
  GEN_SUB, // subtract two values from stack (top two) and push result to stack
  GEN_MUL, // multiply two values from stack (top two) and push result to stack
  GEN_DIV, // divide two values from stack (top two) and push result to stack
  GEN_BEGIN_EXPR, // begin expression, clear stack
  GEN_PUSH, // push to stack
  GEN_ASSIGN, // assign value from the stack to data.op1.id_name
  GEN_WRITE, // write value of data.op1.id_name to stdout
  GEN_MOVE, // move int value in data.op1.int_val to data.op1.id_name
  GEN_MAIN, // generate main function
  GEN_FUNC_START, // generate function start
  GEN_FUNC_END, // generate function end
  GEN_FUNC_CALL // generate function call
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
  Operand *func_param;
} Data;

/*
* Creates an assembly file.
*/
int create_file(FILE **file);

/*
* Adds code line to the linked list.
*/
void add_instr(instr_node **head, char *instr);

/*
* Prints the linked list to the file.
*/
void pop_list_to_file(instr_node **head);

/*
* Generates code based on the given parameters.
*/
int generate_code(instr_node **head, Data data, gencode gencode);

/*
* Initializes the data structure.
*/
Data init_data();

/*
* Initializes the operand structure.
*/
Operand init_operand();

/*
* Initializes the instruction node structure.
*/
instr_node *init_instr_node();

/*
* deletes the asm file 
*/
void destroy_file();

/*
* Generates the header of the assembly file.
*/
void generate_header(FILE *file);

/*
* Generates the main function of the assembly file.
*/
void generate_main(FILE *file);

/*
* Generates the code for the given function.
*/
void generate_function(FILE *file, char *func_name);

/*
* generates the new instrucitons file with header and main
*/
int generate_file();

