#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.c"

typedef enum
{
    NODE_PROGRAM, // 程序节点

    NODE_MAIN,       // 主函数节点
    NODE_FUNCTION,   // 函数节点
    NODE_PARAM_LIST, // 参数列表节点

    NODE_STATEMENT,        // 语句节点
    NODE_VAR_DECL,         // 变量声明节点
    NODE_ASSIGNMENT,       // 赋值语句节点
    NODE_IF_STATEMENT,     // if 语句节点
    NODE_ELSE_STATEMENT,   // else 语句节点
    NODE_FOR_LOOP,         // for循环节点
    NODE_RETURN_STATEMENT, // 返回语句节点
    NODE_FUNCTION_CALL,    // 函数调用节点
    NODE_ARRAY_DECL,       // 数组语句节点
    NODE_KEY_VALUE_DECL,   // 键值语句节点

    NODE_EXPRESSION_LIST, // 表达式列表节点
    NODE_ARGUMENT_LIST,   // 参数列表
    NODE_KEY_VALUE_PAIR,  // 键值对节点
    NODE_EXPRESSION,      // 表达式节点
    NODE_ARRAY_ACCESS,    // 数组下标节点

    NODE_LITERAL,    // 字面量节点
    NODE_OPERATOR,   // 操作符节点
    NODE_IDENTIFIER, // 标识符节点
    NODE_INT,        // 整数节点
    NODE_FLOAT,      // 浮点数节点
    NODE_STRING,     // 字符串节点
    NODE_LETTER,     // 字母节点
    NODE_DIGIT,      // 数字节点
    NODE_CHAR,       // 字符节点
    NODE_ARG_LIST    // 实参列表节点
} NodeType;

typedef struct ASTNode ASTNode;

typedef struct
{
    char *name;
    ASTNode *param_list;
    ASTNode *body;
} FunctionNode;

typedef struct
{
    char *name;
    ASTNode *value;
} VarDeclNode;

typedef struct
{
    ASTNode *condition;
    ASTNode *true_branch;
    ASTNode *false_branch;
} IfStatementNode;

typedef struct
{
    char *var_name;
    ASTNode *start_expr;
    ASTNode *end_expr;
    ASTNode *body;
} ForLoopNode;

typedef struct
{
    ASTNode *expression;
} ReturnStatementNode;

typedef struct
{
    char *name;
    ASTNode *arg_list;
} FunctionCallNode;

typedef struct
{
    ASTNode *left;
    ASTNode *right;
    char *op;
} ExpressionNode;

typedef struct
{
    char *value;
} LiteralNode;

typedef struct
{
    ASTNode **items;
    int count;
} ExpressionListNode;

typedef struct
{
    ASTNode **items;
    int count;
} ArgumentListNode;

typedef struct
{
    char *name;
    ASTNode *list;
} CollectionNode;

typedef struct
{
    char *value;
} IntNode;

typedef struct
{
    char *value;
} FloatNode;

typedef struct
{
    char *value;
} StringNode;

typedef struct
{
    char *name;
} IdentifierNode;

typedef struct
{
    char *op;
} OperatorNode;

typedef struct
{
    ASTNode **items;
    int count;
} ListNode;

typedef struct
{
    ASTNode *key;
    ASTNode *value;
} KeyValuePairNode;

typedef struct
{
    ASTNode *condition;
    ASTNode *body;
} LoopOrIndexNode;

typedef struct
{
    char *text;
} CharNode;

typedef struct
{
    ASTNode *branch;
} ElseStatementNode;

typedef struct
{
    ASTNode *array;
    ASTNode *index;
} ArrayAccessNode;

typedef union
{
    FunctionNode function;
    VarDeclNode var_decl;
    IfStatementNode if_statement;
    ElseStatementNode else_statement;
    ForLoopNode for_loop;
    ReturnStatementNode return_statement;
    FunctionCallNode function_call;
    ExpressionNode expression;
    LiteralNode literal;
    IdentifierNode identifier;
    ExpressionListNode expression_list;
    KeyValuePairNode key_value_pair;
    CollectionNode collection;
    IntNode int_node;
    FloatNode float_node;
    StringNode string_node;
    OperatorNode operator_node;
    ListNode list_node;
    LoopOrIndexNode loop_or_index_node;
    CharNode char_node;
    ArrayAccessNode array_access;
} ASTNodeData;

// AST节点结构体
struct ASTNode
{
    NodeType type;      // 节点类型
    ASTNodeData data;   // 节点数据
    ASTNode **children; // 子节点数组
    int children_count; // 子节点数量
};

ASTNode *parse_program(Token **tokens, int token_count);
ASTNode *parse_main(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_function(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_param_list(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_statement(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_assignment_or_function_call(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_if_statement(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_return_statement(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_for_loop(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_assignment(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_function_call(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_expression(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_compound_statement(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_var_decl(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_array_decl(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_key_value_decl(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_expression_list(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_key_value_pair(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_arg_list(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_literal(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_operator(Token **tokens, int token_count, int *current_token_index);
ASTNode *parse_identifier(Token **tokens, int token_count, int *current_token_index);

ASTNode *create_node(NodeType type)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->type = type;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

void add_child(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }
    parent->children = realloc(parent->children, sizeof(ASTNode *) * (parent->children_count + 1));
    if (parent->children == NULL)
    {
        return;
    }

    parent->children[parent->children_count] = child;
    parent->children_count++;
}

// 释放函数
void free_ast(ASTNode *node)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < node->children_count; i++)
    {
        free_ast(node->children[i]);
    }

    free(node->children);
    free(node);
}

// 消耗函数
void consume(Token **tokens, int token_count, int *current_token_index, TokenType type)
{
    if (*current_token_index >= token_count)
    {
        printf("Error: Token index (%d) out of range (count: %d)\n", *current_token_index, token_count);
        exit(1);
        return;
    }

    Token *currentToken = &((*tokens)[*current_token_index]);

    if (currentToken == NULL)
    {
        printf("Error: Token at index %d is NULL\n", *current_token_index);
        exit(1);
        return;
    }

    if (currentToken->type == type)
    {
        (*current_token_index)++;
    }
    else
    {
        printf("Syntax error: Expected token type %d, but found %d\n", type, currentToken->type);
        exit(1);
    }
}

// 跳过函数
void advance(Token **tokens, int *current_token_index)
{
    (*current_token_index)++;
}

// 解析 program
ASTNode *parse_program(Token **tokens, int token_count)
{
    ASTNode *program_node = create_node(NODE_PROGRAM);
    int current_token_index = 0;

    while (current_token_index < token_count)
    {
        if ((*tokens)[current_token_index].type == TOKEN_FUNCTION)
        {
            add_child(program_node, parse_function(tokens, token_count, &current_token_index));
        }
        else if ((*tokens)[current_token_index].type == TOKEN_MAIN)
        {
            add_child(program_node, parse_main(tokens, token_count, &current_token_index));
        }
        else if (((*tokens)[current_token_index].type == TOKEN_IDENTIFIER) && ((*tokens)[current_token_index + 2].type == TOKEN_LEFT_SQUARE_BRACKET))
        {
            add_child(program_node, parse_array_decl(tokens, token_count, &current_token_index));
        }
        else if (((*tokens)[current_token_index].type == TOKEN_IDENTIFIER) && ((*tokens)[current_token_index + 2].type == TOKEN_LEFT_CURLY_BRACE))
        {
            add_child(program_node, parse_key_value_decl(tokens, token_count, &current_token_index));
        }
        else if ((*tokens)[current_token_index].type == TOKEN_IDENTIFIER)
        {
            add_child(program_node, parse_var_decl(tokens, token_count, &current_token_index));
        }
        else
        {
            printf("Syntax error: Unexpected token at the program level.\n");
            free_ast(program_node);
            exit(1);
            return NULL;
        }
    }

    return program_node;
}

ASTNode *parse_main(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *main_node = create_node(NODE_MAIN);

    // 消耗 main
    consume(tokens, token_count, current_token_index, TOKEN_MAIN);

    // 消耗左括号 (
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_PAREN);

    // 消耗右括号 )
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_PAREN);

    // 消耗左花括号 {
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_CURLY_BRACE);

    // 循环解析直到右花括号 }
    while (*current_token_index < token_count && (*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
    {
        add_child(main_node, parse_statement(tokens, token_count, current_token_index));
    }

    // 消耗右花括号 }
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);

    return main_node;
}

// 解析 function
ASTNode *parse_function(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *function_node = create_node(NODE_FUNCTION);

    // 消耗 function
    consume(tokens, token_count, current_token_index, TOKEN_FUNCTION);

    // 获取函数名标识符
    function_node->data.function.name = strdup((*tokens)[*current_token_index].value);
    advance(tokens, current_token_index);

    // 消耗左括号 (
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_PAREN);

    // 解析参数列表
    function_node->data.function.param_list = parse_param_list(tokens, token_count, current_token_index);

    // 消耗右括号 )
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_PAREN);

    // 消耗左花括号 {
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_CURLY_BRACE);

    // 循环解析直到右花括号 }
    while (*current_token_index < token_count && (*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
    {
        add_child(function_node, parse_statement(tokens, token_count, current_token_index));
    }

    // 消耗右花括号 }
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);

    return function_node;
}

// 解析定义函数的参数列表
ASTNode *parse_param_list(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *param_list_node = create_node(NODE_PARAM_LIST);

    // 循环解析直到右括号 )
    while (*current_token_index < token_count && (*tokens)[*current_token_index].type != TOKEN_RIGHT_PAREN)
    {
        if ((*tokens)[*current_token_index].type == TOKEN_IDENTIFIER)
        {
            ASTNode *param = create_node(NODE_IDENTIFIER);
            param->data.identifier.name = strdup((*tokens)[*current_token_index].value);
            add_child(param_list_node, param);
            advance(tokens, current_token_index);
        }

        if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_COMMA)
        {
            advance(tokens, current_token_index);
        }
    }

    return param_list_node;
}

// 解析 statement
ASTNode *parse_statement(Token **tokens, int token_count, int *current_token_index)
{
    TokenType current_type = (*tokens)[*current_token_index].type;

    switch (current_type)
    {

    case TOKEN_IDENTIFIER:
        // 解析不知道是赋值还是函数调用的语句
        return parse_assignment_or_function_call(tokens, token_count, current_token_index);

    case TOKEN_IF:
        // 解析 if 语句
        return parse_if_statement(tokens, token_count, current_token_index);

    case TOKEN_FOR:
        // 解析 for 循环
        return parse_for_loop(tokens, token_count, current_token_index);

    case TOKEN_RETURN:
        // 解析 return 语句
        return parse_return_statement(tokens, token_count, current_token_index);

    case TOKEN_LEFT_CURLY_BRACE:
        // 解析复合语句
        return parse_compound_statement(tokens, token_count, current_token_index);

    default:
        printf("Syntax error: Unexpected token in statement.\n");
        exit(1);
        return NULL;
    }
}

// 解析复合语句
ASTNode *parse_compound_statement(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *compound_node = create_node(NODE_STATEMENT);
    // 消耗左花括号 {
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_CURLY_BRACE);

    // 循环解析直到右花括号 }
    while (*current_token_index < token_count && (*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
    {
        add_child(compound_node, parse_statement(tokens, token_count, current_token_index));
    }

    // 消耗右花括号 }
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);

    return compound_node;
}

// 解析不知道是赋值还是函数调用的语句
ASTNode *parse_assignment_or_function_call(Token **tokens, int token_count, int *current_token_index)
{
    // 保存当前标识符的位置
    int saved_token_index = *current_token_index;
    ASTNode *identifierNode = parse_identifier(tokens, token_count, current_token_index);

    if (identifierNode == NULL)
    {
        return NULL;
    }

    if (*current_token_index < token_count && ((*tokens)[*current_token_index].type == TOKEN_LEFT_PAREN))
    {
        // 回退令牌位置处理函数调用
        *current_token_index = saved_token_index;
        ASTNode *funcCallNode = parse_function_call(tokens, token_count, current_token_index);

        if (*current_token_index < token_count && ((*tokens)[*current_token_index].type == TOKEN_SEMICOLON))
        {
            consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);
        }
        else
        {
            printf("Syntax error: Expected semicolon after function call\n");
            free_ast(funcCallNode);
            exit(1);
            return NULL;
        }

        return funcCallNode;
    }
    else
    {
        // 回退令牌位置处理赋值语句
        *current_token_index = saved_token_index;
        return parse_assignment(tokens, token_count, current_token_index);
    }
}

ASTNode *parse_if_statement(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *if_node = create_node(NODE_IF_STATEMENT);

    // 消耗 if
    consume(tokens, token_count, current_token_index, TOKEN_IF);

    // 消耗左括号 (
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_PAREN);

    ASTNode *condition = parse_expression(tokens, token_count, current_token_index);
    add_child(if_node, condition);

    // 消耗右括号 )
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_PAREN);

    // 解析 true 分支
    ASTNode *true_branch;
    if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_LEFT_CURLY_BRACE)
    {
        true_branch = parse_compound_statement(tokens, token_count, current_token_index);
    }
    else
    {
        true_branch = parse_statement(tokens, token_count, current_token_index);
    }
    add_child(if_node, true_branch);

    // 解析 else 分支
    if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_ELSE)
    {
        advance(tokens, current_token_index);

        ASTNode *else_node = create_node(NODE_ELSE_STATEMENT);
        if (!else_node)
        {
            free_ast(if_node);
            return NULL;
        }

        ASTNode *false_branch;
        if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_LEFT_CURLY_BRACE)
        {
            false_branch = parse_compound_statement(tokens, token_count, current_token_index);
        }
        else
        {
            false_branch = parse_statement(tokens, token_count, current_token_index);
        }
        add_child(else_node, false_branch);

        add_child(if_node, else_node);
    }

    return if_node;
}

// 解析 return 语句
ASTNode *parse_return_statement(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *return_node = create_node(NODE_RETURN_STATEMENT);

    // 消耗 return
    consume(tokens, token_count, current_token_index, TOKEN_RETURN);

    // 如果下一个不是 ; 则解析返回的表达式
    if ((*tokens)[*current_token_index].type != TOKEN_SEMICOLON)
    {
        return_node->data.return_statement.expression = parse_expression(tokens, token_count, current_token_index);
    }

    // 消耗分号 ;
    consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);

    return return_node;
}

// 解析变量声明语句
ASTNode *parse_var_decl(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *var_decl_node = create_node(NODE_VAR_DECL);

    // 获取变量名
    var_decl_node->data.var_decl.name = strdup((*tokens)[*current_token_index].value);
    advance(tokens, current_token_index);

    // 消耗等号 =
    consume(tokens, token_count, current_token_index, TOKEN_EQUALS);

    // 解析表达式
    var_decl_node->data.var_decl.value = parse_expression(tokens, token_count, current_token_index);

    add_child(var_decl_node, var_decl_node->data.var_decl.value);
    // 消耗分号 ;
    consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);

    return var_decl_node;
}

// 解析 for
ASTNode *parse_for_loop(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *for_loop_node = create_node(NODE_FOR_LOOP);

    // 消耗 for
    consume(tokens, token_count, current_token_index, TOKEN_FOR);

    // 消耗左括号 (
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_PAREN);

    if ((*tokens)[*current_token_index].type != TOKEN_IDENTIFIER)
    {
        printf("Syntax error: Expected identifier in for loop.\n");
        free_ast(for_loop_node);
        exit(1);
        return NULL;
    }
    for_loop_node->data.for_loop.var_name = strdup((*tokens)[*current_token_index].value);
    advance(tokens, current_token_index);

    // 消耗冒号 :
    consume(tokens, token_count, current_token_index, TOKEN_COLON);

    // 解析起始表达式
    for_loop_node->data.for_loop.start_expr = parse_expression(tokens, token_count, current_token_index);

    // 消耗逗号 ,
    consume(tokens, token_count, current_token_index, TOKEN_COMMA);

    // 解析结束表达式
    for_loop_node->data.for_loop.end_expr = parse_expression(tokens, token_count, current_token_index);

    // 消耗右括号 )
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_PAREN);

    // 检查是否为复合语句
    if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_LEFT_CURLY_BRACE)
    {
        // 复合语句
        consume(tokens, token_count, current_token_index, TOKEN_LEFT_CURLY_BRACE);
        while (*current_token_index < token_count && (*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
        {
            add_child(for_loop_node, parse_statement(tokens, token_count, current_token_index));
        }
        consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);
    }
    else
    {
        // 单语句
        add_child(for_loop_node, parse_statement(tokens, token_count, current_token_index));
    }

    return for_loop_node;
}

// 解析赋值语句
ASTNode *parse_assignment(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *assignment_node = create_node(NODE_ASSIGNMENT);

    // 获取变量名
    ASTNode *var_node = create_node(NODE_IDENTIFIER);
    var_node->data.var_decl.name = strdup((*tokens)[*current_token_index].value);

    add_child(assignment_node, var_node);

    // 消耗标识符
    (*current_token_index)++;

    // 检查是否有数组下标
    if ((*tokens)[*current_token_index].type == TOKEN_LEFT_SQUARE_BRACKET)
    {
        advance(tokens, current_token_index); // 跳过左方括号

        ASTNode *index = parse_expression(tokens, token_count, current_token_index); // 解析下标表达式
        add_child(var_node, index);                                                  // 将下标表达式添加为变量节点的子节点

        consume(tokens, token_count, current_token_index, TOKEN_RIGHT_SQUARE_BRACKET); // 确保有右方括号并跳过
    }

    // 消耗等号 =
    consume(tokens, token_count, current_token_index, TOKEN_EQUALS);

    // 解析表达式
    ASTNode *value_node = parse_expression(tokens, token_count, current_token_index);
    add_child(assignment_node, value_node);

    // 消耗分号 ;
    consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);

    return assignment_node;
}

// 解析函数调用
ASTNode *parse_function_call(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_FUNCTION_CALL);
    if (node == NULL)
        return NULL;

    // 解析函数名标识符
    ASTNode *function_name = parse_identifier(tokens, token_count, current_token_index);

    add_child(node, function_name);

    // 消耗左括号
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_PAREN);
    // 解析参数
    if ((*tokens)[*current_token_index].type != TOKEN_RIGHT_PAREN)
    {
        ASTNode *args = parse_arg_list(tokens, token_count, current_token_index);
        add_child(node, args);
    }

    // 消耗右括号 )
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_PAREN);

    return node;
}

// 解析数组声明
ASTNode *parse_array_decl(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_ARRAY_DECL);
    if (node == NULL)
        return NULL;

    ASTNode *array_name = parse_identifier(tokens, token_count, current_token_index);
    add_child(node, array_name);

    // 消耗等号 =
    consume(tokens, token_count, current_token_index, TOKEN_EQUALS);

    // 消耗左方括号 [
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_SQUARE_BRACKET);

    // 解析表达式列表
    ASTNode *expr_list = parse_expression_list(tokens, token_count, current_token_index);
    add_child(node, expr_list);

    // 消耗右方括号 ]
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_SQUARE_BRACKET);

    consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);

    return node;
}

// 解析键值对声明
ASTNode *parse_key_value_decl(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_KEY_VALUE_DECL);
    if (node == NULL)
        return NULL;

    ASTNode *key_value_name = parse_identifier(tokens, token_count, current_token_index);
    add_child(node, key_value_name);

    // 消耗等号 =
    consume(tokens, token_count, current_token_index, TOKEN_EQUALS);

    // 消耗左花括号 {
    consume(tokens, token_count, current_token_index, TOKEN_LEFT_CURLY_BRACE);

    // 解析键值对
    while ((*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
    {
        ASTNode *pair = parse_key_value_pair(tokens, token_count, current_token_index);
        add_child(node, pair);
        if ((*tokens)[*current_token_index].type == TOKEN_COMMA)
        {
            advance(tokens, current_token_index);
        }
    }

    // 消耗右花括号 }
    consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);

    consume(tokens, token_count, current_token_index, TOKEN_SEMICOLON);

    return node;
}

// 解析表达式列表
ASTNode *parse_expression_list(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_EXPRESSION_LIST);
    if (node == NULL)
        return NULL;

    while (1)
    {
        ASTNode *expr;
        if ((*tokens)[*current_token_index].type == TOKEN_LEFT_SQUARE_BRACKET)
        {
            // 创建一个虚拟的数组声明节点
            ASTNode *temp_array_decl = create_node(NODE_ARRAY_DECL);
            advance(tokens, current_token_index);
            ASTNode *expr_list = parse_expression_list(tokens, token_count, current_token_index);
            add_child(temp_array_decl, expr_list);
            consume(tokens, token_count, current_token_index, TOKEN_RIGHT_SQUARE_BRACKET);
            expr = temp_array_decl;
        }
        else if ((*tokens)[*current_token_index].type == TOKEN_LEFT_CURLY_BRACE)
        {
            // 创建一个虚拟的键值对声明节点
            ASTNode *temp_kv_decl = create_node(NODE_KEY_VALUE_DECL);
            advance(tokens, current_token_index);
            while ((*tokens)[*current_token_index].type != TOKEN_RIGHT_CURLY_BRACE)
            {
                ASTNode *pair = parse_key_value_pair(tokens, token_count, current_token_index);
                add_child(temp_kv_decl, pair);
                if ((*tokens)[*current_token_index].type == TOKEN_COMMA)
                {
                    advance(tokens, current_token_index);
                }
            }
            consume(tokens, token_count, current_token_index, TOKEN_RIGHT_CURLY_BRACE);
            expr = temp_kv_decl;
        }
        else
        {
            expr = parse_expression(tokens, token_count, current_token_index);
        }
        add_child(node, expr);

        if ((*tokens)[*current_token_index].type != TOKEN_COMMA)
        {
            break;
        }
        advance(tokens, current_token_index);
    }

    return node;
}

// 解析键值对
ASTNode *parse_key_value_pair(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *pair_node = create_node(NODE_KEY_VALUE_PAIR);
    if (pair_node == NULL)
        return NULL;

    // 解析键
    ASTNode *key = parse_expression(tokens, token_count, current_token_index);
    add_child(pair_node, key);

    // 消耗冒号 :
    consume(tokens, token_count, current_token_index, TOKEN_COLON);

    // 解析值
    ASTNode *value;
    if ((*tokens)[*current_token_index].type == TOKEN_LEFT_SQUARE_BRACKET)
    {
        value = parse_array_decl(tokens, token_count, current_token_index);
    }
    else if ((*tokens)[*current_token_index].type == TOKEN_LEFT_CURLY_BRACE)
    {
        value = parse_key_value_decl(tokens, token_count, current_token_index);
    }
    else
    {
        value = parse_expression(tokens, token_count, current_token_index);
    }
    add_child(pair_node, value);

    return pair_node;
}

// 解析调用函数的参数列表
ASTNode *parse_arg_list(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *args_node = create_node(NODE_ARGUMENT_LIST);
    if (args_node == NULL)
        return NULL;

    // 循环解析直到右括号 )

    while ((*current_token_index < token_count) && ((*tokens)[*current_token_index].type != TOKEN_RIGHT_PAREN))
    {
        ASTNode *arg = parse_expression(tokens, token_count, current_token_index);
        if (arg == NULL)
        {
            free_ast(args_node);
            return NULL;
        }
        add_child(args_node, arg);

        // 如果下一个令牌是逗号, 跳过并继续解析下一个参数
        if ((*current_token_index < token_count) && ((*tokens)[*current_token_index].type == TOKEN_COMMA))
        {
            advance(tokens, current_token_index);
        }
    }

    return args_node;
}

// 解析 expression
ASTNode *parse_expression(Token **tokens, int token_count, int *current_token_index)
{

    // 包括解析子表达式的情形
    ASTNode *node = create_node(NODE_EXPRESSION);
    if (node == NULL)
    {
        return NULL;
    }

    if ((*tokens)[*current_token_index].type == TOKEN_LEFT_PAREN)
    {
        // 跳过左括号 (
        advance(tokens, current_token_index);
        node = parse_expression(tokens, token_count, current_token_index);
        if ((*tokens)[*current_token_index].type != TOKEN_RIGHT_PAREN)
        {
            free_ast(node);
            return NULL;
        }
        // 跳过右括号 )
        advance(tokens, current_token_index);
    }
    else if ((*tokens)[*current_token_index].type == TOKEN_IDENTIFIER)
    {
        int next_token_index = *current_token_index + 1;
        if (next_token_index < token_count && (*tokens)[next_token_index].type == TOKEN_LEFT_PAREN)
        {
            node = parse_function_call(tokens, token_count, current_token_index);
        }
        else
        {
            node = parse_identifier(tokens, token_count, current_token_index);

            // 检查数组下标访问
            if (*current_token_index < token_count && (*tokens)[*current_token_index].type == TOKEN_LEFT_SQUARE_BRACKET)
            {
                advance(tokens, current_token_index); // 消耗 '['
                ASTNode *index = parse_expression(tokens, token_count, current_token_index);
                consume(tokens, token_count, current_token_index, TOKEN_RIGHT_SQUARE_BRACKET); // 消耗 ']'

                // 创建数组访问节点
                ASTNode *array_access = create_node(NODE_ARRAY_ACCESS);
                add_child(array_access, node);
                add_child(array_access, index);
                node = array_access;
            }
        }
    }
    else
    {
        node = parse_literal(tokens, token_count, current_token_index);
    }
    while ((*current_token_index < token_count) &&
           ((*tokens)[*current_token_index].type >= TOKEN_PLUS && (*tokens)[*current_token_index].type <= TOKEN_EQUAL))
    {
        ASTNode *opNode = create_node(NODE_EXPRESSION);
        add_child(opNode, node);
        add_child(opNode, parse_operator(tokens, token_count, current_token_index));
        add_child(opNode, parse_expression(tokens, token_count, current_token_index));
        node = opNode;
    }

    return node;
}

// 解析字面值
ASTNode *parse_literal(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_LITERAL);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (currentToken->type == TOKEN_INT || currentToken->type == TOKEN_FLOAT || currentToken->type == TOKEN_STRING)
    {
        node->data.literal.value = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected literal\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析操作符
ASTNode *parse_operator(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_OPERATOR);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    switch (currentToken->type)
    {
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_STAR:
    case TOKEN_SLASH:
    case TOKEN_GREATER:
    case TOKEN_LESS:
    case TOKEN_GREATER_EQUAL:
    case TOKEN_LESS_EQUAL:
    case TOKEN_EQUAL:
        node->data.operator_node.op = strdup(currentToken->value);
        advance(tokens, current_token_index);
        break;
    default:
        printf("Syntax error: Expected operator\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析标识符
ASTNode *parse_identifier(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_IDENTIFIER);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (currentToken->type == TOKEN_IDENTIFIER)
    {
        node->data.identifier.name = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected identifier\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析 int
ASTNode *parse_int(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_INT);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (currentToken->type == TOKEN_INT)
    {
        node->data.int_node.value = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected integer\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析 float
ASTNode *parse_float(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_FLOAT);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (currentToken->type == TOKEN_FLOAT)
    {
        node->data.float_node.value = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected float\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析 string
ASTNode *parse_string(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_STRING);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (currentToken->type == TOKEN_STRING)
    {
        node->data.string_node.value = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected string\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析 digit
ASTNode *parse_digit(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_DIGIT);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (isdigit(currentToken->value[0]))
    {
        node->data.int_node.value = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected digit\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

// 解析 char
ASTNode *parse_char(Token **tokens, int token_count, int *current_token_index)
{
    ASTNode *node = create_node(NODE_CHAR);
    if (node == NULL)
        return NULL;

    Token *currentToken = &((*tokens)[*current_token_index]);
    if (isalnum(currentToken->value[0]) || ispunct(currentToken->value[0]))
    {
        node->data.char_node.text = strdup(currentToken->value);
        advance(tokens, current_token_index);
    }
    else
    {
        printf("Syntax error: Expected character\n");
        free_ast(node);
        exit(1);
        return NULL;
    }

    return node;
}

void print_ast_to_file(ASTNode *node, int depth, FILE *outfile)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        fprintf(outfile, "    ");
    }

    switch (node->type)
    {
    case NODE_PROGRAM:
        fprintf(outfile, "<program>\n");
        break;
    case NODE_MAIN:
        fprintf(outfile, "<main>\n");
        break;
    case NODE_FUNCTION:
        fprintf(outfile, "<function>: %s\n", node->data.function.name);
        break;
    case NODE_PARAM_LIST:
        fprintf(outfile, "<param_list>\n");
        break;
    case NODE_STATEMENT:
        fprintf(outfile, "<statement>\n");
        break;
    case NODE_VAR_DECL:
        fprintf(outfile, "<var_decl>: %s\n", node->data.var_decl.name);
        break;
    case NODE_ASSIGNMENT:
        fprintf(outfile, "<assignment>\n");
        break;
    case NODE_IF_STATEMENT:
        fprintf(outfile, "<if>\n");
        break;
    case NODE_ELSE_STATEMENT:
        fprintf(outfile, "<else>\n");
        break;
    case NODE_ARRAY_ACCESS:
        fprintf(outfile, "<array_access>\n");
        break;
    case NODE_FOR_LOOP:
        fprintf(outfile, "<for_loop>: %s\n", node->data.for_loop.var_name);
        break;
    case NODE_RETURN_STATEMENT:
        fprintf(outfile, "<return>\n");
        break;
    case NODE_FUNCTION_CALL:
        fprintf(outfile, "<function_call>\n");
        break;
    case NODE_ARRAY_DECL:
        fprintf(outfile, "<array>\n");
        break;
    case NODE_KEY_VALUE_DECL:
        fprintf(outfile, "<key_value>\n");
        break;
    case NODE_EXPRESSION_LIST:
        fprintf(outfile, "<expression_list>\n");
        break;
    case NODE_ARGUMENT_LIST:
        fprintf(outfile, "<arg_list>\n");
        break;
    case NODE_KEY_VALUE_PAIR:
        fprintf(outfile, "<key_value_pair>\n");
        break;
    case NODE_EXPRESSION:
        fprintf(outfile, "<expression>\n");
        break;
    case NODE_LITERAL:
        fprintf(outfile, "<literal>: %s\n", node->data.literal.value);
        break;
    case NODE_OPERATOR:
        fprintf(outfile, "<operator>: %s\n", node->data.operator_node.op);
        break;
    case NODE_IDENTIFIER:
        fprintf(outfile, "<identifier>: %s\n", node->data.identifier.name);
        break;
    case NODE_INT:
        fprintf(outfile, "<int>: %s\n", node->data.int_node.value);
        break;
    case NODE_FLOAT:
        fprintf(outfile, "<float>: %s\n", node->data.float_node.value);
        break;
    case NODE_STRING:
        fprintf(outfile, "<string>: %s\n", node->data.string_node.value);
        break;
    case NODE_LETTER:
        fprintf(outfile, "<letter>\n");
        break;
    case NODE_DIGIT:
        fprintf(outfile, "<digit>\n");
        break;
    case NODE_CHAR:
        fprintf(outfile, "<char>: %s\n", node->data.char_node.text);
        break;
    default:
        fprintf(outfile, "<unknown>\n");
        break;
    }

    for (int i = 0; i < node->children_count; i++)
    {
        print_ast_to_file(node->children[i], depth + 1, outfile);
    }
}

// // 测试输入
// int main()
// {
//     freopen("input.txt", "r", stdin);
//     int fsize = 1000;

//     char source[1000];
//     fread(source, 1, fsize, stdin);

//     source[fsize] = 0;

//     int token_count = 0;
//     Token *tokens = lexer(source, &token_count);
//     ASTNode *root = parse_program(&tokens, token_count);

//     freopen("output_parser.txt", "w", stdout);

//     print_ast_to_file(root, 0, stdout);

//     return 0;
// }