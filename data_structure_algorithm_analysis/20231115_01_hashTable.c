#include "../pengmc.h"

// 定义哈希表节点
typedef struct node 
{
    int key;
    int value;
    struct node *next;
} node_t;

// 定义哈希表结构
typedef struct hashtable 
{
    int size;
    int count;
    node_t **items;
} hashtable_t;

// 简单的哈希函数
unsigned int hash_function(int key, int size) 
{
    return (unsigned)key % size;
}

// 创建节点
node_t* create_node(int key, int value) 
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }
    new_node->key   = key;
    new_node->value = value;
    new_node->next  = NULL;

    return new_node;
}

// 创建哈希表
hashtable_t* create_table(int size) 
{
    hashtable_t *table = malloc(sizeof(hashtable_t));
    if (table == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory for new table\n");
        exit(-1);
    }

    table->size  = size;
    table->count = 0;
    table->items = calloc(table->size, sizeof(node_t*));
    if (table->items == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory for items\n");
        exit(-1);
    }

    return table;
}

// 插入函数
void insert(hashtable_t *table, int key, int value) 
{
    /*key-value，其中的key是索引，value是需要存储的数值，
      一般做法是根据key生成index，index为数组下标*/
    /*根据key-value创建新节点，并将新节点插入到hash中*/
    int index = hash_function(key, table->size);
    node_t *new_node = create_node(key, value);

    if (table->items[index] == NULL) 
        // 如果没有冲突，直接插入
        table->items[index] = new_node;
    else 
    {
        // 处理冲突：在链表头部插入
        new_node->next = table->items[index];
        table->items[index] = new_node;
    }
    table->count++;
}

// 搜索函数
int search(hashtable_t *table, int key) 
{
    int index = hash_function(key, table->size);
    node_t *current = table->items[index];
    while (current != NULL) 
    {
        if (current->key == key) 
            return current->value;

        current = current->next;
    }
    return -1; // 未找到
}

// 删除函数
void delete(hashtable_t *table, int key) 
{
    int index = hash_function(key, table->size);
    node_t *current = table->items[index];
    node_t *prev = NULL;

    while (current != NULL && current->key != key) 
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL) 
        return; // 未找到

    if (prev == NULL) 
        // 删除的是第一个节点
        table->items[index] = current->next;
    else 
        // 删除的是后续节点
        prev->next = current->next;

    free(current);
    table->count--;
}

// 清理哈希表
void free_table(hashtable_t *table) 
{
    for (int i = 0; i < table->size; i++) 
    {
        node_t *current = table->items[i];
        while (current != NULL) 
        {
            node_t *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(table->items);
    free(table);
}

// 主函数
int main() 
{
    hashtable_t *table = create_table(10);

    insert(table, 1, 100);
    insert(table, 2, 200);
    insert(table, 11, 300); // 这将造成和键1的冲突

    printf("Value for key 1: %d\n", search(table, 1));
    printf("Value for key 2: %d\n", search(table, 2));
    printf("Value for key 11: %d\n", search(table, 11));

    delete(table, 1);
    printf("Value for key 1 after deletion: %d\n", search(table, 1));

    free_table(table);

    return 0;
}


