#include <stdio.h>
#include <string.h>

#define OK             1
#define ERROR          0

#if 0
int main(void)
{
    char a[101], s[101];

    int i, len, mid, next, top;

    gets(a);
    len = strlen(a);
    mid = len / 2 - 1;

    top = 0;

    for (i = 0; i <= mid; i++)
        s[++top] = a[i];

    if (0 == len % 2)
        next = mid + 1;
    else
        next = mid + 2;

    for (i = next; i <= len - 1; i++)
    {
        if (a[i] != s[top])
            break;
        top--;
    }

    if (0 == top)
        printf("Yes.\n");
    else
        printf("No.\n");

    return 0;
}
#endif

typedef struct stack
{
    int array[100];
    int top;
}stack_t;

int main(void)
{
    int test_data[10] = {1, 2, 3, 4, 5, 6, 4, 3, 2, 1};
    int i;
    stack_t Slack = {.top = 0};

    int len = sizeof(test_data) / sizeof(test_data[0]);
    int stack_top = len / 2;
    /*将前面的一半数据进栈*/
    /*栈的top指向下一个即将进栈的位置*/
    for (i = 0; i < stack_top; i++)
    {
        Slack.array[Slack.top] = test_data[i];
        Slack.top++;
    }
    /*如果总个数是基数，跳过中间的那个数据*/
    if (0 != len % 2)
        i++;
    /*出栈并比较数值*/
    for (; i < len; i++)
    {
        Slack.top--;
        if (Slack.array[Slack.top] != test_data[i])
            break;
    }

    if (Slack.top)
        printf("ERROR\n");
    else    
        printf("OKK\n");

    return 0;
}