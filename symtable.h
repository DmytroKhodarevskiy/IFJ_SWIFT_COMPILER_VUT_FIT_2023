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
    struct SymTable* local_SymTable; // Local symbol table of the function
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
    AVLNode* root;           // Root of the AVL tree representing the symbol table
} SymTable;




// Function prototypes for AVL tree and symbol table management
SymTable* create_SymTable();
void insert_SymTable(SymTable* table, char* key, SymData data);
void delete_SymTable(SymTable* table, char* key);
AVLNode* search_SymTable(SymTable* table, char* key);


// Function prototype for printing the symbol table
void print_SymTable(SymTable* table);


// AVL tree balancing functions
AVLNode* rightRotate(AVLNode* node);
AVLNode* leftRotate(AVLNode* node);
int getBalanceFactor(AVLNode* node);
void updateHeight(AVLNode* node);
AVLNode* findminValueNode(AVLNode* node);

SymData getSymDataByKey(SymTable* table, char* key); 

void insert_FunctionSymTable(SymTable* table, char* key, DataType returnType, ListFuncParam* paramTypes, int paramCount);
ListFuncParam* addParamToList(ListFuncParam* list, char* paramName, DataType dataType, ParamPrefix prefix);
int countParams(ListFuncParam* params);


void printFuncParamList(ListFuncParam* list);



void destroy_SymTable(SymTable* table);



#endif // SYMTABLE_H    
