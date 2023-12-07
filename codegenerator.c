#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "codegenerator.h"

char *frame_name;

// Define the macro to set 'frame' based on the Frame enum
#define SET_FRAME(f) do { \
    switch(f) { \
        case GF: \
            frame_name = "GF"; \
            break; \
        case LF: \
            frame_name = "LF"; \
            break; \
        case TF: \
            frame_name = "TF"; \
            break; \
        default: \
            frame_name = "UNKNOWN"; \
    } \
} while(0)


char* convert_str(char *IFJStr, char* str)
{
    char tmpStr[10];

    if (IFJStr == NULL)
        return NULL;

    strcpy(IFJStr, "string@");

    size_t srcLen = strlen(str);
    for (size_t i = 0; i < srcLen; i++)
    {
        if (str[i] == '\"') {
            continue;
        }
        size_t len = strlen(IFJStr);
        if ((str[i] >= 0 && str[i] <= 32) || (str[i]==35))
        {
            sprintf(tmpStr, "\\%03d", (int)str[i]);
            strcat(IFJStr, tmpStr);
        }
        else if (str[i] == 36)
        {
            strcat(IFJStr, "$");
        }
        else if (str[i] == 37)
        {
            strcat(IFJStr, "%%");
        }
        else if (str[i] == 92) // '\'
        {
            if (i + 1 < srcLen)
            {
                i++;
                switch (str[i])
                {
                    case 'a':
                        strcat(IFJStr, "\\007");
                        break;
                    case 'b':
                        strcat(IFJStr, "\\008");
                        break;
                    case 't':
                        strcat(IFJStr, "\\009");
                        break;
                    case 'n':
                        strcat(IFJStr, "\\010");
                        break;
                    case 'v':
                        strcat(IFJStr, "\\011");
                        break;
                    case 'f':
                        strcat(IFJStr, "\\012");
                        break;
                    case 'r':
                        strcat(IFJStr, "\\013");
                        break;
                    case 'e':
                        strcat(IFJStr, "\\027");
                        break;
                    case '\'':
                        strcat(IFJStr, "\\039");
                        break;
                    case '\\':
                        strcat(IFJStr, "\\092");
                        break;
                    case '$':
                        strcat(IFJStr, "\\036");
                        break;
                    case '\"':
                        strcat(IFJStr, "\\034");
                        break;
                    default:
                        strcat(IFJStr, "\\");
                        IFJStr[len + 1] = str[i];
                        IFJStr[len + 2] = '\0';
                        break;
                }
            }
            else
                strcat(IFJStr, "\\092");
        }
        else
        {
            IFJStr[len] = str[i];
            IFJStr[len+1] = '\0';
        }
    }
    return IFJStr;
}

char *create_instr_string(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *instr = malloc(MAX_LINE_LENGTH * sizeof(char)); // Adjust size as needed
    if (instr != NULL) {
        vsnprintf(instr, MAX_LINE_LENGTH, format, args);
    }
    va_end(args);
    return instr;
}

void print_list(const instr_node *head) {
    const instr_node *current = head;
    int count = 0;
    
    while (current != NULL) {
        fprintf(stderr, "Node %d: %s", count, current->instr);
        current = current->next;
        count++;
    }
}

int create_file(FILE **file) {
    *file = fopen("IFJ23.code", "w");

    if (*file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void generate_header() {
  printf( "# IFJ dlya liudiej bez lichnoi zhizni\n");
  printf( ".IFJcode23\n");
  printf( "JUMP $$main");
}

void add_instr(instr_node **head, char *instr) {

    instr_node *new_node = malloc(sizeof(instr_node));
    if (new_node == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Error allocating memory in newnode\n");
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

// define global id, id_name must be in data.op.id_name
void CREATE_ID(instr_node **head, char *id_name, char *string, int deepness, Frame frame) {
  SET_FRAME(frame);
  // sprintf(string, "# declare a var\nDEFVAR %s@%s_%d\n", frame_name, id_name, deepness);
  sprintf(string, "DEFVAR %s@%s_%d\n", frame_name, id_name, deepness);
  add_instr(head, string);
}

// move int value in data.op.int_val to data.op.id_name
void MOVE(instr_node **head, char *id_name, char *value, char *string, int deepness, Frame frame, token_type type){
  SET_FRAME(frame);
  char *type_string;
  if (type == T_INT) {
        type_string = "int";
  } else if (type == T_SING_STRING) {
        type_string = "string";
  } else if (type == T_DOUBLE) {
        type_string = "float";
  } else {
        type_string = "unknown";
  }

  if (type == T_DOUBLE) {
    float val = atof(value);
    sprintf(string,"# assign\nMOVE %s@%s_%d %s@%a\n", frame_name, id_name, deepness, type_string, val);
  }
  else
    sprintf(string,"# assign\nMOVE %s@%s_%d %s@%s\n", frame_name, id_name, deepness, type_string, value);
  
  add_instr(head, string);
}

void WHILE_START(instr_node **head, char *string, int deepness, int while_cnt) {
  string = "\n# while {\n";
  add_instr(head, string);
  char *instr = create_instr_string("LABEL $WHILE_START_d%d_c%d\n", deepness, while_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

void WHILE_CHECK(instr_node **head, char *string, int deepness, int while_cnt) {
  string = "\n# while (res) \nPOPS GF@%%res\n";
  add_instr(head, string);
  char *instr = create_instr_string("JUMPIFNEQ $WHILE_END_d%d_c%d GF@%%%%res bool@true\n# }\n\n", deepness, while_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

void WHILE_END(instr_node **head, char *string, int deepness, int while_cnt) {
  char *instr = create_instr_string("JUMP $WHILE_START_d%d_c%d\n# }\n\n", deepness, while_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
  instr = create_instr_string("LABEL $WHILE_END_d%d_c%d\n", deepness, while_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

// assign value from the stack to data.op.id_name
void ASSIGN(instr_node **head, char *id_name, char *string, int deepness, Frame frame) {
  SET_FRAME(frame);
  sprintf(string, "POPS %s@%s_%d\n", frame_name, id_name, deepness);
  add_instr(head, string);

  char *instr = create_instr_string("CLEARS\n\n");
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

// write value of data.op.id_name to stdout
void WRITE(instr_node **head, char *id_name, char *string, int deepness, Frame frame, DataType type, char *value) {
  SET_FRAME(frame);
  char *string_to_convert = malloc(MAX_LINE_LENGTH * sizeof(char));
  if(type == TYPE_UNKNOWN)
    sprintf(string, "\n# write(...)\nWRITE %s@%s_%d\n", frame_name, id_name, deepness);
  else if(type == TYPE_DOUBLE)
    sprintf(string, "\n# write(...)\nWRITE %s@%a\n", type_to_string(type), atof(id_name));
  else if(type == TYPE_NIL)
    sprintf(string, "\n# write(...)\nWRITE nil@nil\n");
  else if(type == TYPE_STRING)
    sprintf(string, "\n# write(...)\nWRITE %s\n", convert_str(string_to_convert, value));
  else
    sprintf(string, "\n# write(...)\nWRITE %s@%s\n", type_to_string(type), value);
  add_instr(head, string);
}

// begin expression, clear stack
void BEGIN_EXPR(instr_node **head, char *string) {
  string = "CLEARS\n";
  add_instr(head, string);
}

// push value of data.op.id_name to stack
// void PUSH(instr_node **head, char *id_name, char *string, int deepness, Frame frame) {
//   SET_FRAME(frame);
//   sprintf(string, "PUSHS %s@%s_%d\n", frame_name, id_name, deepness);
//   add_instr(head, string);
// }
void PUSH(instr_node **head, char *id_name, char *string, int deepness, Frame frame, DataType type, char *value) {
    SET_FRAME(frame);
    char *type_string;
    type_string = type_to_string(type);
    char *string_to_convert = malloc(MAX_LINE_LENGTH * sizeof(char));
    if(type == TYPE_UNKNOWN)
        sprintf(string, "PUSHS %s@%s_%d\n", frame_name, id_name, deepness);
    else if(type == TYPE_DOUBLE) {
        float val = atof(value);
        sprintf(string, "PUSHS %s@%a\n", type_string, val);
    }
    else if(type == TYPE_NIL)
        sprintf(string, "PUSHS nil@nil\n");
    else if(type == TYPE_STRING)
        sprintf(string, "PUSHS %s\n", convert_str(string_to_convert, value));
    else{
        sprintf(string, "PUSHS %s@%s\n", type_string, value);
    }

    add_instr(head, string);
}

// add two values from stack (top two) and push result to stack
void ADD(instr_node **head, char *string) {
  string = "ADDS\n";
  add_instr(head, string);
}

// subtract two values from stack (top two) and push result to stack
void SUB(instr_node **head, char *string) {
  string = "SUBS\n";
  add_instr(head, string);
}

void CONCAT(instr_node **head, char *string) {
    string = "POPS GF@?temp_1\n";
    add_instr(head, string);
    string = "POPS GF@?temp_2\n";
    add_instr(head, string);
    string = "CONCAT GF@&str GF@&str GF@?temp_2\n";
    add_instr(head, string);
    string = "CONCAT GF@&str GF@&str GF@?temp_1\n";
    add_instr(head, string);
    string = "PUSHS GF@&str\n";
    add_instr(head, string);
}

// multiply two values from stack (top two) and push result to stack
void MUL(instr_node **head, char *string) {
  string = "MULS\n";
  add_instr(head, string);
}

// divide two values from stack (top two) and push result to stack
void DIV(instr_node **head, char *string) {
  string = "DIVS\n";
  add_instr(head, string);
}

void IDIV(instr_node **head, char *string) {
    string = "IDIVS\n";
    add_instr(head, string);
}

void LT(instr_node **head, char *string) {
    string = "LTS\n";
    add_instr(head, string);
}

void GT(instr_node **head, char *string) {
    string = "GTS\n";
    add_instr(head, string);
}

void EQ(instr_node **head, char *string) {
    string = "EQS\n";
    add_instr(head, string);
}

void AND(instr_node **head, char *string) {
    string = "ANDS\n";
    add_instr(head, string);
}

void OR(instr_node **head, char *string) {
    string = "ORS\n";
    add_instr(head, string);
}

void NOT(instr_node **head, char *string) {
    string = "NOTS\n";
    add_instr(head, string);
}

void INT2FLOAT(instr_node **head, char *string) {
    string = "INT2FLOATS\n";
    add_instr(head, string);
}

void FLOAT2INT(instr_node **head, char *string) {
    string = "FLOAT2INTS\n";
    add_instr(head, string);
}

void INT2CHAR(instr_node **head, char *string) {
    string = "INT2CHARS\n";
    add_instr(head, string);
}

void STRI2INT(instr_node **head, char *string) {
    string = "STRI2INTS\n";
    add_instr(head, string);
}

void MAIN(instr_node **head, char *string) {
  string = "\n";
  add_instr(head, string);
  string = "LABEL $$main\n";
  add_instr(head, string);
  string = "DEFVAR GF@%%retval_main\n";
  add_instr(head, string);
    string = "DEFVAR GF@?temp_1\n";
    add_instr(head, string);
    string = "DEFVAR GF@?temp_2\n";
    add_instr(head, string);
    string = "DEFVAR GF@&str\n";
    add_instr(head, string);
    string = "MOVE GF@&str string@\n";
    add_instr(head, string);
  string = "CREATEFRAME\n";
  add_instr(head, string);
  string = "PUSHFRAME\n";
  add_instr(head, string);
  string = "JUMP **main_declares**\n";
  add_instr(head, string);
  string = "LABEL **main_declares_return**\n";
  add_instr(head, string);
}

void FUNC_START(instr_node **head, char *func_name, char *string) {
  sprintf(string, "\n\nLABEL $%s\n", func_name);
  add_instr(head, string);
  string = "PUSHFRAME\n";
  add_instr(head, string);
  string = "DEFVAR LF@%%retval\n";
  add_instr(head, string);
  string = "MOVE LF@%%retval nil@nil\n";
  add_instr(head, string);
  char *instr = create_instr_string("JUMP **%s_declares**\n", func_name);
  if (instr != NULL) 
    add_instr(head, instr);
  instr = create_instr_string("LABEL **%s_declares_return**\n", func_name);
  if (instr != NULL) 
    add_instr(head, instr);
}

void CREATEFRAME(instr_node **head, char *string) {
  string = "CREATEFRAME\n";
  add_instr(head, string);
}

void PUSHFRAME(instr_node **head, char *string) {
  string = "PUSHFRAME\n";
  add_instr(head, string);
}

void FUNC_CALL(instr_node **head, char *func_name, Operand *func_param, unsigned int func_param_count, char *string) {
  sprintf(string, "\nCREATEFRAME\n");
  add_instr(head, string);

  for (int i = 0; i < func_param_count; i++) {
    char *instr = create_instr_string("DEFVAR TF@%s\n", func_param[i].id_name);
    if (instr != NULL) {
        add_instr(head, instr);
    }

    instr = create_instr_string("MOVE TF@%s_0 int@%s\n# ..., %s,\n", func_param[i].id_name, func_param[i].val, func_param[i].id_name);
    if (instr != NULL) {
        add_instr(head, instr);
    }
  }

  char *instr = create_instr_string("CALL $%s\n# %s(...)\n", func_name, func_name);
  if (instr != NULL) {
    add_instr(head, instr);
  }
}

void FUNC_END(instr_node **head, char* retval, char *string) {
  string = "POPFRAME\n";
  add_instr(head, string);
  string = "RETURN\n# }\n";
  add_instr(head, string);
}

void POP_TMP(instr_node **head, char *string, int deepness) {
    sprintf(string, "POPS %s@?%s_%d\n", "GF", "temp", deepness);
    add_instr(head, string);
}

void PUSH_TMP(instr_node **head, char *string, int deepness) {
    sprintf(string, "PUSHS %s@?%s_%d\n", "GF", "temp", deepness);
    add_instr(head, string);
}

//CALL ONCE IN MAIN
void IF_START(instr_node **head, char *string, int deepness) {
  string = "DEFVAR GF@%%res\n\n";
  add_instr(head, string);
}

void IF_CHECK(instr_node **head, char *string, int deepness, int else_cnt) {
  string = "# if (res) \nPOPS GF@%%res\n";
  add_instr(head, string);

  char *instr = create_instr_string("JUMPIFNEQ $IF_ELSE_d%d_c%d GF@%%%%res bool@true\n# {\n", deepness, else_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

void IF_END(instr_node **head, char *string, int deepness, int if_cnt) {
  char *instr = create_instr_string("JUMP $IF_END_d%d_c%d\n# }\n\n", deepness, if_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

void ELSE_START(instr_node **head, char *string, int deepness, int else_cnt) {
  char *instr = create_instr_string("# else { \nLABEL $IF_ELSE_d%d_c%d\n", deepness, else_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}

void ELSE_IF_END(instr_node **head, char *string, int deepness, int if_cnt) {
  char *instr = create_instr_string("\nLABEL $IF_END_d%d_c%d\n# }\n\n", deepness, if_cnt);
  if (instr != NULL) {
      add_instr(head, instr);
  }
}


void EXIT(instr_node **head, char *string) {
  string = "\n# EXIT \nPOPFRAME\nEXIT int@0\n\n\n";
  add_instr(head, string);
}

void BUILTIN(instr_node **head, char *string) {
  //WRITE
  char *instr = create_instr_string("\n\n# Write function\nLABEL $%%write\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("WRITE LF@%%param\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of write ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);

  //INT2DOUBLE
  instr = create_instr_string("\n# Int2Double function\nLABEL $%%int2float\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("PUSHFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%retval\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("INT2FLOAT LF@%%retval LF@%%param\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("POPFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of int2float ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);

  //DOUBLE2INT
  instr = create_instr_string("\n# Double2Int function\nLABEL $%%double2int\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("PUSHFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%retval\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("FLOAT2INT LF@%%retval LF@%%param\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("POPFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of double2int ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);

  //LENGTH
  instr = create_instr_string("\n# Length function\nLABEL $%%length\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("PUSHFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%retval\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("STRLEN LF@%%retval LF@%%param\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("POPFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of length ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);

  //READSTRING
  instr = create_instr_string("\n# ReadString function\nLABEL $%%readString\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("CREATEFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("PUSHFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%retval\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%string_tmp\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%bool\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%len\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("READ LF@%%string_tmp string\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("STRLEN LF@%%len LF@%%string_tmp\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("EQ LF@%%bool LF@%%len int@0\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("JUMPIFEQ $%%eq_to_zero_readstring LF@%%bool bool@true\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("MOVE LF@%%retval LF@%%string_tmp\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("JUMP $%%readString_end\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("LABEL $%%eq_to_zero_readstring\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("MOVE LF@%%retval nil@nil\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("LABEL $%%readString_end\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("POPFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of readString ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);

    //SUBSTR
  // instr = create_instr_string("\n# Substr function\nLABEL $substr\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("PUSHFRAME\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%retval\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%tmp\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%tmp2\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%tmp3\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("MOVE LF@%%tmp int@0\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("MOVE LF@%%tmp2 int@0\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("MOVE LF@%%tmp3 int@0\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("LT LF@%%tmp3 LF@param2 int@0\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("JUMPIFEQ $substr_end LF@%%tmp3 bool@true\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("LT LF@%%tmp3 LF@param3 int@0\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("JUMPIFEQ $substr_end LF@%%tmp3 bool@true\n");

  // CHR
  // instr = create_instr_string("\n# Chr function\nLABEL $chr\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("PUSHFRAME\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%retval\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);
  // instr = create_instr_string("DEFVAR LF@%%tmp\n");
  // if (instr != NULL) 
  //     add_instr(head, instr);

  //ORD
  instr = create_instr_string("\n# Ord function\nLABEL $%%ord\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("PUSHFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%retval\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%strlen\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("STRLEN LF@%%strlen LF@%%param\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("DEFVAR LF@%%bool\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("EQ LF@%%bool LF@%%strlen int@0\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("JUMPIFEQ $%%eq_to_zero LF@%%bool bool@true\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("STRI2INT LF@%%retval LF@%%param int@0\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("JUMP $%%ord_end\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("LABEL $%%eq_to_zero\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("MOVE LF@%%retval int@0\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("LABEL $%%ord_end\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("POPFRAME\n");
  if (instr != NULL) 
      add_instr(head, instr);
  instr = create_instr_string("RETURN\n# end of ord ---------\n");
  if (instr != NULL) 
      add_instr(head, instr);



}

int generate_code(instr_node **head, Data data, gencode gencode, int deepness, Frame frame) {
  
  char *string = malloc(MAX_LINE_LENGTH * sizeof(char)); 
  if (string == NULL) {
      // Handle memory allocation error
      return EXIT_FAILURE;
  }

  int ifelse_cnt = data.ifelse_cnt;
  int while_cnt = data.while_cnt;

  switch (gencode) {

    char *id_name_create;
    char *id_name_assign;
    char *id_name_write;
    char *id_name_push;
    char *retval;
    char *id_name_move;
    char *func_name;
    char *val;
    token_type type;

    case GEN_WHILE_END:
        WHILE_END(head, string, deepness, while_cnt);
        break;

    case GEN_WHILE_CHECK:
        WHILE_CHECK(head, string, deepness, while_cnt);
        break;

    case GEN_WHILE_START:
        WHILE_START(head, string, deepness, while_cnt);
        break;

    // Operand func_param[10];
    case GEN_BUILTIN:
        BUILTIN(head, string);
        break;

    case GEN_EXIT:
        EXIT(head, string);
        break;

    case GEN_CREATEFRAME:
        CREATEFRAME(head, string);
        break;

    case GEN_PUSHFRAME:
        PUSHFRAME(head, string);
        break;

    case GEN_IF_START:
        IF_START(head, string, deepness);
        break;

    case GEN_IF_CHECK:
        IF_CHECK(head, string, deepness, ifelse_cnt);
        break;

    case GEN_IF_END:
        IF_END(head, string, deepness, ifelse_cnt);
        break;

    case GEN_ELSE_START:
        ELSE_START(head, string, deepness, ifelse_cnt);
        break;

    case GEN_PUSH:
        val = data.op.val;
        id_name_push = data.op.id_name;
        type = data.op.type;
        PUSH(head, id_name_push, string, deepness, frame, type, val);
        break;

    case GEN_PUSH_TMP:
        PUSH_TMP(head, string, deepness);
        break;

    case GEN_ELSE_IF_END:
        ELSE_IF_END(head, string, deepness, ifelse_cnt);
        break;

    case GEN_CONCAT:
        CONCAT(head, string);
        break;

    // define data.op.id_name to global
    case GEN_CREATE_ID:
        // char *id_name_create;
        id_name_create = data.op.id_name;
        CREATE_ID(head, id_name_create, string, deepness, frame);
        break;
    
    // moves int value in data.op.int_val to data.op.id_name
    case GEN_MOVE:
        val = data.op.val;
        id_name_move = data.op.id_name;
        type = data.op.type;
        MOVE(head, id_name_move, val, string, deepness, frame, type);
        break;
    
    // pop value from expression to data.op.id_name
    case GEN_ASSIGN:
        // char *id_name_assign;
        id_name_assign = data.op.id_name;
        ASSIGN(head, id_name_assign, string, deepness, frame);
        break;
    
    // write value of data.op.id_name
    case GEN_WRITE:
        val = data.op.val;
        id_name_write = data.op.id_name;
        type = data.op.type;
        WRITE(head, id_name_write, string, deepness, frame, type, val);
        break;
    
    // begin expression, clear stack
    case GEN_BEGIN_EXPR:
        BEGIN_EXPR(head, string);
        break;

    // add two values from stack (top two) and push result to stack
    case GEN_ADD:
        ADD(head, string);
        break;

    // subtract two values from stack (top two) and push result to stack
    case GEN_SUB:
        SUB(head, string);
        break;

    // multiply two values from stack (top two) and push result to stack
    case GEN_MUL:
        MUL(head, string);
        break;

    case GEN_IDIV:
        IDIV(head, string);
        break;

    case GEN_LT:
        LT(head, string);
        break;

    case GEN_GT:
        GT(head, string);
        break;

    case GEN_EQ:
        EQ(head, string);
        break;

    case GEN_AND:
        AND(head, string);
        break;

    case GEN_OR:
        OR(head, string);
        break;

    case GEN_NOT:
        NOT(head, string);
        break;

    case GEN_INT2FLOAT:
        INT2FLOAT(head, string);
        break;

    case GEN_FLOAT2INT:
        FLOAT2INT(head, string);
        break;

    case GEN_INT2CHAR:
        INT2CHAR(head, string);
        break;

    case GEN_STRI2INT:
        STRI2INT(head, string);
        break;

    case GEN_POP_TMP:
        POP_TMP(head, string, deepness);
        break;
    // generate main function
    case GEN_MAIN:
        MAIN(head, string);
        break;
    
    // divide two values from stack (top two) and push result to stack
    case GEN_DIV:
        DIV(head, string);
        break;

    // generate function start
    case GEN_FUNC_START:
        // char *func_name;
        func_name = data.func_name;
        FUNC_START(head, func_name, string);
        break;
    // generate function end
    case GEN_FUNC_END:
        retval = data.op.id_name;
        FUNC_END(head, retval, string);
        break;

    // generate function call
    case GEN_FUNC_CALL:
        func_name = data.func_name;
        FUNC_CALL(head, func_name, data.func_param, data.func_param_count ,string);
        break;

    // Add default case to handle unexpected values
    default:
        // Handle unexpected case
        break;
  }

  return EXIT_SUCCESS;
}

void pop_list_to_file(instr_node **head) {
  FILE *file = fopen("IFJ23.code", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  instr_node *current = *head;
  int cnt = 6;
  while (current != NULL) {
    printf( "%s", current->instr);
    cnt++;
    instr_node *tmp = current;
    current = current->next;
    // free(tmp->instr);
    tmp->instr = NULL;
    free(tmp);
  }
  fclose(file);
}

void destroy_file() {
  remove("IFJ23.code"); 
}


Data init_data() {
  Data data;
  data.func_name = NULL;
  data.op.type = TYPE_UNKNOWN;
  data.op.id_name = NULL;
  data.op2.id_name = NULL;
  return data;
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

  fclose(file);

  return EXIT_SUCCESS;
}

// Function to initialize instr_list_dynamic
instr_list_dynamic *init_instr_list_dynamic() {
    instr_list_dynamic *new_list = malloc(sizeof(instr_list_dynamic));
    if (new_list == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    new_list->size = 0;
    new_list->lists = NULL; // Start with no lists
    return new_list;
}

// Function to add a new linked list to instr_list_dynamic
int add_new_linked_list(instr_list_dynamic *list, char *name) {
    if (list == NULL) {
        return -1; // Error: list is NULL
    }

    // Resize the array to accommodate one more list
    int new_size = list->size + 1;
    instr_node **new_lists = realloc(list->lists, new_size * sizeof(instr_node*));
    if (new_lists == NULL) {
        return -1; // Error: realloc failed
    }

    // Allocate memory for the new head node
    instr_node *new_head = malloc(sizeof(instr_node));
    if (new_head == NULL) {
        return -1; // Error: malloc failed
    }

    // Allocate memory and set the name for the new head node
    new_head->name_of_llist = strdup(name); // 'strdup' allocates memory and copies the string
    new_head->instr = NULL;
    new_head->next = NULL;

    new_head->declarations = NULL;

    char *func_name_dec = malloc(MAX_LINE_LENGTH * sizeof(char));
    sprintf(func_name_dec, "\n\nLABEL **%s_declares**\n", name);
    fprintf(stderr, "func_name_dec: %s\n", func_name_dec);
    add_instr(&new_head->declarations, func_name_dec);
    // new_head.


    // Update the list structure
    list->lists = new_lists;
    list->lists[list->size] = new_head; // Set the new list head
    list->size = new_size;
    return 0; // Success
}

char *type_to_string(DataType type) {
       switch (type) {
           case TYPE_INT:
               return "int";
           case TYPE_DOUBLE:
               return "float";
           case TYPE_STRING:
               return "string";
           case TYPE_NIL:
               return "nil";
           case TYPE_INT_NULLABLE:
               return "int";
           case TYPE_DOUBLE_NULLABLE:
               return "float";
           case TYPE_STRING_NULLABLE:
               return "string";
           case TYPE_VOID:
               return "void";
           case FUNC:
               return "func";
           case TYPE_UNKNOWN:
               return "unknown";
           case TYPE_BOOL:
               return "bool";
           default:
               return "unknown";
       }
}

// Function to search the linked list in instr_list_dynamic by the name of the linked list
instr_node *search_by_name_in_list(instr_list_dynamic *list, char *name, instr_node *main_node) {
    if (list == NULL || name == NULL) {
        return main_node; // Handle NULL pointers
    }

    // Iterate over the array of list heads
    for (int i = 0; i < list->size; ++i) {
        instr_node *current = list->lists[i];
        // Traverse the linked list
        while (current != NULL && current->name_of_llist != NULL) {
            if (strcmp(current->name_of_llist, name) == 0) {
                return current; // Found the list with the matching name
            }
            current = current->next;

        }
    }
    return main_node; // List with the given name not found
}

void pop_all_lists_to_file(instr_list_dynamic *list) {
    FILE *file = fopen("IFJ23.code", "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if (list == NULL) {
        fclose(file);
        return; // Handle NULL pointer for list
    }

    for (int i = 0; i < list->size; i++) {
        instr_node *current = list->lists[i];
        instr_node *declares = list->lists[i]->declarations;
        // Check if the list is not empty
        if (current != NULL) {
            // Free the head node's name_of_llist if necessary
            // free(current->name_of_llist);
            // Process the rest of the list
            current = current->next;
            while (current != NULL) {
                if (current->instr != NULL) {
                    printf("%s", current->instr); // Write instruction to file
                    // free(current->instr);
                }
                instr_node *tmp = current;
                current = current->next;
                free(tmp);
            }
            // Free the head node itself
            free(list->lists[i]);
        }

        if (declares != NULL) {
            // Free the head node's name_of_llist if necessary
            // free(current->name_of_llist);
            // Process the rest of the list
            while (declares != NULL) {
                if (declares->instr != NULL) {
                    printf("%s", declares->instr); // Write instruction to file
                    // free(current->instr);
                }
                instr_node *tmp = declares;
                declares = declares->next;
                free(tmp);
            }
            // Free the head node itself
            // free(list->lists[i]->declarations);
        }

        list->lists[i] = NULL;
    }

    fclose(file);
}

void print_list_names(instr_list_dynamic *list) {
    if (list == NULL) {
        return; // Handle NULL pointer
    }

    for (int i = 0; i < list->size; i++) {
        instr_node *current = list->lists[i];

        if (current != NULL && current->name_of_llist != NULL) {
            fprintf(stderr, "List Name: %s\n", current->name_of_llist);
        }

    }
}

bool is_substr_in_list(instr_node *head, const char *substr) {
    instr_node *current = head;

    while (current != NULL) {
        if (strstr(current->instr, substr) != NULL) {
            return true;  // Found the substring
        }
        current = current->next;
    }

    return false;  // Substring not found
}