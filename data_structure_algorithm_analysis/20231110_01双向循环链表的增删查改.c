#include "pengmc.h"

// 定义双向循环链表的节点结构
typedef struct Node 
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

// 创建新节点
Node* createNode(int data) 
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) 
    {
        return NULL;
    }
    newNode->data = data;
    newNode->prev = newNode;
    newNode->next = newNode;
    return newNode;
}

// 插入节点到链表的头部
void insertHead(Node** head, int data) 
{
    Node* newNode = createNode(data);
    if (!newNode) 
    {
        return;
    }
    if (*head == NULL) 
    {
        *head = newNode;
    } 
    else 
    {
        newNode->next = *head;
        newNode->prev = (*head)->prev;
        (*head)->prev->next = newNode;
        (*head)->prev = newNode;
        *head = newNode;
    }
}

// 删除节点
void deleteNode(Node** head, int key) 
{
    Node* temp = *head;
    Node* toBeDeleted = NULL;
    if (*head == NULL) 
    {
        return;
    }

    do 
    {
        if (temp->data == key) 
        {
            toBeDeleted = temp;
            break;
        }
        temp = temp->next;
    } while (temp != *head);

    if (toBeDeleted) 
    {
        toBeDeleted->prev->next = toBeDeleted->next;
        toBeDeleted->next->prev = toBeDeleted->prev;

        if (toBeDeleted == *head) 
        {
            *head = (*head)->next;
        }
        if (toBeDeleted == *head) 
        {
            *head = NULL;
        }
        free(toBeDeleted);
    }
}

// 查找节点
Node* findNode(Node* head, int key) 
{
    Node* temp = head;
    if (head == NULL) 
    {
        return NULL;
    }
    do 
    {
        if (temp->data == key) 
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != head);
    return NULL;
}

// 修改节点数据
void updateNode(Node* head, int oldKey, int newKey) 
{
    Node* toBeUpdated = findNode(head, oldKey);
    if (toBeUpdated) 
    {
        toBeUpdated->data = newKey;
    }
}

// 打印链表
void printList(Node* head) 
{
    Node* temp = head;
    if (head == NULL) 
    {
        return;
    }
    do 
    {
        DBG_PRINTF("%d ", temp->data);
        temp = temp->next;
    } while (temp != head);
    DBG_PRINTF("\n");
}

// 主函数
int main() 
{
    Node* head = NULL;

    insertHead(&head, 10); // 插入节点
    insertHead(&head, 20);
    insertHead(&head, 30);
    printList(head); // 打印链表

    deleteNode(&head, 20); // 删除节点
    printList(head);

    updateNode(head, 10, 100); // 修改节点数据
    printList(head);

    return 0;
}
