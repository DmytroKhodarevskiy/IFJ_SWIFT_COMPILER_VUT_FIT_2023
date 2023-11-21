#include <stdio.h>
#include <stdlib.h>

#include "codegenerator.h"

int create_file(FILE **file) {
    *file = fopen("IFJ23.code", "w");

    if (*file == NULL) {
        // If the file couldn't be opened/created, print an error message
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // fclose(*file);
    return EXIT_SUCCESS;
}

void generate_header(FILE *file) {
  fprintf(file, ".IFJcode23\n");
}

// let a
void appendToFile(const char *filename, const char *content) {
    FILE *file = fopen(filename, "a"); // Open in append mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s\n", content); // Append content to the file
    fclose(file); // Always close the file when you're done
}

// void add_instr(instr_node **head, char *instr) {
//   instr_node *new_node = malloc(sizeof(instr_node));
//   new_node->instr = instr;
//   new_node->next = *head;
//   *head = new_node;
// }

void add_instr(instr_node **head, char *instr) {
    instr_node *new_node = malloc(sizeof(instr_node));
    if (new_node == NULL) {
        // Handle memory allocation error
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


int generate_code(instr_node **head, Data data, int gencode) {
  
  // GENERATE ALL SHIT HERE :)
  char *string = malloc(256 * sizeof(char)); // Allocate memory for the string on the heap
    if (string == NULL) {
        // Handle memory allocation error
        return EXIT_FAILURE;
    }
  // FILE *file = fopen("IFJ23.code", "a");

  if (gencode == GEN_CREATE_ID) {
    char *id_name = data.op1.id_name;
    // char string[256];
    // sprintf(string, )
    sprintf(string, "DEFVAR GF@%s\n", id_name);
    add_instr(head, string);
    // fprintf(file, "DEFVAR GF@%s\n", id_name);
  }

 if (gencode == GEN_MOVE)
 {
    int value = data.op1.int_val;
    char *id_name = data.op1.id_name;
    // char string[256];
    sprintf(string,"MOVE GF@%s int@%d\n", id_name, value);
    add_instr(head, string);
    // fprintf(file,"MOVE GF@%s int@%d\n", id_name, value);
 }
 

  if (gencode == GEN_ASSIGN) {
    char *id_name = data.op1.id_name;
    // char *string;
    sprintf(string, "POPS GF@%s\n", id_name);
    add_instr(head, string);
    // fprintf(file, "POPS GF@%s\n", id_name);
  }

  if (gencode == GEN_WRITE) {
    // printf("YA EBLAN\n");
    char *id_name = data.op1.id_name;
    // printf("YA EBLAN\n");
    // char *string;
    sprintf(string, "WRITE GF@%s\n", id_name);
    add_instr(head, string);
    // fprintf(file, "WRITE GF@%s\n", id_name);
    // printf("YA EBLAN\n");
  }

  if (gencode == GEN_BEGIN_EXPR) {
    // char *string = "CLEARS\n";
    string = "CLEARS\n";
    add_instr(head, string);
    // fprintf(file, "CLEARS\n");
  }

  if (gencode == GEN_PUSH) {
    char *id_name = data.op1.id_name;
    // printf("AWDOAWDOAWDOIAOWIDOWDOWN %s\n", id_name);
    // char string[256];
    sprintf(string, "PUSHS GF@%s\n", id_name);
    add_instr(head, string);
    // fprintf(file, "PUSHS GF@%s\n", id_name);
  }

  // $ a + b
  if (gencode == GEN_ADD) {
    // char *string = "ADDS\n";
    string = "ADDS\n";
    add_instr(head, string);
    // fprintf(file, "ADDS\n");
  }




  return EXIT_SUCCESS;
}

void pop_list_to_file(instr_node **head) {
  FILE *file = fopen("IFJ23.code", "a");
  instr_node *current = *head;
  int cnt = 0;
  while (current != NULL) {
    // printf("instruction to print to file %d: %s", cnt, current->instr);
    fprintf(file, "%s", current->instr);
    if (cnt == 0) {
      fprintf(file, "%s", current->instr);
    }
    cnt++;
    current = current->next;
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

  // fclose(file);

  return EXIT_SUCCESS;
}