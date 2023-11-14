#include <stdio.h>
#include <stdlib.h>

#include "parser.c"

typedef struct
{
    char *op;
    char *arg1;
    char *arg2;
    char *result;
} IRInstruction;

void generateIR(ASTNode *node);
void emit(char *op, char *arg1, char *arg2, char *result);

void emit(char *op, char *arg1, char *arg2, char *result)
{
    printf("%s %s %s %s\n", op, arg1 ? arg1 : "", arg2 ? arg2 : "", result ? result : "");
}

void generateIR(ASTNode *node)
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
    case NODE_FUNCTION:
        printf("Function: %s\n", node->data.function.name);

        if (node->children_count > 0)
        {
            generateIR(node->children[0]);

            for (int i = 1; i < node->children_count; i++)
            {
                generateIR(node->children[i]);
            }
        }
        break;

    case NODE_VAR_DECL:
        emit("alloc", node->data.var_decl.name, NULL, NULL);
        if (node->data.var_decl.value != NULL)
        {
            generateIR(node->data.var_decl.value);
            emit("store", "temp_var", NULL, node->data.var_decl.name);
        }
        break;

    case NODE_MAIN:
        printf("Function: main\n");

        for (int i = 0; i < node->children_count; i++)
        {
            generateIR(node->children[i]);
        }

        emit("end_main", NULL, NULL, NULL);

        break;

    case NODE_FUNCTION_CALL:
        if (node->children_count >= 1)
        {
            generateIR(node->children[0]);

            char param_list[1024] = "";

            if (node->children_count == 2)
            {
                for (int i = 0; i < node->children[1]->children_count; i++)
                {
                    generateIR(node->children[1]->children[i]);

                    strcat(param_list, "temp_param");
                    if (i < node->children[1]->children_count - 1)
                    {
                        strcat(param_list, ", ");
                    }
                }
            }

            emit("call_function", "temp_func_name", param_list, "temp_result");
        }
        else
        {
            printf("Error: Incomplete function call structure.\n");
        }
        break;

    case NODE_IF_STATEMENT:
        if (node->children_count >= 2)
        {
            generateIR(node->children[0]);
            emit("if_false", "temp_condition", "label_else", NULL);

            generateIR(node->children[1]);

            if (node->children_count > 2 && node->children[2]->type == NODE_ELSE_STATEMENT)
            {
                emit("goto", "label_end_if", NULL, NULL);
                emit("label", "label_else", NULL, NULL);
                generateIR(node->children[2]->children[0]);
            }
            else
            {
                emit("label", "label_else", NULL, NULL);
            }

            emit("label", "label_end_if", NULL, NULL);
        }
        else
        {
            printf("Error: Incomplete if statement structure.\n");
        }
        break;

    case NODE_FOR_LOOP:
        generateIR(node->data.for_loop.start_expr);

        emit("label", "loop_start", NULL, NULL);

        generateIR(node->data.for_loop.end_expr);
        emit("if_false", "temp_condition", "loop_end", NULL);

        if (node->children_count > 0)
        {
            for (int i = 0; i < node->children_count; i++)
            {
                generateIR(node->children[i]);
            }
        }

        emit("add", node->data.for_loop.var_name, NULL, NULL);
        emit("goto", "loop_start", NULL, NULL);

        emit("label", "loop_end", NULL, NULL);
        break;

    case NODE_RETURN_STATEMENT:

        generateIR(node->data.return_statement.expression);
        emit("return", NULL, NULL, NULL);
        break;

    case NODE_LITERAL:

        emit("load_const", node->data.literal.value, NULL, "temp_literal");
        break;

    case NODE_IDENTIFIER:

        emit("load", node->data.identifier.name, NULL, "temp_var");
        break;

    case NODE_EXPRESSION_LIST:

        for (int i = 0; i < node->data.expression_list.count; i++)
        {
            generateIR(node->data.expression_list.items[i]);
        }
        break;

    case NODE_ARGUMENT_LIST:

        for (int i = 0; i < node->children_count; i++)
        {
            generateIR(node->children[i]);

            emit("arg", "temp_param", NULL, NULL);
        }
        break;

    case NODE_KEY_VALUE_PAIR:

        if (node->children_count >= 2)
        {
            generateIR(node->children[0]);
            generateIR(node->children[1]);

            emit("key_value_pair", "temp_key", "temp_value", "temp_pair");
        }
        else
        {
            printf("Error: Key-value pair node does not have two children.\n");
        }
        break;

    case NODE_ARRAY_ACCESS:
        generateIR(node->data.array_access.array);
        generateIR(node->data.array_access.index);
        emit("array_access", "temp_array", "temp_index", "temp_element");
        break;

    case NODE_OPERATOR:
        emit("operator", node->data.operator_node.op, NULL, NULL);
        break;

    case NODE_INT:

        emit("load_const", node->data.int_node.value, NULL, "temp_int");
        break;

    case NODE_FLOAT:

        emit("load_const", node->data.float_node.value, NULL, "temp_float");
        break;

    case NODE_STRING:

        emit("load_const", node->data.string_node.value, NULL, "temp_string");
        break;

    case NODE_CHAR:

        emit("load_const", node->data.char_node.text, NULL, "temp_char");
        break;
    default:

        break;
    }

    for (int i = 0; i < node->children_count; i++)
    {
        generateIR(node->children[i]);
    }
}

// // 测试输入
// int main()
// {
//     if (freopen("input.txt", "r", stdin) == NULL)
//     {
//         fprintf(stderr, "Error opening file\n");
//         return 1;
//     }
//     int fsize = 1000;
//     char source[1000] = {0};

//     for (long i = 0; i < fsize; i++)
//     {
//         int c = getchar();
//         if (c == EOF)
//         {
//             break;
//         }
//         source[i] = c;
//     }
//     source[fsize] = 0;

//     int token_count = 0;
//     Token *tokens = lexer(source, &token_count);
//     ASTNode *root = parse_program(&tokens, token_count);
//     freopen("output_pseudo.txt", "w", stdout);
//     generateIR(root);
// }
