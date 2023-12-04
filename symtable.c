#include "symtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))


/////////////////////          Helpful functions

int height(AVLNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}


int getBalance(AVLNode* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}


AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

   
    x->right = y;
    y->left = T2;

    
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

   
    return x;
}


AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    
    y->left = x;
    x->right = T2;


    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

   
    return y;
}

AVLNode* findminValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}







///////////////////////////////////////////////////////////////////



/**
 * @brief Creates a new symbol table
 * 
 * 
 * @return SymTable* Pointer to the newly created symbol table
 */
SymTable* create_SymTable() {
    SymTable* newTable = (SymTable*)malloc(sizeof(SymTable));
    if (newTable == NULL) {
        fprintf(stderr, "Memory allocation for SymTable failed\n");
        return NULL;
    }

    // newTable->name = malloc(sizeof(char)*256); // +1 for null terminator
    // memset(newTable->name, 0, 256 * sizeof(char));
    newTable->name = malloc(sizeof(char) * 256);
    memset(newTable->name, 0, 256 * sizeof(char));



    newTable->root = NULL;
    return newTable;
}


/**
 * @brief Searches the symbol table for a symbol with the given name
 * 
 * 
 * @param table The symbol table to search
 * @param name The name of the symbol to search for
 * @return AVLNode* Pointer to the node containing the symbol, or NULL if the symbol is not found
 */
AVLNode* search_SymTable(SymTable* table, char* key) {
    AVLNode* current = table->root;
    while (current != NULL) {
        int compareResult = strcmp(current->key, key);

        if (compareResult == 0) {
            // if (current->data.isDefined == false) {
                // return NULL;
            // }
            return current;
        } else if (compareResult > 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}




/**
 * @brief Creates a new AVL node with the given key and data
 * 
 * 
 * @param key The key of the new symbol
 * @param data The data of the new symbol
 * @return AVLNode* Pointer to the newly created node
 */
AVLNode* create_AVLNode(char* key, SymData data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation for AVLNode failed\n");
        return NULL;
    }
    
    node->key = key;  
    node->data = data;        
    node->left = NULL;        
    node->right = NULL;      
    node->height = 1;         
    return node;
}



SymData initSymData() {
    SymData node;
    // if (node == NULL) {
        // return NULL; // Memory allocation failed
    // }

    // Allocate memory for the name and copy the initialName into it
    // node->name = malloc(strlen(initialName) + 1); // +1 for null terminator
    // if (node->name == NULL) {
        // free(node); // Don't forget to free the node if name allocation fails
        // return NULL;
    // }
    // strcpy(node->name, initialName);

    node.name = "???"; //
    node.dtype = TYPE_UNKNOWN;//
    node.isDefined = false;
    node.canbeChanged = false; //
    node.isGlobal = false;//
    node.isFunction = false;//
    node.returnType = TYPE_UNKNOWN; // Set to an appropriate initial value
    node.paramTypes.next = NULL; // Set to an appropriate initial value
    node.paramCount = 0;
    // node.local_SymTable = NULL;
    node.isNil = false;

    return node;
}



/**
 * @brief Inserts a new symbol into the symbol table
 * 
 * 
 * @param table The symbol table to insert into
 * @param key The key of the new symbol
 * @param data The data of the new symbol
 * @return AVLNode* Pointer to the newly created node
 */
AVLNode* insert_AVLNode(AVLNode* node, char* key, SymData data) {
   
    if (node == NULL)
        return(create_AVLNode(key, data));

    if (strcmp(key, node->key) < 0)
        node->left = insert_AVLNode(node->left, key, data);
    else if (strcmp(key, node->key) > 0)
        node->right = insert_AVLNode(node->right, key, data);
    else  
        return node;

    
    node->height = 1 + max(height(node->left), height(node->right));

   
    int balance = getBalance(node);


    if (balance > 1 && strcmp(key, node->left->key) < 0)
        return rightRotate(node);

    
    if (balance < -1 && strcmp(key, node->right->key) > 0)
        return leftRotate(node);

    
    if (balance > 1 && strcmp(key, node->left->key) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

   
    if (balance < -1 && strcmp(key, node->right->key) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}



/**
 * @brief Inserts a new symbol into the symbol table
 * 
 * 
 * @param table The symbol table to insert into
 * @param key The key of the new symbol
 * @param data The data of the new symbol
 */
void insert_SymTable(SymTable* table, char* key, SymData data) {
    if (search_SymTable(table, key) != NULL) {
        exitWithError("Error: Symbol already defined\n", ERR_SEMANT_UNDF_VALUE);
    }

    table->root = insert_AVLNode(table->root, key, data);
}



/**
 * @brief Deletes a node from the tree
 * 
 * 
 * @param root The root of the subtree to delete from
 * @param key The key of the node to delete
 * @return AVLNode* Pointer to the new root of the subtree
 */

AVLNode* deleteNode(AVLNode* root, char* key) {
    if (root == NULL)
        return root;

    if (strcmp(key, root->key) < 0)
        root->left = deleteNode(root->left, key);

    else if (strcmp(key, root->key) > 0)
        root->right = deleteNode(root->right, key);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
               
                *root = *temp; 
            }
            free(temp);
        } else {
            AVLNode* temp = findminValueNode(root->right);

            root->key = temp->key;
            root->data = temp->data; 


            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

 
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

  
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}





/**
 * @brief Deletes the given symbol from the symbol table
 * 
 * 
 * @param table The symbol table to delete from
 * @param key The key of the symbol to delete
 */
void delete_SymTable(SymTable* table, char* key) {
    table->root = deleteNode(table->root, key);
}




/**
 * 
 * @brief Inserts Function into the symbol table
 * 
 * @param table The symbol table to insert into
 * @param key The key of the new symbol
 * @param returnType The return type of the function
 * @param paramTypes The parameter types of the function
 * @param paramCount The number of parameters of the function
 */
void insert_FunctionSymTable(SymTable* table, char* key, DataType returnType, ListFuncParam* params, int paramCount) {
    if (table == NULL)
        return;

    SymData functionData;
    if (params == NULL) {
        functionData.paramTypes.next = NULL;
        functionData.paramCount = 0;
    }
    else {
        functionData.paramTypes = *params;
        functionData.paramCount = paramCount;
    }
    // functionData.paramTypes = *params; 
    // functionData.paramCount = paramCount;
    
    functionData.name = key; 
    functionData.dtype = FUNC; 
    functionData.returnType = returnType;
    functionData.isDefined = true;
    functionData.canbeChanged = false;
    functionData.isFunction = true;
    functionData.isGlobal = true; 
    // functionData.local_SymTable = (struct SymTable*)create_SymTable();

    insert_SymTable(table, functionData.name, functionData);
}








/**
 * Adds a new parameter to the parameter list.
 *
 * @param list The head of the parameter list.
 * @param paramName The name of the parameter.
 * @param prefix The prefix of the parameter.
 * @param dataType The data type of the parameter.
 * @return The head of the updated parameter list.
 */
ListFuncParam* addParamToList(ListFuncParam* list, char* paramName, DataType dataType, ParamPrefix prefix, char* prefixName) {
  ListFuncParam* newParam = (ListFuncParam*)malloc(sizeof(ListFuncParam));
  if (newParam == NULL) {
    // Handle memory allocation failure
    return NULL;
  }

  // Allocate memory for the parameter name and copy it
  newParam->name = (char*)malloc(strlen(paramName) + 1); // +1 for null terminator
  if (newParam->name == NULL) {
    // Handle memory allocation failure
    free(newParam);
    return NULL;
  }
  strcpy(newParam->name, paramName);

  newParam->prefixName = prefixName;
  newParam->prefix = prefix;
  newParam->dataType = dataType;
  newParam->next = NULL;

  // Append to the list
  if (list == NULL) {
    return newParam;
  } else {
    ListFuncParam* current = list;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newParam;
    return list;
  }
}





int countParams(ListFuncParam* params) {
    int count = 0;
    ListFuncParam* current = params;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}



void printFuncParamList(ListFuncParam* list) {
    const ListFuncParam* current = list;
    while (current != NULL) {
        fprintf(stderr, "Parameter Name: %s, Data Type: %d, Prefix: %d, prefix string: %s\n", current->name, current->dataType, current->prefix, current->prefixName);
        current = current->next;
    }
}


/**
 * Searches the symbol table for a symbol with the given key and returns its data.
 * If the symbol is not found, sets an error flag.
 *
 * @param table The symbol table to search in.
 * @param key The key of the item to search for.
 * @param error Pointer to an int where the error flag will be set (0 for no error, 1 for error).
 * @return The SymData of the found item, or an undefined value if not found.
 */
SymData getSymDataByKey(SymTable* table, char* key) {

    if (table == NULL || key == NULL) {
        fprintf(stderr, "Node not found\n");
        exit(1);
    }

    AVLNode* foundNode = search_SymTable(table, key);
    if (foundNode == NULL) {
        fprintf(stderr, "Node not found\n");
        exit(1);
    }

    return foundNode->data;
}



/**
 * @brief Updates the data of a symbol in the symbol table.
 * 
 * @param table The symbol table.
 * @param key The key of the symbol to update.
 * @param newData The new data to update the symbol with.
 * @return true if the update was successful, false otherwise.
 */
bool updateSymData(SymTable* table, char* key, SymData newData) {
    if (table == NULL || key == NULL) {
        fprintf(stderr, "Invalid table or key\n");
        return false;
    }

    // Search for the node with the given key
    AVLNode* node = search_SymTable(table, key);
    if (node == NULL) {
        fprintf(stderr, "Symbol with key '%s' not found\n", key);
        return false;
    }

    // Update the data of the symbol
    node->data = newData;

    return true;
}




// FUNCTIONS FOR PRINTING THE SYMBOL TABLE AND TREE
void print_SymData(SymData* data) {
    if (!data) return;

    fprintf(stderr, "Name: %s, DataType: %d, isDefined: %d, canbeChanged: %d, isGlobal: %d, isFunction: %d IsNil: %d\n",
           data->name, data->dtype, data->isDefined, data->canbeChanged, data->isGlobal, data->isFunction, data->isNil);

    if (data->isFunction) {
        printf(", ReturnType: %d, ParamCount: %d\n", data->returnType, data->paramCount);
        ListFuncParam* param = &data->paramTypes;
            if (data->paramCount != 0) {
                while (param) {
                    if (param != NULL)
                    fprintf(stderr, "Param: %s, DataType: %d , Prefixes: %d\n", param->name, param->dataType, param->prefix);
                    if (param != NULL)
                    param = param->next;
            }
        }
    } else {
        fprintf(stderr, "\n");
    }
}

void inOrderTraversal(AVLNode* node) {
    if (node == NULL) {
        return;
    }
    inOrderTraversal(node->left);
    fprintf(stderr, "Key: %s, ", node->key);
    print_SymData(&node->data);
    inOrderTraversal(node->right);
    // printf("CHEGO\n");
}

/**
 * @brief Printing the symbol table
 * 
 * 
 * @param table The symbol table to print
 */
void print_SymTable(SymTable* table) {
    if (table == NULL || table->root == NULL) {
        fprintf(stderr, "Symbol table is empty or not initialized.\n");
        return;
    }

    fprintf(stderr, "Symbol Table Contents:\n");
    inOrderTraversal(table->root);
    fprintf(stderr, "\n");
}




void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("   ");
    }
}


void print_AVLTree(AVLNode* node, int space) {
    if (node == NULL) return;


    space += 5;

    print_AVLTree(node->right, space);

    printf("\n");
    printSpaces(space);
    printf("%s\n", node->key);

    print_AVLTree(node->left, space);
}

void printTree(SymTable* table) {
    if (table == NULL || table->root == NULL) {
        fprintf(stderr, "Symbol table is empty or not initialized.\n");
        return;
    }

    print_AVLTree(table->root, 0);
}