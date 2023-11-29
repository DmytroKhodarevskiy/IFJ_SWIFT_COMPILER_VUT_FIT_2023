include <stdlib.h>
#include "symtable.h"
#include "shared.h"

void BST_init(SymbolNode *root) {
    (*root) = NULL;
}

SymbolNode BST_find(SymbolNode root, string *name){
    if(root == NULL) {
        return NULL;
    } else {
        int cmp = string_cmp(name, &root->name);
        if(cmp == 0) {
            return root;
        } else if(cmp < 0) {
            return BST_find(root->left, name);
        } else {
            return BST_find(root->right, name);
        }
    }
}

SymbolNode BST_create_node(string *name, void *data, SymbolType type) {
    SymbolNode node = (SymbolNode)malloc(sizeof(struct symbol_t));
    if(node == NULL) {
        return NULL;
    }
    string_init(&node->name);
    string_copy(&node->name, name);
    node->data = data;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int height(SymbolNode node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

SymbolNode rightRotate(SymbolNode y) {
    SymbolNode x = y->left;
    SymbolNode T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

SymbolNode leftRotate(SymbolNode x) {
    SymbolNode y = x->right;
    SymbolNode T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(SymbolNode N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

void BST_insert(SymbolNode *root, string *name, void *data, SymbolType type) {
    // Perform the normal BST insert
    if (*root == NULL) {
        *root = BST_create_node(name, data, type);
        return;
    }

    if (string_cmp(name, &((*root)->name)) < 0)
        BST_insert(&((*root)->left), name, data, type);
    else if (string_cmp(name, &((*root)->name)) > 0)
        BST_insert(&((*root)->right), name, data, type);
    else
        return; // Duplicate key is not allowed in BST

    // Update the height of the ancestor node
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));

    // Balance the tree
    int balance = getBalance(*root);

    // Left Left Case
    if (balance > 1 && string_cmp(name, &((*root)->left->name)) < 0) {
        *root = rightRotate(*root);
    }

    // Right Right Case
    if (balance < -1 && string_cmp(name, &((*root)->right->name)) > 0) {
        *root = leftRotate(*root);
    }

    // Left Right Case
    if (balance > 1 && string_cmp(name, &((*root)->left->name)) > 0) {
        (*root)->left = leftRotate((*root)->left);
        *root = rightRotate(*root);
    }

    // Right Left Case
    if (balance < -1 && string_cmp(name, &((*root)->right->name)) < 0) {
        (*root)->right = rightRotate((*root)->right);
        *root = leftRotate(*root);
    }
}

SymbolNode minValueNode(SymbolNode node) {
    SymbolNode current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

void BST_delete(SymbolNode *root, string *name) {
    // Perform standard BST delete
    if (*root == NULL)
        return;

    if (string_cmp(name, &((*root)->name)) < 0)
        BST_delete(&((*root)->left), name);
    else if (string_cmp(name, &((*root)->name)) > 0)
        BST_delete(&((*root)->right), name);
    else {
        // Node with one child or no child
        if ((*root)->left == NULL || (*root)->right == NULL) {
            SymbolNode temp = (*root)->left ? (*root)->left : (*root)->right;
            if (temp == NULL) {
                temp = *root;
                *root = NULL;
            } else {
                **root = *temp;
            }
            free(temp);
        } else {
            // Node with two children
            SymbolNode temp = minValueNode((*root)->right);
            (*root)->name = temp->name;
            (*root)->data = temp->data;
            (*root)->type = temp->type;
            BST_delete(&((*root)->right), &(temp->name));
        }
    }

    // If the tree had only one node then return
    if (*root == NULL)
        return;

    // Update the height of the current node
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));

    // Balance the tree
    int balance = getBalance(*root);

    // Left Left Case
    if (balance > 1 && getBalance((*root)->left) >= 0)
        *root = rightRotate(*root);

    // Left Right Case
    if (balance > 1 && getBalance((*root)->left) < 0) {
        (*root)->left = leftRotate((*root)->left);
        *root = rightRotate(*root);
    }

    // Right Right Case
    if (balance < -1 && getBalance((*root)->right) <= 0)
        *root = leftRotate(*root);

    // Right Left Case
    if (balance < -1 && getBalance((*root)->right) > 0) {
        (*root)->right = rightRotate((*root)->right);
        *root = leftRotate(*root);
    }
}

void BST_destroy(SymbolNode *root) {
    if (*root != NULL) {
        BST_destroy(&((*root)->left));
        BST_destroy(&((*root)->right));
        free(*root);
        *root = NULL;
    }
}

void st_init(Symtable *table) {
    BST_init(&(table->root));
}

void st_insert_var(Symtable *table, string *name, DataVariable *data) {
    BST_insert(&(table->root), name, data, Variable);
}

void st_insert_func(Symtable *table, string *name, DataFunction *data) {
    BST_insert(&(table->root), name, data, Function);
}

SymbolNode st_find(Symtable table, string *name) {
    return BST_find(table.root, name);
}

void st_delete(Symtable *table, string *name) {
    BST_delete(&(table->root), name);
}

void st_destroy(Symtable *table) {
    BST_destroy(&(table->root));
}

void st_print(Symtable table) {

}