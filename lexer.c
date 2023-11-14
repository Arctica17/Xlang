#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    TOKEN_INT,    // 整数
    TOKEN_FLOAT,  // 浮点数
    TOKEN_STRING, // 字符串

    TOKEN_IDENTIFIER, // 标识符

    TOKEN_PLUS,          // 加号 +
    TOKEN_MINUS,         // 减号 -
    TOKEN_STAR,          // 乘号 *
    TOKEN_SLASH,         // 除号 /
    TOKEN_GREATER,       // 大于 >
    TOKEN_LESS,          // 小于 <
    TOKEN_GREATER_EQUAL, // 大于等于 >=
    TOKEN_LESS_EQUAL,    // 小于等于 <=
    TOKEN_EQUAL,         // 等于 ==

    TOKEN_EQUALS,    // 等号 =
    TOKEN_COLON,     // 冒号 :
    TOKEN_SEMICOLON, // 分号 ;
    TOKEN_COMMA,     // 逗号 ,

    TOKEN_LEFT_PAREN,           // 左括号 (
    TOKEN_RIGHT_PAREN,          // 右括号 )
    TOKEN_LEFT_SQUARE_BRACKET,  // 左方括号 [
    TOKEN_RIGHT_SQUARE_BRACKET, // 右方括号 ]
    TOKEN_LEFT_CURLY_BRACE,     // 左花括号 {
    TOKEN_RIGHT_CURLY_BRACE,    // 右花括号 }

    TOKEN_HASH, // 注释符 #

    TOKEN_MAIN,     // main 关键字
    TOKEN_FUNCTION, // function 关键字
    TOKEN_FOR,      // for 循环关键字
    TOKEN_IF,       // if 条件关键字
    TOKEN_ELSE,     // else 条件关键字
    TOKEN_RETURN,   // return 关键字

    TOKEN_EOF,    // 文件结束标记
    TOKEN_UNKNOWN // 未知

} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

Token *new_token(TokenType type, char *value)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    return token;
}

// 判断是否是空白字符
int is_whitespace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f';
}

// 判断是否是数字
int is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

// 判断是否是字母或下划线
int is_alpha(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

// 判断是否是关键字
TokenType check_keyword(const char *start, int length)
{
    if (length == 4 && strncmp(start, "main", 4) == 0)
        return TOKEN_MAIN;
    if (length == 8 && strncmp(start, "function", 8) == 0)
        return TOKEN_FUNCTION;
    if (length == 3 && strncmp(start, "for", 3) == 0)
        return TOKEN_FOR;
    if (length == 2 && strncmp(start, "if", 2) == 0)
        return TOKEN_IF;
    if (length == 4 && strncmp(start, "else", 4) == 0)
        return TOKEN_ELSE;
    if (length == 6 && strncmp(start, "return", 6) == 0)
        return TOKEN_RETURN;
    return TOKEN_IDENTIFIER;
}

// 设置 Token 值
void set_token_value(Token *token, const char *start, int length)
{
    token->value = malloc(length + 1);
    strncpy(token->value, start, length);
    token->value[length] = '\0';
}

// Lexer 函数
Token *lexer(const char *source, int *token_count)
{
    int length = strlen(source);
    Token *tokens = malloc(sizeof(Token) * length);
    int count = 0;
    const char *start;

    for (int i = 0; i < length; i++)
    {
        if (is_whitespace(source[i]))
            continue;

        if (source[i] == '#')
        { // 处理注释
            while (i < length && source[i] != '\n')
                i++;
            continue;
        }

        start = &source[i];
        TokenType type;

        // 处理字符串
        if (source[i] == '\"')
        {
            i++;
            start = &source[i];
            while (i < length && source[i] != '\"')
            {
                if (source[i] == '\\' && i + 1 < length)
                    i++;
                i++;
            }
            type = TOKEN_STRING;
            set_token_value(&tokens[count], start, &source[i] - start);
            tokens[count].type = type;
            count++;
            continue;
        }

        // 处理数字
        if (is_digit(source[i]))
        {
            start = &source[i];
            while (is_digit(source[i + 1]))
                i++;

            if (source[i + 1] == '.' && is_digit(source[i + 2]))
            {
                i += 2;
                while (is_digit(source[i + 1]))
                    i++;
                type = TOKEN_FLOAT;
            }
            else
            {
                type = TOKEN_INT;
            }

            set_token_value(&tokens[count], start, &source[i] - start + 1);
            tokens[count].type = type;
            count++;
            continue;
        }

        // 处理标识符和关键字
        if (is_alpha(source[i]) || source[i] == '_')
        {
            start = &source[i];
            while (is_alpha(source[i + 1]) || is_digit(source[i + 1]) || source[i + 1] == '_')
                i++;
            int word_length = &source[i] - start + 1;
            type = check_keyword(start, word_length);
            set_token_value(&tokens[count], start, word_length);
            tokens[count].type = type;
            set_token_value(&tokens[count], start, &source[i] - start + 1);
            count++;
            continue;
        }

        // 处理其他
        switch (source[i])
        {
        case '+':
            type = TOKEN_PLUS;
            break;
        case '-':
            type = TOKEN_MINUS;
            break;
        case '*':
            type = TOKEN_STAR;
            break;
        case '/':
            type = TOKEN_SLASH;
            break;
        case ':':
            type = TOKEN_COLON;
            break;
        case ';':
            type = TOKEN_SEMICOLON;
            break;
        case ',':
            type = TOKEN_COMMA;
            break;
        case '(':
            type = TOKEN_LEFT_PAREN;
            break;
        case ')':
            type = TOKEN_RIGHT_PAREN;
            break;
        case '[':
            type = TOKEN_LEFT_SQUARE_BRACKET;
            break;
        case ']':
            type = TOKEN_RIGHT_SQUARE_BRACKET;
            break;
        case '{':
            type = TOKEN_LEFT_CURLY_BRACE;
            break;
        case '}':
            type = TOKEN_RIGHT_CURLY_BRACE;
            break;
        case '=':
            if (source[i + 1] == '=')
            {
                type = TOKEN_EQUAL;
                i++;
            }
            else
                type = TOKEN_EQUALS;
            break;
        case '>':
            if (source[i + 1] == '=')
            {
                type = TOKEN_GREATER_EQUAL;
                i++;
            }
            else
                type = TOKEN_GREATER;
            break;
        case '<':
            if (source[i + 1] == '=')
            {
                type = TOKEN_LESS_EQUAL;
                i++;
            }
            else
                type = TOKEN_LESS;
            break;

        default:
            type = TOKEN_UNKNOWN;
            break;
        }

        if (type != TOKEN_UNKNOWN)
        {
            tokens[count].type = type;
            set_token_value(&tokens[count], start, &source[i] - start + 1);
            count++;
        }
        else
        {
            fprintf(stderr, "Unrecognized token starting at %s\n", start);
        }
    }

    *token_count = count;
    return tokens;
}

// 打印 Token
void print_token(Token token)
{
    switch (token.type)
    {
    case TOKEN_INT:
        printf("TOKEN_INT: %s\n", token.value);
        break;
    case TOKEN_FLOAT:
        printf("TOKEN_FLOAT: %s\n", token.value);
        break;
    case TOKEN_STRING:
        printf("TOKEN_STRING: %s\n", token.value);
        break;
    case TOKEN_IDENTIFIER:
        printf("TOKEN_IDENTIFIER: %s\n", token.value);
        break;
    case TOKEN_PLUS:
        printf("TOKEN_PLUS: %s\n", token.value);
        break;
    case TOKEN_MINUS:
        printf("TOKEN_MINUS: %s\n", token.value);
        break;
    case TOKEN_STAR:
        printf("TOKEN_STAR: %s\n", token.value);
        break;
    case TOKEN_SLASH:
        printf("TOKEN_SLASH: %s\n", token.value);
        break;
    case TOKEN_GREATER:
        printf("TOKEN_GREATER: %s\n", token.value);
        break;
    case TOKEN_LESS:
        printf("TOKEN_LESS: %s\n", token.value);
        break;
    case TOKEN_GREATER_EQUAL:
        printf("TOKEN_GREATER_EQUAL: %s\n", token.value);
        break;
    case TOKEN_LESS_EQUAL:
        printf("TOKEN_LESS_EQUAL: %s\n", token.value);
        break;
    case TOKEN_EQUAL:
        printf("TOKEN_EQUAL: %s\n", token.value);
        break;
    case TOKEN_EQUALS:
        printf("TOKEN_EQUALS: %s\n", token.value);
        break;
    case TOKEN_COLON:
        printf("TOKEN_COLON: %s\n", token.value);
        break;
    case TOKEN_SEMICOLON:
        printf("TOKEN_SEMICOLON: %s\n", token.value);
        break;
    case TOKEN_COMMA:
        printf("TOKEN_COMMA: %s\n", token.value);
        break;
    case TOKEN_LEFT_PAREN:
        printf("TOKEN_LEFT_PAREN: %s\n", token.value);
        break;
    case TOKEN_RIGHT_PAREN:
        printf("TOKEN_RIGHT_PAREN: %s\n", token.value);
        break;
    case TOKEN_LEFT_SQUARE_BRACKET:
        printf("TOKEN_LEFT_SQUARE_BRACKET: %s\n", token.value);
        break;
    case TOKEN_RIGHT_SQUARE_BRACKET:
        printf("TOKEN_RIGHT_SQUARE_BRACKET: %s\n", token.value);
        break;
    case TOKEN_LEFT_CURLY_BRACE:
        printf("TOKEN_LEFT_CURLY_BRACE: %s\n", token.value);
        break;
    case TOKEN_RIGHT_CURLY_BRACE:
        printf("TOKEN_RIGHT_CURLY_BRACE: %s\n", token.value);
        break;
    case TOKEN_HASH:
        printf("TOKEN_HASH: %s\n", token.value);
        break;
    case TOKEN_MAIN:
        printf("TOKEN_MAIN: %s\n", token.value);
        break;
    case TOKEN_FUNCTION:
        printf("TOKEN_FUNCTION: %s\n", token.value);
        break;
    case TOKEN_FOR:
        printf("TOKEN_FOR: %s\n", token.value);
        break;
    case TOKEN_IF:
        printf("TOKEN_IF: %s\n", token.value);
        break;
    case TOKEN_ELSE:
        printf("TOKEN_ELSE: %s\n", token.value);
        break;
    case TOKEN_RETURN:
        printf("TOKEN_RETURN: %s\n", token.value);
        break;
    case TOKEN_EOF:
        printf("TOKEN_EOF: %s\n", token.value);
        break;
    default:
        printf("UNKNOWN TOKEN: %s\n", token.value);
    }
}

// // 测试输入
// int main()
// {
//     freopen("input.txt", "r", stdin);
//     int fsize = 1000;

//     char source[1000] = {0};
//     fread(source, 1, fsize, stdin);

//     int token_count = 0;
//     Token *tokens = lexer(source, &token_count);

//     freopen("output_lexer.txt", "w", stdout);
//     for (int i = 0; i < token_count; i++)
//     {
//         print_token(tokens[i]);
//     }

//     return 0;
// }