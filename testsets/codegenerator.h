#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "symtable.h"
#include "memory.h"
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
  struct instr_node *declarations;
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
  GEN_FUNC_CALL, // generate function call, CANT SET FRAME (USE UNUSED)
  GEN_IF_START,
  GEN_IF_CHECK,
  GEN_ELSE_START,
  GEN_IF_END,
  GEN_ELSE_IF_END,
  GEN_PUSHFRAME,
  GEN_CREATEFRAME,
  GEN_POPFRAME,
  GEN_EXIT,
  GEN_POP_TMP,
  GEN_BUILTIN,
    GEN_PUSH_TMP,
    GEN_IDIV,
    GEN_LT,
    GEN_GT,
    GEN_EQ,
    GEN_AND,
    GEN_OR,
    GEN_NOT,
    GEN_INT2FLOAT,
    GEN_FLOAT2INT,
    GEN_INT2CHAR,
    GEN_STRI2INT,
    GEN_READ,
    GEN_CONCAT,
    GEN_WHILE_START,
    GEN_WHILE_CHECK,
    GEN_WHILE_END,
    GEN_CALL,
    GEN_RETURN,
    GEN_STRLEN,
} gencode;

typedef struct Operand {
  char *id_name;
  char *val;
  DataType type;
} Operand;

typedef struct Data {
  Operand op;
  Operand op2;
  char *func_name;
  Operand func_param;
  unsigned int func_param_count;
  int ifelse_cnt;
  int while_cnt;
  // int else_cnt;
} Data;

instr_list_dynamic *init_instr_list_dynamic();
// int add_new_linked_list(instr_list_dynamic *list);
int add_new_linked_list(instr_list_dynamic *list, char *name);
// instr_node *search_by_name_in_list(instr_list_dynamic *list, const char *name, instr_node *main_node);
instr_node *search_by_name_in_list(instr_list_dynamic *list, char *name, instr_node *main_node);
void print_list_names(instr_list_dynamic *list);
void pop_all_lists_to_file(instr_list_dynamic *list);
char *type_to_string(DataType type);
// bool is_instr_in_list(instr_node *head, const char *instr);
bool is_substr_in_list(instr_node *head, const char *substr);

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
// int generate_code(instr_node **head, Data data, gencode gencode, Frame frame);
int generate_code(instr_node **head, Data data, gencode gencode, int deepness, Frame frame);

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
// void generate_header(FILE *file);
void generate_header();

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

