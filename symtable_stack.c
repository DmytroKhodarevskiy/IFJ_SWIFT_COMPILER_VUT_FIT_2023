
#include "symtable_stack.h"

// void initializeStack(SymStack *stack) {
void s_initializeStack(SymStack *stack) {
  stack->top = -1;
  stack->size = INITIAL_STACK_SIZE;
  stack->items = (SymTable **)malloc(stack->size * sizeof(SymTable*));

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
  stack->items = (SymTable **)realloc(stack->items, stack->size * sizeof(SymTable*));

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

    // return &(stack->items[stack->top]);
    return stack->items[stack->top];
}


void s_push(SymStack *stack, SymTable *item) {

  // printf("pushing: %s\n", item->name);

  if (stack->top == stack->size - 1) {
    // Stack is full, resize it
    // printf("YA SIUDA NE ZAHODIL\n");
    s_resizeStack(stack);
  }

  stack->items[++stack->top] = item;

  // (*stack->items)->name = item->name;
  // (*stack->items)->root = item->root;
  // (*stack->items)->next = item->next;

  // Print_Sym_stack(stack);
  // print_SymTable(&(stack->items[stack->top]));
  // print_SymTable(stack->items[stack->top]);
}

SymTable s_pop(SymStack *stack) {
  SymTable item;
  if (s_isEmpty(stack)) {
    fprintf(stderr, "Error: Stack underflow\n");
    exit(EXIT_FAILURE);
  }

  item = *(stack->items[stack->top--]);
  return item;
}

void s_freeStack(SymStack *stack) {
  free(stack->items);
  stack->top = -1;
  stack->size = 0;
}

// SymData s_search_symtack(SymStack *stack, char *key) {
AVLNode *s_search_symtack(SymStack *stack, char *key) {
  // SymData data;
  AVLNode *node;
  AVLNode *funcnode;
  SymTable *table;
  table = create_SymTable();
  int i;

  // printf("stack->top: %d\n", stack->top);

  for (i = stack->top; i >= 0; i--) {
    
    // printf("i: %d\n", i);
    // printf("function_naadadme:\n");

    table = stack->items[i];
    // printf("function_naadadme:\n");
    
    char function_name[256];
    // char *function_name = table->name;
    strncpy(function_name, table->name, strlen(table->name) + 1);
    // printf("table->name: %s\n", table->name);
    // printf("function_name: %s\n", function_name);
    node = search_SymTable(table, key);

    // printf("function_name: %s\n", function_name);

    if (node != NULL) {
      // data = node->data;
      return node;
    }

    // SymTable *global = create_SymTable();
    SymTable *global;
    global = stack->items[0];
    // printf("function_name: %s\n", function_name);
    funcnode = search_SymTable(global, function_name);
    if (funcnode == NULL) {
      fprintf(stderr, "Error: Undefined function\n");
    }


      if (funcnode != NULL) {     
          if (funcnode->data.isFunction) {

    // printf("HEEEEEEREEEE?\n");
            // printf("function_namedawdwa: %s\n", function_name);
    // printf("HEEEEEEREEEE?\n");
            // printf("funcnode: %s\n", funcnode->data.paramTypes.name);
            // printf("key: %s\n", key);
            // if (funcnode->data.paramTypes.next == NULL)
                // printf("PIZDA\n");

            // if (&funcnode->data.paramTypes == NULL)
                // printf("PIZDA\n");
            if (funcnode->data.paramCount != 0)
              node = s_search_param_list(&funcnode->data.paramTypes, key);

    // printf("HEEEEEEREEEE?\n");
            // if (node != NULL)
              // printf("node: %s\n", node->data.name);
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
  // ListFuncParam *param;
  // int i;
  if (param_list == NULL) {
    return NULL;
  }

  ListFuncParam* current = param_list;
  char *parameter = NULL;

    while (current != NULL) {
        if (strcmp(current->name, key) == 0) {
            // printf("current->name: %s\n", current->name);
            // printf("key: %s\n", key);
            parameter = current->name;
            break;
            // return current; // Name found, return the node
        }
            // printf("key: %s\n", key);
            // printf("current->name: %s\n", current->name);
        current = current->next;
    }

  // printf("parameter: %s\n", parameter);

  if (parameter == NULL) 
      return NULL; // Name not found

  // printf("parameter: %s\n", parameter);

    (*node).data = initSymData();

  // printf("parameter: %s\n", parameter);
    (*node).data.dtype = current->dataType;
    (*node).data.name = parameter;
  // printf("parameter: %s\n", parameter);
    // strncpy((*node).data.name, parameter, strlen(parameter) + 1);
  // printf("parameter: %s\n", parameter);

    (*node).key = parameter;
    (*node).left = NULL;
    (*node).right = NULL;
    (*node).height = 1;

    (*node).data.isDefined = true;
    (*node).data.isFunction = false;
    (*node).data.isGlobal = false;
    (*node).data.canbeChanged = false;
    // (*node).data.canbeChanged = true;
    (*node).data.paramCount = 0;
    (*node).data.paramTypes.next = NULL;
    (*node).data.returnType = TYPE_UNKNOWN;

    // printf("node->data.name: %s\n", node->data.name);
    // printf("node->data.dtype: %d\n", node->data.dtype);
    // printf("node->data.isDefined: %d\n", node->data.isDefined);
    // printf("node->data.isFunction: %d\n", node->data.isFunction);
    // printf("node->data.isGlobal: %d\n", node->data.isGlobal);
    // printf("node->data.canbeChanged: %d\n", node->data.canbeChanged);
    // printf("node->data.paramCount: %d\n", node->data.paramCount);
    // printf("node->data.paramTypes.next: %p\n", node->data.paramTypes.next);
    // printf("node->data.returnType: %d\n", node->data.returnType);

    // printf("node->data.name: %s\n", node->data.name);
    return node;
}

void Print_Sym_stack(SymStack *stack) {
  // printf("[$] ");
  
  int i;
  SymTable *table;
  // AVLNode *node;
  // ListFuncParam *param;

  printf("[$] ");

  for (i = 0; i <= stack->top; i++) {
    // table = *(stack->items) + i;
    table = stack->items[i];


    printf("%s ", table->name);
    // printf("Symbol table: %s\n", table->name);
    // printf("--------------------------------------------------\n");
    // printf("Name\t\tType\t\tDefined\t\tGlobal\t\tCan be changed\t\tFunction\t\tReturn type\t\tParameter count\t\tParameter types\n");
    // printf("--------------------------------------------------\n");

    // node = table->root;
    // Print_Sym_table(node);
  }
    printf("\n");
}

SymData* s_getFirstFunctionSymData(SymStack *stack) {
    if (stack == NULL || s_isEmpty(stack)) {
        fprintf(stderr, "Error: Stack is empty or NULL in s_getFirstFunctionSymData\n");
        return NULL;
    }

    // Iterate through the stack from top to bottom
    for (int i = stack->top; i >= 0; i--) {
        SymTable *currentTable = stack->items[i];
        if (currentTable == NULL) {
            continue; // Skip if the current symbol table is NULL
        }

        // Check in the global symbol table if the current symbol table represents a function
        SymTable *globalTable = stack->items[0];
        AVLNode *node = search_SymTable(globalTable, currentTable->name);
        if (node != NULL && node->data.isFunction) {
            // Return SymData if the symbol table represents a function
            return &(node->data);
        }
    }

    fprintf(stderr, "Error: No function symbol table found in the stack\n");
    return NULL;
}

