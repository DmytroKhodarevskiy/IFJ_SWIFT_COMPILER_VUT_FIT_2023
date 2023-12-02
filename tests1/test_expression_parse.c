#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "../expression_parse.c"

void mute_stdout(void) {
    fflush(stdout); // Flush any buffered data
    int fd = open("/dev/null", O_WRONLY); // Open /dev/null for writing
    dup2(fd, STDOUT_FILENO); // Redirect stdout to /dev/null
    close(fd); // Close the file descriptor for /dev/null
}

void unmute_stdout(int saved_stdout) {
    fflush(stdout); // Flush any data written to /dev/null
    dup2(saved_stdout, STDOUT_FILENO); // Restore stdout
    close(saved_stdout); // Close the saved file descriptor
}

bool test(char *test_file) {
  FILE* file = fopen(test_file, "r");

  // printf("Testing file: %s\n", test_file);

  if(file == NULL){
    printf("Error: File not found\n");
    exit(1);
  }
  int error = 0;

  int saved_stdout = dup(STDOUT_FILENO);
  mute_stdout();

  while (true){
    if (feof(file)){
      break;
    }
    Token token = get_token(file);
    bool lex_ok;
    if (get_index_from_token(token) != 8){
      lex_ok = parse_expression(&token, &error, &file);
      unmute_stdout(saved_stdout);
      return lex_ok;
    }
  }
  unmute_stdout(saved_stdout);
  return false;
}


void Perform_test_true(char *test_file, int *err) {
  // printf("Testing file: %s\n", test_file);
  bool ok = test(test_file);
  if (ok) {
    printf("\033[32m Test %s passed! \033[0m\n", test_file);
  } else {
    (*err)++;
    printf("\033[31m Test %s failed... \033[0m\n", test_file);
  }

}

void Perform_test_false(char *test_file, int *err) {
  // printf("Testing file: %s\n", test_file);
  bool ok = test(test_file);
  if (!ok) {
    printf("\033[32m Test %s passed! \033[0m\n", test_file);
  } else {
    (*err)++;
    printf("\033[31m Test %s failed... \033[0m\n", test_file);
  }

}

// Function to process each filename
void processFile_true(const char *filename, int *err) {
    char *folder = malloc(strlen(filename) + strlen(folder) + 1);
    strcpy(folder, "tests1/inputs/true/");
    strcat(folder, filename);
    Perform_test_true(folder, err);
    free(folder);
}

void processFile_false(const char *filename, int *err) {
    char *folder = malloc(strlen(filename) + strlen(folder) + 1);
    strcpy(folder, "tests1/inputs/false/");
    // printf("%s\n", folder);
    // printf("%s\n", filename);
    strcat(folder, filename);
    // folder[strlen(folder)] = '\0';
    Perform_test_false(folder, err);
    free(folder);
}

// Function to process all files in the directory
void processAllFiles(const char **filenames, int count, bool foldertype, int *err) {
    
    if (foldertype) {
      for (int i = 0; i < count; ++i) {
        processFile_true(filenames[i], err);
      }
    } else {
      for (int i = 0; i < count; ++i) {
        processFile_false(filenames[i], err);
      }
    }
}

void process_dir(const char *folderPath, bool foldertype, int *err) {
  // Allocate an initial array of string pointers
    DIR *dir;
    struct dirent *entry;
    char **filenames;
    int count = 0;
    int arr_size = 10; // Initial size of the array
    filenames = malloc(arr_size * sizeof(char*));
    if (!filenames) {
        perror("Malloc failed");
    }

    // Open the directory
    dir = opendir(folderPath);
    if (dir == NULL) {
        perror("Unable to read directory");
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Ignore the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Resize the array if necessary
        if (count >= arr_size) {
            arr_size *= 2;
            filenames = realloc(filenames, arr_size * sizeof(char*));
            if (!filenames) {
                perror("Realloc failed");
            }
        }

        // Allocate memory for the filename and copy it
        filenames[count] = strdup(entry->d_name);
        // printf("%s\n", filenames[count]);
        if (!filenames[count]) {
            perror("Strdup failed");
        }

        count++;
    }

    // Process all files
    processAllFiles((const char **)filenames, count, foldertype, err);

    // printf("%d files processed\n", count);
    // printf("%s file\n", filenames[0]);
    // printf("%s file\n", filenames[1]);
    // printf("%s file\n", filenames[2]);
    // for (int i = 0; i < count; ++i) {
    //     if (filenames[i] != NULL) {
    //       free(filenames[i]);
    //       filenames[i] = NULL; // Prevent double free by setting to NULL
    //     } 
    //   }
    // free(filenames);
    closedir(dir);

}


int main() {
    // DIR *dir;
    // struct dirent *entry;
    const char *folderPathTrue = "tests1/inputs/true";
    const char *folderPathFalse = "tests1/inputs/false";
    // char **filenames;
    // int count = 0;
    // int arr_size = 10; // Initial size of the array
    int err = 0;

    process_dir(folderPathTrue, true, &err);
    printf("\n");
    process_dir(folderPathFalse, false, &err);


    if (err > 0) {
      printf("\n\033[31m %d tests1 failed... Димончик работай \033[0m\n", err);
    } else 
    printf("\nAll tests1 passed!\n");

    // Cleanup
    // for (int i = 0; i < count; ++i) {
    //     printf("%s\n", filenames[i]);
    //     free(filenames[i]);
    // }
    // free(filenames);

    return 0;
}