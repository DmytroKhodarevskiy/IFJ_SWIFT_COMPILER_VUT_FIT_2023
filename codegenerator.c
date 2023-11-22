#include <stdio.h>
#include <stdlib.h>

#include "codegenerator.h"

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
    instr_node *new_node = malloc(sizeof(instr_node));
    if (new_node == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Error allocating memory in newnode\n");
        return;
    }

    new_node->instr = instr;
    new_node->next = NULL;

    // If the list is empty, make the new node the head
    if (*head == NULL) {
        *head = new_node;
    } else {
        // Traverse the list to find the last node
        instr_node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Insert the new node at the end of the list
        current->next = new_node;
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
  string = "CREATEFRAME\n";
  add_instr(head, string);
  string = "PUSHFRAME\n";
  add_instr(head, string);
  string = "DEFVAR LF@%%retval_main\n";
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

    // define data.op1.id_name to global
    case GEN_CREATE_ID:
        char *id_name_create = data.op1.id_name;
        CREATE_ID(head, id_name_create, string);
        break;
    
    // moves int value in data.op1.int_val to data.op1.id_name
    case GEN_MOVE:
        int int_val = data.op1.int_val;
        char *id_name_move = data.op1.id_name;
        MOVE(head, id_name_move, int_val, string);
        break;
    
    // pop value from expression to data.op1.id_name
    case GEN_ASSIGN:
        char *id_name_assign = data.op1.id_name;
        ASSIGN(head, id_name_assign, string);
        break;
    
    // write value of data.op1.id_name
    case GEN_WRITE:
        char *id_name_write = data.op1.id_name;
        WRITE(head, id_name_write, string);
        break;
    
    // begin expression, clear stack
    case GEN_BEGIN_EXPR:
        BEGIN_EXPR(head, string);
        break;
    
    // push value of data.op1.id_name to stack
    case GEN_PUSH:
        char *id_name_push = data.op1.id_name;
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
        char *func_name = data.func_name;
        FUNC_START(head, func_name, string);
        break;

    // generate function end
    case GEN_FUNC_END:
        char *retval = data.op1.id_name;
        FUNC_END(head, retval, string);
        break;

    // Add default case to handle unexpected values
    default:
        // Handle unexpected case
        break;
  }



  // // define data.op1.id_name to global
  // if (gencode == GEN_CREATE_ID) {
  //   char *id_name = data.op1.id_name;
  //   // sprintf(string, "DEFVAR GF@%s\n", id_name);
  //   // add_instr(head, string);
  //   CREATE_ID(head, id_name, string);
  // }

  // // moves int value in data.op1.int_val to data.op1.id_name
  // if (gencode == GEN_MOVE) {
  //     int int_val = data.op1.int_val;
  //     char *id_name = data.op1.id_name;
  //     // sprintf(string,"MOVE GF@%s int@%d\n", id_name, value);
  //     // add_instr(head, string);
  //     MOVE(head, id_name, int_val, string);
  // }
 
  // // pop value from expression to data.op1.id_name
  // if (gencode == GEN_ASSIGN) {
  //   char *id_name = data.op1.id_name;
  //   // sprintf(string, "POPS GF@%s\n", id_name);
  //   // add_instr(head, string);
  //   ASSIGN(head, id_name, string);
  // }

  // // write value of data.op1.id_name
  // if (gencode == GEN_WRITE) {
  //   char *id_name = data.op1.id_name;
  //   // sprintf(string, "WRITE GF@%s\n", id_name);
  //   // add_instr(head, string);
  //   WRITE(head, id_name, string);
  // }

  // // begin expression, clear stack
  // if (gencode == GEN_BEGIN_EXPR) {
  //   // string = "CLEARS\n";
  //   // add_instr(head, string);
  //   BEGIN_EXPR(head, string);
  // }

  // // push value of data.op1.id_name to stack
  // if (gencode == GEN_PUSH) {
  //   char *id_name = data.op1.id_name;
  //   // sprintf(string, "PUSHS GF@%s\n", id_name);
  //   // add_instr(head, string);
  //   PUSH(head, id_name, string);
  // }

  // // add two values from stack (top two) and push result to stack
  // if (gencode == GEN_ADD) {
  //   // string = "ADDS\n";
  //   // add_instr(head, string);
  //   ADD(head, string);
  // }

  // if (gencode == GEN_SUB) {
  //   // string = "SUBS\n";
  //   // add_instr(head, string);
  //   SUB(head, string);
  // }

  // if (gencode == GEN_MUL) {
  //   // string = "MULS\n";
  //   // add_instr(head, string);
  //   MUL(head, string);
  // }

  // if (gencode == GEN_DIV) {
  //   // string = "DIVS\n";
  //   // add_instr(head, string);
  //   DIV(head, string);
  // }

  // if (gencode == GEN_MAIN) {
  //   MAIN(head, string);
  // }


  return EXIT_SUCCESS;
}

void pop_list_to_file(instr_node **head) {
  FILE *file = fopen("IFJ23.code", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  instr_node *current = *head;
  int cnt = 5;
  while (current != NULL) {
    // printf("instruction to print to file on line %d: %s", cnt, current->instr);
    fprintf(file, "%s", current->instr);
    cnt++;
    instr_node *tmp = current;
    current = current->next;
    printf("instruction to print to file on line %d: %s", cnt, tmp->instr);
    // free(tmp->instr);
    tmp->instr = NULL;
    printf("instruction to prindawdawdwat to file on line %d: %s\n", cnt, tmp->instr);
    free(tmp);
  }
  fclose(file);
}

// int generate_code(FILE *file) {
//   generate_header(file);
//   generate_main(file);
//   return EXIT_SUCCESS;
// }

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