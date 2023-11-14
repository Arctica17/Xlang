# X 语言

## 1 快速入门

### Hello World 示例

```
print("Hello, World!");
```

### 变量和数据类型

```
x = 10;        // 整数
y = 3.14;      // 浮点数
name = "Alice"; // 字符串
```

### 数据结构

```
array = [1, 2, 3];        // 数组
pair = {1: "value"};    // 键值对
```

### 函数

```
function add(a, b)
{
    return a + b;
}
result = add(5, 3);
```

### 循环和迭代

```
for(i: 0, 10)
{
    array[i] = i;
}
```

### 条件语句

```
if(x > 5)
    print("x is greater than 5");
if (x == 5)
    print("x is 5");
else
    print("x is less than 5");
```

### 输入输出

```
input = read();
print("You entered:", input);
```

### 段落示例

```
x = 10;

function max(a, b)
{
    if(a >= b)
        return a;
    else
        return b;
}

main()
{
    y = read();
    print(max(x, y));
}
```

## 2 BNF 范式

```
<program> ::= {<var_decl> | <array_decl> | <key_value_decl> | <function>} <main> {<var_decl> | <array_decl> | <key_value_decl> | <function>}

<var_decl> ::= <identifier> "=" <expression> ";"

<main> ::= "main()" "{" {<statement>} "}"

<function> ::= "function" <identifier> "(" [<param_list>] ")" "{" {<statement>} "}"

<param_list> ::= <identifier> {"," <identifier>}

<statement> ::= <assignment>
              | <if_statement>
              | <for_loop>
              | <return_statement>
              | <function_call>

<assignment> ::= <identifier> "=" <expression> ";"
               | <identifier> "[" <expression> "]" "=" <expression> ";"

<if_statement> ::= "if" "(" <expression> ")" <statement> ["else" <statement>]

<for_loop> ::= "for" "(" <identifier> ":" <expression> "," <expression> ")" "{" {<statement>} "}"

<return_statement> ::= "return" [<expression>] ";"

<function_call> ::= <identifier> "(" [<arg_list>] ")" ";"

<array_decl> ::= <identifier> "=" "[" <expression_list> "]" ";"

<key_value_decl> ::= <identifier> "=" "{" <key_value_list> "}" ";"

<expression_list> ::= <expression> {"," <expression>}

<key_value_list> ::= <key_value_pair> {"," <key_value_pair>}

<key_value_pair> ::= <expression> ":" <expression>

<arg_list> ::= <expression> {"," <expression>}

<expression> ::= <literal>
               | <identifier>
               | <function_call>
               | <expression> <operator> <expression>
               | <identifier> "[" <expression> "]"
               | "(" <expression> ")"
               | "{" <key_value_list> "}"

<literal> ::= <int>
            | <float>
            | <string>

<operator> ::= "+" | "-" | "*" | "/" | "==" | "<" | ">" | "<=" | ">="

<identifier> ::= <letter> {<letter> | <digit>}

<int> ::= <digit> {<digit>}

<float> ::= <digit> {<digit>} "." {<digit>}

<string> ::= "\"" {<char>} "\""

<letter> ::= "a" | "b" | ... | "z" | "A" | "B" | ... | "Z"

<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

<char> ::= <letter> | <digit> | <other_chars>


```

