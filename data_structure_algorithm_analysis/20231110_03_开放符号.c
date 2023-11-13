#include "pengmc.h"

typedef struct Node 
{
    char data;
    struct Node *next;
} Node_t;

typedef struct 
{
    Node_t *top;
} Stack;

// 初始化栈
void initStack(Stack *s) 
{
    s->top = NULL;
}

// 栈是否为空
bool isEmpty(Stack *s) 
{
    return s->top == NULL;
}

// 入栈，相当于头插法
void push(Stack *s, char data) 
{
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));
    if (newNode == NULL) 
        exit(1); // 内存分配失败

    newNode->data = data;
    newNode->next = s->top;
    s->top = newNode;
}

// 出栈
char pop(Stack *s) 
{
    if (isEmpty(s)) 
        exit(1); // 栈为空时错误

    Node_t *temp = s->top;
    char data = temp->data;
    s->top    = s->top->next;
    free(temp);
    
    return data;
}

// 检查符号是否匹配
bool isMatch(char open, char close) 
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// 检查平衡符号
bool checkBalancedSymbols() 
{
    Stack s;
    initStack(&s);
    char ch;

    while ((ch = getchar()) != EOF) 
    {
        if (ch == '(' || ch == '[' || ch == '{') 
            push(&s, ch);
        else if (ch == ')' || ch == ']' || ch == '}') 
        {
            if (isEmpty(&s)) 
            {
                printf("错误：多余的封闭符号 %c\n", ch);
                return false;
            } 
            else 
            {
                char stackTop = pop(&s);
                if (!isMatch(stackTop, ch)) 
                {
                    printf("错误：符号 %c 与 %c 不匹配\n", stackTop, ch);
                    return false;
                }
            }
        }
    }

    if (!isEmpty(&s)) 
    {
        printf("错误：有未匹配的开放符号\n");
        return false;
    }

    return true;
}

int main() 
{
    if (checkBalancedSymbols()) 
        printf("符号平衡\n");
    else 
        printf("符号不平衡\n");

    return 0;
}
