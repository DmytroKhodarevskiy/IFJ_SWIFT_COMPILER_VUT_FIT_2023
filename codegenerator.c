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

    return EXIT_SUCCESS;
}

void generate_header(FILE *file) {
  fprintf(file, ".IFJcode23\n");
}

int generate_main(FILE *file) {
  // GENERATE ALL SHIT HERE :)


  return EXIT_SUCCESS;
}

int generate_code(FILE *file) {
  generate_header(file);
  generate_main(file);
  return EXIT_SUCCESS;
}

void destroy_file() {
  remove("IFJ23.code"); 
}

// int main() {
// change main to any other function and call it
int main() {
  int status;

  destroy_file();

  FILE *file;
  status = create_file(&file);

  if (status == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  status = generate_code(file);
  if (status == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  fclose(file);

  return EXIT_SUCCESS;
}