#include "../pengmc.h"

// 定义栈节点
typedef struct node 
{
    char data;
    struct node* next;
} node_t;

// 定义栈结构
typedef struct stack 
{
    node_t* top;
} stack_t;

// 创建栈
stack_t* create_stack() 
{
    stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
    stack->top = NULL;

    return stack;
}

// 压栈
void push(stack_t* stack, char data) 
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));

    if (!new_node)
    {
        MALLOC_ERROR_FILE_FUNC_LINE;
        exit(-1);
    }
    /*新节点的下一节点为原来的栈顶指针，更新栈顶指针*/
    new_node->data = data;
    new_node->next = stack->top;
    stack->top     = new_node;
}

// 弹栈
char pop(stack_t* stack) 
{
    if (stack->top == NULL) 
        return '\0'; // 表示栈为空

    /*取值*/
    char data    = stack->top->data;
    /*定义临时指针指向栈顶指针，更新栈顶指针，释放原栈顶指针*/
    node_t* temp = stack->top;
    stack->top   = stack->top->next;
    free(temp);

    return data;
}

// 检查栈是否为空
int is_empty(stack_t* stack) 
{
    return stack->top == NULL;
}

// 检查括号是否匹配
int is_match(char open, char close) 
{
    if (open == '(' && close == ')') 
        return 1;
    if (open == '[' && close == ']') 
        return 1;
    if (open == '{' && close == '}') 
        return 1;
    return 0;
}

// 检查括号字符串是否匹配
int are_parentheses_balanced(char* expr) 
{
    stack_t* stack = create_stack();
    for (int i = 0; expr[i] != '\0'; i++) 
    {
        if (expr[i] == '(' || expr[i] == '[' || expr[i] == '{') 
            push(stack, expr[i]);
        else if (expr[i] == ')' || expr[i] == ']' || expr[i] == '}') 
        {
            if (is_empty(stack) || !is_match(pop(stack), expr[i])) 
                return 0; // 不匹配
        }
    }
    
    int balanced = is_empty(stack);
    while (!is_empty(stack)) 
    // 清理
        pop(stack);

    free(stack);

    return balanced;
}

int main() 
{
    char* expression = "{[(())]11111}";
    if (are_parentheses_balanced(expression)) 
        printf("Balanced\n");
    else 
        printf("Not Balanced\n");

    return 0;
}
