#include "pengmc.h"

typedef struct Node
{
    int value;
    struct Node *pnext;
} Node_t;

bool create_linklist(Node_t **head, Node_t **tail, int value);
bool linklist_print(Node_t *head);
Node_t* findDriver(Node_t **head, Node_t **ppre);
bool exchange_two_elements(Node_t **head, Node_t **tail, int value1, int value2);

int main(void)
{
    srand(time(NULL));
    int array[10], i;

    Node_t *head, *tail;
    head = tail = NULL;

    for (i = 0; i < 10; i++)
    {
        array[i] = rand() % 100;
        DBG_PRINTF("%d ", array[i]);

        create_linklist(&head, &tail, array[i]);
    }
    DBG_PRINTF("\n遍历链表:\n");

    // linklist_print(head);

    DBG_PRINTF("\n请输入要交换的两个相邻元素值:\n");
    int c, d;
    scanf("%d%d", &c, &d);
    exchange_two_elements(&head, &tail, c, d);
    DBG_PRINTF("\n遍历链表2:\n");

    linklist_print(head);
    DBG_PRINTF("\n");

    return 0;
}


bool create_linklist(Node_t **head, Node_t **tail, int value)
{
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));

    if (newNode == NULL)
    {
        DBG_PRINTF("malloc error code\n");
        return ERROR;
    }

    newNode->value = value;
    newNode->pnext = NULL;

    if (*head == NULL)
    {
        *head = newNode;
        *tail = newNode;
    }
    else
    {
        (*tail)->pnext = newNode;
        *tail          = newNode;
    }
    
    return OK;
}

bool linklist_print(Node_t *head)
{
    if (head == NULL)
    {
        DBG_PRINTF("linklist is empty.\n");
        return OK;
    }

    Node_t *pcur = head;
    while (pcur)
    {
        DBG_PRINTF("%d ", pcur->value);
        pcur = pcur->pnext;
    }
    
    return OK;
}

bool exchange_two_elements11(Node_t **head, Node_t **tail, int value1, int value2)
{
    Node_t *pcur = *head;
    Node_t *ppre = NULL;

    if (*head == NULL || (*head)->pnext == *tail)
    {
        DBG_PRINTF("ERROR_CODE\n");
        return ERROR;
    }

    while (pcur != NULL)
    {
        ppre = pcur;
        pcur = pcur->pnext;

        if (ppre->value == value1 && pcur->value == value2)
            break;
    }
    /*更新头节点*/
    if (ppre == *head)
    {
        ppre->pnext = pcur->pnext;
        pcur->pnext = ppre;
        *head       = pcur;
    }
    /*更新尾节点*/
    else if (pcur == *tail)
    {
        /*找到ppre的前驱节点， 这里假定为driver*/
        Node_t *pfront = findDriver(head, &ppre);
        pfront->pnext  = pcur;
        pcur->pnext    = ppre;
        *tail          = ppre;
    }
    /*更新中间节点*/
    else
    {
        Node_t *pfront = findDriver(head, &ppre);
        pfront->pnext  = pcur;
        ppre->pnext    = pcur->pnext;
        pcur->pnext    = ppre;
    }
    
    return OK;
}

Node_t* findDriver(Node_t **head, Node_t **ppre)
{
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));
    Node_t *pcur    = *head;

    /*链表为空或者寻找头结点的前驱节点，均为错误*/
    if (*head == NULL || *head == *ppre)
    {
        DBG_PRINTF("ERROR_CODE\n");
        return NULL;
    }

    while (pcur == *ppre)
    {
        newNode = pcur;
        pcur    = pcur->pnext;
    }
    
    if (pcur == NULL)
    {
        DBG_PRINTF("ERROR_CODE   没找到\n");
        return NULL;
    }

    return newNode;
}



bool exchange_two_elements(Node_t **head, Node_t **tail, int value1, int value2) 
{
    if (*head == NULL || (*head)->pnext == NULL) 
    {
        DBG_PRINTF("链表太短，无法交换。\n");
        return ERROR;
    }

    Node_t *pcur = *head, *ppre = NULL;

    while (pcur->pnext != NULL && !(pcur->value == value1 && pcur->pnext->value == value2)) 
    {
        ppre = pcur;
        pcur = pcur->pnext;
    }

    if (pcur->pnext == NULL) 
    {
        DBG_PRINTF("未找到相邻的元素进行交换。\n");
        return ERROR;
    }

    Node_t *nextNode = pcur->pnext;
    pcur->pnext = nextNode->pnext;
    nextNode->pnext = pcur;

    if (ppre == NULL) 
    { // pcur 是头节点
        *head = nextNode;
    } 
    else 
    {
        ppre->pnext = nextNode;
    }

    if (nextNode->pnext == NULL) 
    { // nextNode 现在是尾节点
        *tail = nextNode;
    }

    return OK;
}


