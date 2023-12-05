#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256 // Adjust as needed

typedef enum {
  GF,
  LF,
  TF,
  UNUSED
} Frame;

typedef struct instr_node {
  char *name_of_llist;
  char *instr;
  struct instr_node *next;
} instr_node;

typedef struct {
    instr_node **lists; // pointer to an array of linked list heads
    int size;           // current size of the array
} instr_list_dynamic;

typedef enum {
  GEN_CREATE_ID, // define global id, id_name must be in data.op1.id_name, YOU CAN SET FRAME
  GEN_ADD, // add two values from stack (top two) and push result to stack, CANT SET FRAME (USE UNUSED)
  GEN_SUB, // subtract two values from stack (top two) and push result to stack, CANT SET FRAME (USE UNUSED)
  GEN_MUL, // multiply two values from stack (top two) and push result to stack, CANT SET FRAME (USE UNUSED)
  GEN_DIV, // divide two values from stack (top two) and push result to stack, CANT SET FRAME (USE UNUSED)
  GEN_BEGIN_EXPR, // begin expression, clear stack, CANT SET FRAME (USE UNUSED)
  GEN_PUSH, // push to stack, YOU CAN SET FRAME
  GEN_ASSIGN, // assign value from the stack to data.op1.id_name, YOU CAN SET FRAME
  GEN_WRITE, // write value of data.op1.id_name to stdout, YOU CAN SET FRAME
  GEN_MOVE, // move int value in data.op1.int_val to data.op1.id_name, YOU CAN SET FRAME
  GEN_MAIN, // generate main function, CANT SET FRAME (USE UNUSED)
  GEN_FUNC_START, // generate function start, CANT SET FRAME (USE UNUSED)
  GEN_FUNC_END, // generate function end, CANT SET FRAME (USE UNUSED)
  GEN_FUNC_CALL // generate function call, CANT SET FRAME (USE UNUSED)
} gencode;

typedef struct Operand {
  char *id_name;
  char *val;
  token_type type;
} Operand;

typedef struct Data {
  Operand op;
  Operand op2;
  char *func_name;
  Operand *func_param;
  unsigned int func_param_count;
} Data;

instr_list_dynamic *init_instr_list_dynamic();
// int add_new_linked_list(instr_list_dynamic *list);
int add_new_linked_list(instr_list_dynamic *list, char *name);
instr_node *search_by_name_in_list(instr_list_dynamic *list, const char *name);
void print_list_names(instr_list_dynamic *list);
void pop_all_lists_to_file(instr_list_dynamic *list);

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
// int generate_code(instr_node **head, Data data, gencode gencode);
int generate_code(instr_node **head, Data data, gencode gencode, Frame frame);

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

