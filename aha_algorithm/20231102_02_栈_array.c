#include <stdio.h>

#define OK             1
#define ERROR          0

typedef struct queue
{
    int data[1000];
    int head;
    int tail;
}queue_t;

typedef struct stack
{
    int data[10];
    int top;
}stack_t;

void print_array(int start, int end, int *array);

int main(void)
{
    int i, temp;

    queue_t q1, q2;
    stack_t desk;

    q1.head = q1.tail = 0;
    q2.head = q2.tail = 0;

    desk.top = -1; 

#if (0)
    for (i = 0; i < 6; i++)
    {
        scanf("%d", &q1.data[q1.tail]);
        q1.tail++;
    }
    for (i = 0; i < 6; i++)
    {
        scanf("%d", &q2.data[q2.tail]);
        q2.tail++;
    }
#else
    /*q1数据进入队列并更新队尾*/
    int q1_data[100] = {2, 4, 1, 2, 5, 6};
    for (i = 0; i < 6; i++)
    {
        q1.data[q1.tail] = q1_data[i];
        q1.tail++;
    }

    /*q2数据进入队列并更新队尾*/
    int q2_data[100] = {3, 1, 3, 5, 6, 4};
    for (i = 0; i < 6; i++)
    {
        q2.data[q2.tail] = q2_data[i];
        q2.tail++;
    }
#endif

    while (q1.head != q1.tail && q2.head != q2.tail)
    {
        temp = q1.data[q1.head++];
        /*遍历栈*/
        for (i = 0; i <= desk.top; i++)
        {
            if (desk.data[i] == temp)
            {
                q1.data[q1.tail++] = temp;
                while (desk.top >= i)
                    q1.data[q1.tail++] = desk.data[desk.top--];
                break;
            }
        }

        if (i > desk.top)
            desk.data[++desk.top] = temp;
        printf("\nQ1: ");
        print_array(q1.head, q1.tail, q1.data);
        printf("\nS1: ");
        print_array(0, desk.top, desk.data);

        temp = q2.data[q2.head++];
        /*遍历栈*/
        for (i = 0; i <= desk.top; i++)
        {
            if (desk.data[i] == temp)
            {
                q2.data[q2.tail++] = temp;
                while (desk.top >= i)
                    q2.data[q2.tail++] = desk.data[desk.top--];
                break;
            }
        }

        if (i > desk.top)
            desk.data[++desk.top] = temp;

        printf("\nQ2: ");
        print_array(q2.head, q2.tail, q2.data);
        printf("\nS2: ");
        print_array(0, desk.top, desk.data);
    }
    
    if (q1.head == q1.tail)
        printf("\nQ2 has win.\n");

    if (q2.head == q2.tail)
        printf("\nQ1 has win.\n");


    return 0;
}

void print_array(int start, int end, int *array)
{
    int i;
    for (i = start; i <= end; i++)
    {
        printf("%d ", array[i]);
    }
}