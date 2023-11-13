#include <stdio.h>
#include <stdlib.h>

#define OK             1
#define ERROR          0

typedef struct queue
{
    int value;
    struct queue *pnext;
} queue_t;

void enter_queue(queue_t **head, queue_t **tail, const int value);
void print_queue(queue_t *head);
void delete_queue(queue_t **head, int num);

int main(void)
{
    int value, i;
    queue_t *head = NULL;
    queue_t *tail = NULL;

    int a[9] = {6, 3, 1, 7, 5, 8, 9, 2, 4};

#if (0)
    /*终端读入数据*/
    while (EOF != scanf("%d", &value))
    {
        enter_queue(&head, &tail, value);
    }
#else
    /*数组读入数据*/
    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        enter_queue(&head, &tail, a[i]);
#endif

    print_queue(head);
    
    printf("Here is the QQ number: ");
    while (head)
    {
        printf("%d ", head->value);
        delete_queue(&head, 1);
        if (head)
        {
            value = head->value;
            delete_queue(&head, 1);
            enter_queue(&head, &tail, value);
        }
    }
    printf("\n");
    return 0;
}

void enter_queue(queue_t **head, queue_t **tail, const int value)
{
    queue_t *temp = (queue_t *)malloc(sizeof(struct queue));
    if (!temp)
    {
        printf("Malloc memory failed.\n");
        exit(1);
    }

    temp->value = value;
    temp->pnext = NULL;

    /*首节点为空，新加入的节点为首尾节点*/
    if (!(*head))
    // if (NULL == (*head))
    {
        *head = temp;
        *tail = temp;
    }
    else/*队列采用尾插法,并更新尾节点*/
    {
        (*tail)->pnext = temp;
        *tail          = temp;
    }
}

void print_queue(queue_t *head)
{
    queue_t *pcurr = head;
    printf("Here is the original array:");
    while (pcurr)
    {
        printf("%d ", pcurr->value);
        pcurr = pcurr->pnext;
    }

    printf("\n");
}

void delete_queue(queue_t **head, int num)
{
    int i;
    queue_t *pcur = *head;
    for (i = 0; i < num; i++)
    {
        *head = pcur->pnext;
        free(pcur);
        pcur = *head;
    }
}
