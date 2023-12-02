#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include <stdio.h>

// Enum for the data types in IFJ23
typedef enum {
    TYPE_INT, 
    TYPE_DOUBLE, 
    TYPE_STRING,
    TYPE_NIL,
    TYPE_INT_NULLABLE, 
    TYPE_DOUBLE_NULLABLE, 
    TYPE_STRING_NULLABLE, 
    TYPE_VOID, 
    FUNC,
    TYPE_UNKNOWN,
    TYPE_BOOL,
} DataType;


typedef enum {
    PREFIX_UNDERSCORE,
    PREFIX_DEFAULT,
} ParamPrefix;


typedef struct FuncParam {
    char* name;
    ParamPrefix prefix; 
    DataType dataType;
    struct FuncParam* next;
} ListFuncParam;




typedef struct {
    //TODO CHANGE TO DYNAMIC STRING MB
    char* name;              // Name of the symbol (variable, function, etc.)
    DataType dtype;          // Data type of the symbol
    bool isDefined;          // Whether the function has been defined
    bool canbeChanged;       // Whether the symbol can be changed
    bool isGlobal;           // Whether the symbol is global
    bool isFunction;         // Whether the symbol is a function
    DataType returnType;     // Return type of the function
    ListFuncParam paramTypes;    // Array of parameter types
    int paramCount;          // Number of parameters
    // struct SymTable* local_SymTable; // Local symbol table of the function
    bool isNil;               // Whether the symbol is nil
} SymData;

typedef struct AVLNode {
    char * key;              // Key of the tree
    SymData data;            // The data of the node (symbol information)
    struct AVLNode* left;    // Pointer to the left child
    struct AVLNode* right;   // Pointer to the right child
    int height;              // Height of the node (used for balancing)
} AVLNode;

typedef struct {
    char * name;             // Name of the symbol table
    AVLNode* root;           // Root of the AVL tree representing the symbol table
} SymTable;


/**
 * @brief Gets the height of an AVL node.
 *
 * @param node The AVL node whose height is to be determined.
 * @return int The height of the node.
 */
int height(AVLNode* node);

/**
 * @brief Calculates the balance factor of an AVL node.
 *
 * @param node The AVL node for which to calculate the balance factor.
 * @return int The balance factor of the node.
 */
int getBalance(AVLNode* node);

/**
 * @brief Performs a right rotation on an AVL node.
 *
 * @param y The node to be rotated.
 * @return AVLNode* The new root after rotation.
 */
AVLNode* rightRotate(AVLNode* y);

/**
 * @brief Performs a left rotation on an AVL node.
 *
 * @param x The node to be rotated.
 * @return AVLNode* The new root after rotation.
 */
AVLNode* leftRotate(AVLNode* x);

/**
 * @brief Finds the node with the minimum value in an AVL subtree.
 *
 * @param node The root node of the subtree.
 * @return AVLNode* The node with the minimum value in the subtree.
 */
AVLNode* findminValueNode(AVLNode* node);

/**
 * @brief Initializes a symbol table.
 *
 * @return SymTable* Pointer to the newly created symbol table.
 */
SymTable* create_SymTable();

/**
 * @brief Searches for a symbol in the symbol table.
 *
 * @param table The symbol table to search.
 * @param key The key of the symbol to find.
 * @return AVLNode* Node containing the symbol, or NULL if not found.
 */
AVLNode* search_SymTable(SymTable* table, char* key);

/**
 * @brief Creates a new AVL node with the given key and data.
 *
 * @param key The key for the new node.
 * @param data The data for the new node.
 * @return AVLNode* The newly created node.
 */
AVLNode* create_AVLNode(char* key, SymData data);

/**
 * @brief Initializes symbol data.
 *
 * @return SymData The initialized symbol data.
 */
SymData initSymData();

/**
 * @brief Inserts a symbol into the symbol table.
 *
 * @param table The symbol table.
 * @param key The key of the symbol.
 * @param data The data of the symbol.
 */
void insert_SymTable(SymTable* table, char* key, SymData data);

/**
 * @brief Deletes a symbol from the symbol table.
 *
 * @param root The root of the AVL tree.
 * @param key The key of the symbol to delete.
 * @return AVLNode* The new root of the tree.
 */
AVLNode* deleteNode(AVLNode* root, char* key);

/**
 * @brief Deletes a symbol from the symbol table.
 *
 * @param table The symbol table.
 * @param key The key of the symbol to delete.
 */
void delete_SymTable(SymTable* table, char* key);

/**
 * @brief Inserts a function symbol into the symbol table.
 *
 * @param table The symbol table.
 * @param key The key of the function.
 * @param returnType The return type of the function.
 * @param params The parameters of the function.
 * @param paramCount The number of parameters.
 */
void insert_FunctionSymTable(SymTable* table, char* key, DataType returnType, ListFuncParam* params, int paramCount);

/**
 * @brief Adds a parameter to a function's parameter list.
 *
 * @param list The parameter list.
 * @param paramName The name of the parameter.
 * @param dataType The data type of the parameter.
 * @param prefix The prefix of the parameter.
 * @return ListFuncParam* The updated parameter list.
 */
ListFuncParam* addParamToList(ListFuncParam* list, char* paramName, DataType dataType, ParamPrefix prefix);

/**
 * @brief Counts the number of parameters in a function's parameter list.
 *
 * @param params The parameter list.
 * @return int The number of parameters.
 */
int countParams(ListFuncParam* params);

/**
 * @brief Prints a function's parameter list.
 *
 * @param list The parameter list to print.
 */
void printFuncParamList(ListFuncParam* list);

/**
 * @brief Retrieves symbol data by key from the symbol table.
 *
 * @param table The symbol table.
 * @param key The key of the symbol.
 * @return SymData The symbol data.
 */
SymData getSymDataByKey(SymTable* table, char* key);

/**
 * @brief Updates the data of a symbol in the symbol table.
 *
 * @param table The symbol table.
 * @param key The key of the symbol.
 * @param newData The new data for the symbol.
 * @return bool True if update was successful, false otherwise.
 */
bool updateSymData(SymTable* table, char* key, SymData newData);

/**
 * @brief Prints symbol data.
 *
 * @param data The symbol data to print.
 */
void print_SymData(SymData* data);

/**
 * @brief Performs an in-order traversal of the AVL tree, printing each node.
 *
 * @param node The root node of the AVL tree.
 */
void inOrderTraversal(AVLNode* node);

/**
 * @brief Prints the contents of the symbol table.
 *
 * @param table The symbol table to print.
 */
void print_SymTable(SymTable* table);

/**
 * @brief Prints spaces for AVL tree visualization.
 *
 * @param count The number of spaces to print.
 */
void printSpaces(int count);

/**
 * @brief Prints the AVL tree structure.
 *
 * @param node The root node of the AVL tree.
 * @param space The starting space offset for printing.
 */
void print_AVLTree(AVLNode* node, int space);

/**
 * @brief Prints the entire AVL tree structure.
 *
 * @param table The symbol table containing the AVL tree.
 */
void printTree(SymTable* table);

#endif // SYMTABLE_H    
