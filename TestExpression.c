#include "expression_parse.c"

void add_symbol(SymTable* table, char* str, DataType type, bool isNullable) {
    SymData newData;
    //newData.name = strdup(str);  // strdup allocates memory and copies the string "a"
    newData.name = str;  // strdup allocates memory and copies the string "a"
    newData.dtype = type;    // Set data type to TYPE_INT
    newData.isDefined = true;    // Set as defined
    newData.canbeChanged = true; // Can be changed
    newData.isGlobal = true;     // Is a global symbol
    newData.isFunction = false;  // Not a function
    newData.isNil = isNullable;       // Not nil

    // Insert the new symbol into the symbol table
    insert_SymTable(table, newData.name, newData);
}

DataType test1() {
    FILE *file = fopen("tests_for_exp/1", "r");
    int error = 0;
    SymTable* table = create_SymTable();
    if (table == NULL) {
        fprintf(stderr, "Failed to create symbol table.\n");
        return 1;
    }
    // Create a new symbol
    add_symbol(table, "a", TYPE_INT, false);
    add_symbol(table, "b", TYPE_INT, false);
    Token token = get_token(file);
    DataType type = parse_expression(table, &token, &error, &file);
    free(table);
    return type;
}

DataType test2() {
    FILE *file = fopen("tests_for_exp/2", "r");
    int error = 0;
    SymTable* table = create_SymTable();
    if (table == NULL) {
        fprintf(stderr, "Failed to create symbol table.\n");
        return 1;
    }
    // Create a new symbol
    add_symbol(table, "a", TYPE_INT,false);
    add_symbol(table, "b", TYPE_INT, false);
    Token token = get_token(file);
    DataType type = parse_expression(table, &token, &error, &file);
    free(table);
    return type;
}

DataType test3() {
    FILE *file = fopen("tests_for_exp/3", "r");
    int error = 0;
    SymTable* table = create_SymTable();
    if (table == NULL) {
        fprintf(stderr, "Failed to create symbol table.\n");
        return 1;
    }
    // Create a new symbol
    add_symbol(table, "a", TYPE_STRING, false);
    add_symbol(table, "b", TYPE_STRING, false);
    Token token = get_token(file);
    DataType type = parse_expression(table, &token, &error, &file);
    free(table);
    return type;
}

DataType test4() {
    FILE *file = fopen("tests_for_exp/4", "r");
    int error = 0;
    SymTable* table = create_SymTable();
    if (table == NULL) {
        fprintf(stderr, "Failed to create symbol table.\n");
        return 1;
    }
    // Create a new symbol
    add_symbol(table, "a", TYPE_INT_NULLABLE, false);
    add_symbol(table, "b", TYPE_INT_NULLABLE, false);
    Token token = get_token(file);
    DataType type = parse_expression(table, &token, &error, &file);
    free(table);
    return type;
}

DataType test5() {
    FILE *file = fopen("tests_for_exp/5", "r");
    int error = 0;
    SymTable* table = create_SymTable();
    if (table == NULL) {
        fprintf(stderr, "Failed to create symbol table.\n");
        return 1;
    }
    // Create a new symbol
    add_symbol(table, "a", TYPE_INT_NULLABLE, true);
    add_symbol(table, "b", TYPE_INT_NULLABLE, true);
    Token token = get_token(file);
    DataType type = parse_expression(table, &token, &error, &file);
    free(table);
    return type;
}

int main() {
    DataType test_1 = test1();
    if(test_1 == TYPE_INT){
        printf("Test 1 passed\n");
    }
    else{
        printf("Test 1 failed\nExpected: ->");
        print_expression_type(TYPE_INT);
        printf("Actual: -> ");
        print_expression_type(test_1);
    }

    DataType test_2 = test2();
    if(test_2 == TYPE_DOUBLE){
        printf("Test 2 passed\n");
    }
    else{
        printf("Test 2 failed\nExpected: -> ");
        print_expression_type(TYPE_DOUBLE);
        printf("Actual: -> ");
        print_expression_type(test_2);
    }

    DataType test_3 = test3();
    if(test_3 == TYPE_STRING){
        printf("Test 3 passed\n");
    }
    else{
        printf("Test 3 failed\nExpected: -> ");
        print_expression_type(TYPE_STRING);
        printf("Actual: -> ");
        print_expression_type(test_3);
    }

    DataType test_4 = test4();
    if(test_4 == TYPE_INT){
        printf("Test 4 passed\n");
    }
    else{
        printf("Test 4 failed\nExpected: -> ");
        print_expression_type(TYPE_INT);
        printf("Actual: -> ");
        print_expression_type(test_4);
    }

    DataType test_5 = test5();
    if(test_5 == TYPE_BOOL){
        printf("Test 5 passed\n");
    }
    else{
        printf("Test 5 failed\nExpected: -> ");
        print_expression_type(TYPE_BOOL);
        printf("Actual: -> ");
        print_expression_type(test_5);
    }

    return 0;
}
