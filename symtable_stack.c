
#include "symtable_stack.h"

// void initializeStack(SymStack *stack) {
void s_initializeStack(SymStack *stack) {
  stack->top = -1;
  stack->size = INITIAL_STACK_SIZE;
  stack->items = (SymTable *)malloc(stack->size * sizeof(SymTable));

  if (stack->items == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

int s_isEmpty(SymStack *stack) {
  return stack->top == -1;
}

void s_resizeStack(SymStack *stack) {
  stack->size *= STACK_GROWTH_FACTOR;
  stack->items = (SymTable *)realloc(stack->items, stack->size * sizeof(SymTable));

  if (stack->items == NULL) {
    exitWithError("Memory allocation failed.\n", ERR_INTERNAL);
    // fprintf(stderr, "Error: Memory allocation failed\n");
    // exit(EXIT_FAILURE);
  }
}

SymTable *s_peek(SymStack *stack) {
    if (s_isEmpty(stack)) {
        // fprintf(stderr, "Error: Stack is empty\n");
        // exit(EXIT_FAILURE);
        exitWithError("Error: Stack is empty\n", ERR_INTERNAL);
    }

    return &(stack->items[stack->top]);
}


void s_push(SymStack *stack, SymTable item) {
  if (stack->top == stack->size - 1) {
    // Stack is full, resize it
    s_resizeStack(stack);
  }

  stack->items[++stack->top] = item;
}

SymTable s_pop(SymStack *stack) {
  SymTable item;
  if (s_isEmpty(stack)) {
    fprintf(stderr, "Error: Stack underflow\n");
    exit(EXIT_FAILURE);
  }

  item = stack->items[stack->top--];
  return item;
}

void s_freeStack(SymStack *stack) {
  free(stack->items);
  stack->top = -1;
  stack->size = 0;
}

// SymData s_search_symtack(SymStack *stack, char *key) {
AVLNode *s_search_symtack(SymStack *stack, char *key) {
  SymData data;
  AVLNode *node;
  AVLNode *funcnode;
  SymTable *table;
  table = create_SymTable();
  int i;

  // printf("stack->top: %d\n", stack->top);

  for (i = stack->top; i >= 0; i--) {
    
    
    // printf("function_naadadme:\n");

    *table = stack->items[i];
    // printf("function_naadadme:\n");
    
    char *function_name = table->name;
    
    // printf("function_name: %s\n", function_name);
    node = search_SymTable(table, key);
    // printf("function_name: %s\n", function_name);

    
    if (node != NULL) {
      // data = node->data;
      return node;
    }

    SymTable *global = create_SymTable();
    *global = stack->items[0];
    funcnode = search_SymTable(global, function_name);

      if (funcnode != NULL) {     
          if (funcnode->data.isFunction) {
            node = s_search_param_list(funcnode->data.paramTypes.next, key);
            if (node != NULL)
              return node;
          }
      }

    }

  return NULL;
  // exitWithError("Error: Undefined variable or function\n", ERR_SEMANT_UNDF_VALUE);
}

AVLNode *s_search_param_list(ListFuncParam *param_list, char *key) {
  AVLNode *node;
  ListFuncParam *param;
  int i;

  ListFuncParam* current = param_list;
  char *parameter = NULL;

    while (current != NULL) {
        if (strcmp(current->name, key) == 0) {
            parameter = current->name;
            break;
            // return current; // Name found, return the node
        }
        current = current->next;
    }

  if (parameter == NULL) 
      return NULL; // Name not found

    (*node).data = initSymData();

    (*node).data.dtype = current->dataType;
    (*node).data.name = parameter;
    (*node).key = parameter;
    (*node).left = NULL;
    (*node).right = NULL;
    (*node).height = 1;

    (*node).data.isDefined = true;
    (*node).data.isFunction = false;
    (*node).data.isGlobal = false;
    (*node).data.canbeChanged = false;
    (*node).data.paramCount = 0;
    (*node).data.paramTypes.next = NULL;
    (*node).data.returnType = TYPE_UNKNOWN;

    return node;
}

void Print_Sym_stack(SymStack *stack) {
  // printf("[$] ");
  
  int i;
  SymTable *table;
  AVLNode *node;
  ListFuncParam *param;

  printf("[$] ");

  for (i = 0; i <= stack->top; i++) {
    table = stack->items + i;

    printf("%s ", table->name);
    // printf("Symbol table: %s\n", table->name);
    // printf("--------------------------------------------------\n");
    // printf("Name\t\tType\t\tDefined\t\tGlobal\t\tCan be changed\t\tFunction\t\tReturn type\t\tParameter count\t\tParameter types\n");
    // printf("--------------------------------------------------\n");

    node = table->root;
    // Print_Sym_table(node);
  }
    printf("\n");
}