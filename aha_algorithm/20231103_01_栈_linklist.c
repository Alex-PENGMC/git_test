#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define OK             0
#define ERROR          -1

typedef struct queue
{
    int value;
    struct queue *pnext;
}queue_t;

typedef struct stack
{
    int value;
    struct stack *ptop;
}stack_t;

int push_queue(queue_t **head, queue_t **tail, int data);
int pop_queue(queue_t **head, int *data);
int pop_stack(stack_t **head, int *data);
int push_stack(stack_t **head, int data);
void print_queue(queue_t **head);

int main(void)
{
    int i, temp;
    queue_t *q1_head = NULL;
    queue_t *q1_tail = NULL;
    queue_t *q2_head = NULL;
    queue_t *q2_tail = NULL;

    stack_t *s1_head = NULL;
    
    int q1_data[] = {2, 4, 1, 2, 5, 6};
    int q2_data[] = {3, 1, 3, 5, 6, 4};

    /*队列初始化*/ 
    for (i = 0; i < sizeof(q1_data) / sizeof(q1_data[0]); i++)
    {
        push_queue(&q1_head, &q1_tail, q1_data[i]);
        push_queue(&q2_head, &q2_tail, q2_data[i]);
    }

    while (q1_head && q2_head)
    {
        /*q1队列首元素出队*/
        pop_queue(&q1_head, &temp);
        /*定义一个临时栈*/
        stack_t *temp_stack = NULL;
        /*从栈顶指针遍历整个栈，并将数据存入临时栈*/
        while (s1_head && s1_head->value != temp)
        {
            int stack_data;
            pop_stack(&s1_head, &stack_data);
            push_stack(&temp_stack, stack_data);
        }
        /*找到了*/
        if (s1_head)
        {
            int stack_data;
            /*将相等元素出栈加入临时栈中*/
            pop_stack(&s1_head, &stack_data);
            push_stack(&temp_stack, stack_data);
            /*将临时栈中所有元素压入队列中*/
            while (temp_stack)
            {
                int stack_data;
                pop_stack(&temp_stack, &stack_data);
                push_queue(&q1_head, &q1_tail, stack_data);
            }
            push_queue(&q1_head, &q1_tail, temp);
        }
        else
        {
            while (temp_stack)
            {
                int stack_data;
                pop_stack(&temp_stack, &stack_data);
                push_stack(&s1_head, stack_data);
            }
            push_stack(&s1_head, temp);
        }
        if (!q1_head)
        {
            printf("q2 has win.\n");
            break;
        }

        stack_t *temp_stack1 = NULL;
        /*q2队列首元素出队*/
        pop_queue(&q2_head, &temp);
        while (s1_head && s1_head->value != temp)
        {
            int stack_data;
            pop_stack(&s1_head, &stack_data);
            push_stack(&temp_stack1, stack_data);
        }
        /*找到了*/
        if (s1_head)
        {
            int stack_data;
            pop_stack(&s1_head, &stack_data);
            push_stack(&temp_stack1, stack_data);

            while (temp_stack1)
            {
                int stack_data;
                pop_stack(&temp_stack1, &stack_data);
                push_queue(&q2_head, &q2_tail, stack_data);
            }
            push_queue(&q2_head, &q2_tail, temp);
        }
        else
        {
            while (temp_stack1)
            {
                int stack_data;
                pop_stack(&temp_stack1, &stack_data);
                push_stack(&s1_head, stack_data);
            }
            push_stack(&s1_head, temp);
        }
        if (!q2_head)
        {
            printf("q1 has win.\n");
            break;
        }

        printf("\nThis is q1:\n");
        print_queue(&q1_head);
        printf("\nThis is q2:\n");
        print_queue(&q2_head);

    }
    
    return 0;
}

int push_queue(queue_t **head, queue_t **tail, int data)
{
    queue_t *temp = (queue_t *)malloc(sizeof(queue_t));
    
    if (!temp)
    {
        printf("malloc error in %s : %d\n", __func__, __LINE__);
        return ERROR;
    }
    
    temp->value = data;
    temp->pnext = NULL;

    if (!(*head))
    {
        *head = temp;
        *tail = temp;
    }
    else
    {
        (*tail)->pnext = temp;
        *tail = temp;
    }

    return OK;
}

int pop_queue(queue_t **head, int *data)
{
    if (!(*head))
    {
        printf("The queue is empty in %s %d.\n", __func__, __LINE__);
        return ERROR;
    }
    
    queue_t *temp = *head;

    if (data)
        *data  = temp->value; 

    *head = (*head)->pnext;

    free(temp);

    return OK;
}

int pop_stack(stack_t **head, int *data) 
{
    if (!head || !(*head)) {
        printf("The stack is empty in %s %d.\n", __func__, __LINE__);
        return ERROR;
    }

    stack_t *temp = *head;
    *data = temp->value;
    *head = (*head)->ptop;

    free(temp);

    return OK;
}

int push_stack(stack_t **head, int data)
{
    stack_t *temp = (stack_t *)malloc(sizeof(stack_t));
    if (!(temp))
    {
        printf("Malloc error in %s line %d.\n", __func__, __LINE__);
        return ERROR;
    }

    temp->value = data;
    temp->ptop  = *head;

    *head = temp;

    return OK;
}

void print_queue(queue_t **head)
{
    queue_t *pcur = *head;
    while (pcur)
    {
        printf("%d ", pcur->value);
        pcur = pcur->pnext;
    }
}