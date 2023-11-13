#include "pengmc.h"

typedef struct Node
{
    int value;
    struct Node *ppre;
    struct Node *pnext;
}Node_t;

void linklist_tail_insert(Node_t **head, Node_t **tail, int value);
void linklist_sort_insert(Node_t **head, Node_t **tail, int value);
bool linklist_delete(Node_t **head, Node_t **tail, int value);
void linklist_print(Node_t *head, Node_t *tail);

int main(void)
{
    Node_t *head, *tail;
    head = tail = NULL;

    int value;
#if (0)
    while (EOF != scanf("%d", &value))
    {
        /*采用尾插*/
        // linklist_tail_insert(&head, &tail, value);
        /*插入并排序*/
        linklist_sort_insert(&head, &tail, value);
        /*遍历节点*/
        linklist_print(head, tail);
    }
#endif

    srand(time(NULL));
    int array[10], i;

    for (i = 0; i < 5; i++)
    {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
        linklist_sort_insert(&head, &tail, array[i]);
    }
    printf("\n");

    linklist_print(head, tail);

    while (1)
    {
        printf("输入要删除的节点值：");
        scanf("%d", &value);
        linklist_delete(&head, &tail, value);
        linklist_print(head, tail);
    }
    

    return 0;
}

void linklist_tail_insert(Node_t **head, Node_t **tail, int value)
{
    Node_t *temp = (Node_t *)malloc(sizeof(Node_t));

    if (!temp)
    {
        printf("malloc error: ERROR CODE\n");
        return;
    }
    /*取值、尾指针指向头节点*/
    temp->value = value;

    if (*head == NULL)
    {
        *head = temp;
        *tail = temp;
        temp->pnext = temp;
        temp->ppre  = temp;
    }
    else
    {
        /*尾节点下一个节点指向新节点*/
        (*tail)->pnext = temp;
        /*新节点的前节点指向尾节点*/
        temp->ppre = *tail;
        /*新节点的尾节点指向头节点*/
        temp->pnext = *head;
        /*头节点的前节点指向新节点*/
        (*head)->ppre = temp;
        /*新节点成为新的尾节点*/
        *tail = temp;
    }
}

void linklist_sort_insert(Node_t **head, Node_t **tail, int value)
{
    Node_t *temp = (Node_t *)malloc(sizeof(Node_t));
    if (!temp)
    {
        printf("malloc error: ERRORCODE\n");
        return;
    }

    temp->value = value;
    if (!(*head))
    {
        *head = temp;
        *tail = temp;
        temp->pnext = temp;
        temp->ppre  = temp; 
    }
    else
    {    
        Node_t *pcur = *head;
        Node_t *ppre = NULL;
        /*查找插入位置*/
        while (pcur->value >= value && pcur->pnext != *head)
        {
            ppre = pcur;
            pcur = pcur->pnext;
        }

        if (pcur->value < value)
        {
            /*头插*/
            if (ppre == NULL)
            {
                temp->pnext    = *head;
                (*head)->ppre  = temp;

                (*tail)->pnext = temp;
                temp->ppre     = *tail;

                *head          = temp;
            }
            /*中间插*/
            else
            {
                /*处理后一节点关系*/
                temp->pnext = ppre->pnext;
                ppre->pnext->ppre = temp;
                /*处理前一节点关系*/
                temp->ppre  = ppre;
                ppre->pnext = temp;

                // ppre->pnext = temp;
                // temp->ppre  = ppre;

                // temp->pnext = pcur;
                // pcur->ppre  = temp;
            }
        }
        else
        {
            (*tail)->pnext = temp;
            temp->ppre     = *tail;
            temp->pnext    = *head;
            (*head)->ppre  = temp;
            /*额外需要更新尾节点*/
            *tail          = temp;
        }
    }
}

bool linklist_delete(Node_t **head, Node_t **tail, int value) 
{
    if (*head == NULL) 
    {
        // 链表为空
        printf("linklist is empty.\n");
        return false;
    }

    Node_t *pcur = *head;
    Node_t *ppre = NULL;
    bool found = false;

    do 
    {
        if (pcur->value == value) 
        {
            found = true;
            break;
        }
        ppre = pcur;
        pcur = pcur->pnext;
    } while (pcur != *head);

    if (!found) 
    {
        // 要删除的元素不在链表中
        printf("the element is not in linklist.\n");
        return false;
    }

    if (*head == *tail && pcur == *head) 
    {
        // 链表只有一个节点
        free(pcur);
        *head = *tail = NULL;
    } 
    else if (pcur == *head) 
    {
        // 删除头节点
        (*tail)->pnext = pcur->pnext;
        pcur->pnext->ppre = *tail;
        *head = pcur->pnext;
        free(pcur);

    } 
    else if (pcur == *tail) 
    {
        // 删除尾节点
        ppre->pnext = *head;
        (*head)->ppre = ppre;
        *tail = ppre;
        free(pcur);
    } 
    else 
    {
        // 删除中间节点
        ppre->pnext = pcur->pnext;
        pcur->pnext->ppre = ppre;
        free(pcur);
    }

    return true;
}


void linklist_print(Node_t *head, Node_t *tail)
{
    if (head != NULL) 
    {
        Node_t *pcur = head;
        do 
        {
            printf("%d ", pcur->value);
            pcur = pcur->pnext;
        } while (pcur != head);
        printf("\n");
    }
    else
        printf("Linklist is empty11.\n");
}

