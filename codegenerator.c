#include <stdio.h>
#include <stdlib.h>

#include "codegenerator.h"

//TODO: replace string with vsprintf

void print_list(const instr_node *head) {
    const instr_node *current = head;
    int count = 0;
    
    while (current != NULL) {
        printf("Node %d: %s", count, current->instr);
        current = current->next;
        count++;
    }
}

int create_file(FILE **file) {
    *file = fopen("IFJ23.code", "w");

    if (*file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void generate_header(FILE *file) {
  fprintf(file, "# IFJ dlya liudiej bez lichnoi zhizni\n");
  fprintf(file, ".IFJcode23\n");
  fprintf(file, "JUMP $$main\n\n");
}

void add_instr(instr_node **head, char *instr) {

    // printf("instruction to add to list: %s\n", instr);

    instr_node *new_node = malloc(sizeof(instr_node));
    if (new_node == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Error allocating memory in newnode\n");
        return;
    }

    new_node->instr = instr;
    // printf("new_node->instr: %s\n", new_node->instr);
    new_node->next = NULL;

    // If the list is empty, make the new node the head
    if (*head == NULL) {

        printf("head is null\n");
        *head = new_node;
    } else {
        // Traverse the list to find the last node
        instr_node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        printf("current->instr: %s\n", current->instr);
        // Insert the new node at the end of the list
        current->next = new_node;
        printf("new_node->instr: %s\n", new_node->instr);
    }
}

// define global id, id_name must be in data.op1.id_name
void CREATE_ID(instr_node **head, char *id_name, char *string) {
  sprintf(string, "DEFVAR GF@%s\n", id_name);
  add_instr(head, string);
}

// move int value in data.op1.int_val to data.op1.id_name
void MOVE(instr_node **head, char *id_name, int value, char *string) {
  sprintf(string,"MOVE GF@%s int@%d\n", id_name, value);
  add_instr(head, string);
}

// assign value from the stack to data.op1.id_name
void ASSIGN(instr_node **head, char *id_name, char *string) {
  sprintf(string, "POPS GF@%s\n", id_name);
  add_instr(head, string);
}

// write value of data.op1.id_name to stdout
void WRITE(instr_node **head, char *id_name, char *string) {
  sprintf(string, "WRITE GF@%s\n", id_name);
  add_instr(head, string);
}

// begin expression, clear stack
void BEGIN_EXPR(instr_node **head, char *string) {
  string = "CLEARS\n";
  add_instr(head, string);
}

// push value of data.op1.id_name to stack
void PUSH(instr_node **head, char *id_name, char *string) {
  sprintf(string, "PUSHS GF@%s\n", id_name);
  add_instr(head, string);
}

// add two values from stack (top two) and push result to stack
void ADD(instr_node **head, char *string) {
  string = "ADDS\n";
  add_instr(head, string);
}

// subtract two values from stack (top two) and push result to stack
void SUB(instr_node **head, char *string) {
  string = "SUBS\n";
  add_instr(head, string);
}

// multiply two values from stack (top two) and push result to stack
void MUL(instr_node **head, char *string) {
  string = "MULS\n";
  add_instr(head, string);
}

// divide two values from stack (top two) and push result to stack
void DIV(instr_node **head, char *string) {
  string = "DIVS\n";
  add_instr(head, string);
}

void MAIN(instr_node **head, char *string) {
  string = "\n";
  add_instr(head, string);
  string = "LABEL $$main\n";
  add_instr(head, string);
  string = "DEFVAR GF@%%retval_main\n";
  add_instr(head, string);
}

void FUNC_START(instr_node **head, char *func_name, char *string) {
  sprintf(string, "LABEL $%s\n", func_name);
  add_instr(head, string);
  string = "PUSHFRAME\n";
  add_instr(head, string);
  string = "DEFVAR LF@%%retval\n";
  add_instr(head, string);
  string = "MOVE LF@%%retval nil@nil\n";
  add_instr(head, string);
}

void FUNC_CALL(instr_node **head, char *func_name, Operand *func_param, char *string) {
  // string = "CREATEFRAME\n";
  sprintf(string, "CREATEFRAME\n");
  add_instr(head, string);

  // while (func_param != NULL) {
  //   sprintf(string, "DEFVAR TF@%s\n", func_param->id_name);
  //   add_instr(head, string);
  //   sprintf(string, "MOVE TF@%s int@%d\n", func_param->id_name, func_param->int_val);
  //   add_instr(head, string);
  //   func_param++;
  // }
  for (int i = 0; i < 2; i++) {

    sprintf(string, "DEFVAR TF@%s\n", func_param[i].id_name);
    // printf("string: %s\n", string);
    add_instr(head, string);

    sprintf(string, "MOVE TF@%s int@%d\n", func_param[i].id_name, func_param[i].int_val);
    add_instr(head, string);
  }

  sprintf(string, "CALL $%s\n", func_name);
  add_instr(head, string);
}

void FUNC_END(instr_node **head, char* retval, char *string) {
  sprintf(string, "MOVE LF@%%retval LF@%s\n", retval);
  add_instr(head, string);
  string = "POPFRAME\n";
  add_instr(head, string);
  string = "RETURN\n\n";
  add_instr(head, string);
}

int generate_code(instr_node **head, Data data, gencode gencode) {
  
  // GENERATE ALL SHIT HERE :)

  char *string = malloc(256 * sizeof(char)); 
  if (string == NULL) {
      // Handle memory allocation error
      return EXIT_FAILURE;
  }

  switch (gencode) {

    char *id_name_create;
    char *id_name_assign;
    char *id_name_write;
    char *id_name_push;
    char *retval;
    char *id_name_move;
    char *func_name;
    int int_val;
    // Operand func_param[10];

    // define data.op1.id_name to global
    case GEN_CREATE_ID:
        // char *id_name_create;
        id_name_create = data.op1.id_name;
        CREATE_ID(head, id_name_create, string);
        break;
    
    // moves int value in data.op1.int_val to data.op1.id_name
    case GEN_MOVE:
        int_val = data.op1.int_val;
        id_name_move = data.op1.id_name;
        MOVE(head, id_name_move, int_val, string);
        break;
    
    // pop value from expression to data.op1.id_name
    case GEN_ASSIGN:
        // char *id_name_assign;
        id_name_assign = data.op1.id_name;
        ASSIGN(head, id_name_assign, string);
        break;
    
    // write value of data.op1.id_name
    case GEN_WRITE:
        // char *id_name_write;
        id_name_write = data.op1.id_name;
        WRITE(head, id_name_write, string);
        break;
    
    // begin expression, clear stack
    case GEN_BEGIN_EXPR:
        BEGIN_EXPR(head, string);
        break;
    
    // push value of data.op1.id_name to stack
    case GEN_PUSH:
        // char *id_name_push;
        id_name_push = data.op1.id_name;
        PUSH(head, id_name_push, string);
        break;
    
    // add two values from stack (top two) and push result to stack
    case GEN_ADD:
        ADD(head, string);
        break;

    // subtract two values from stack (top two) and push result to stack
    case GEN_SUB:
        SUB(head, string);
        break;

    // multiply two values from stack (top two) and push result to stack
    case GEN_MUL:
        MUL(head, string);
        break;

    // divide two values from stack (top two) and push result to stack
    case GEN_DIV:
        DIV(head, string);
        break;

    // generate main function
    case GEN_MAIN:
        MAIN(head, string);
        break;

    // generate function start
    case GEN_FUNC_START:
        // char *func_name;
        func_name = data.func_name;
        FUNC_START(head, func_name, string);
        break;

    // generate function end
    case GEN_FUNC_END:
        // char *retval;
        retval = data.op1.id_name;
        FUNC_END(head, retval, string);
        break;

    // generate function call
    case GEN_FUNC_CALL:
        // char *func_name;
        // Operand *func_param;
        func_name = data.func_name;
        // printf("dara.func_name: %s\n", data.func_name);
        // printf("dara.func_param1.id: %s\n", data.func_param[0].id_name);
        // printf("dara.func_param2.id: %s\n", data.func_param[1].id_name);
        // printf("dara.func_param1.int_val: %d\n", data.func_param[0].int_val);
        // printf("dara.func_param2.int_val: %d\n", data.func_param[1].int_val);
        // printf("---------------------------\n");
        // func_param = data.func_param;
        FUNC_CALL(head, func_name, data.func_param, string);
        break;

    // Add default case to handle unexpected values
    default:
        // Handle unexpected case
        break;
  }

  return EXIT_SUCCESS;
}

void pop_list_to_file(instr_node **head) {
  FILE *file = fopen("IFJ23.code", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  instr_node *current = *head;
  int cnt = 6;
  while (current != NULL) {
    // printf("instruction to print to file on line %d: %s", cnt, current->instr);
    fprintf(file, "%s", current->instr);
    cnt++;
    instr_node *tmp = current;
    current = current->next;
    // printf("instruction to print to file on line %d: %s", cnt, tmp->instr);
    // free(tmp->instr);
    tmp->instr = NULL;
    // printf("instruction to prindawdawdwat to file on line %d: %s\n", cnt, tmp->instr);
    free(tmp);
  }
  fclose(file);
}

void destroy_file() {
  remove("IFJ23.code"); 
}


// a + b
// Data data = init_data();
// data.id1_name = a
// data.id2_name = b
// generate(data, GEN_ADD);
Data init_data() {
  Data data;
  data.func_name = NULL;
  // data.func_param = NULL;
  data.op1.id_name = NULL;
  data.op2.id_name = NULL;
  return data;
}

int generate_file() {
  int status;

  destroy_file();

  FILE *file;
  status = create_file(&file);

  if (status == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  generate_header(file);
  // status = generate_code(file);
  // if (status == EXIT_FAILURE) {
    // return EXIT_FAILURE;
  // }

  fclose(file);

  return EXIT_SUCCESS;
}