#include "../pengmc.h"

typedef struct node 
{
    int key;
    int value;
    int hash;
    struct node* next;
} node_t;

typedef struct 
{
    int size;
    int count;
    node_t** items;
} hashtable;

uint hash_function(int key, int size) 
{
    return (unsigned)key % size;
}

node_t* create_node(int key, int value, int hash) 
{
    /*申请节点空间*/
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    
    if (!new_node)
    {
        MALLOC_ERROR_FILE_FUNC_LINE;
        return ERROR;
    }

    /*节点填值*/
    new_node->key    = key;
    new_node->value  = value;
    new_node->hash   = hash;
    new_node->next   = NULL;

    return new_node;
}

hashtable* create_table(int size) 
{
    hashtable* table = (hashtable*) malloc(sizeof(hashtable));
    table->size = size;
    table->count = 0;
    table->items = (node_t**) malloc(sizeof(node_t*) * size);
    for (int i = 0; i < size; i++) 
        table->items[i] = NULL;

    return table;
}

void insert(hashtable* table, int key, int value) 
{
    int hash = hash_function(key, table->size);
    node_t* new_node = create_node(key, value, hash);

    if (table->items[hash] == NULL) 
    {
        // No collision, directly insert the item
        table->items[hash] = new_node;
    } 
    else 
    {
        // Handle collision with chaining
        node_t* current = table->items[hash];
        while (current->next != NULL) 
            current = current->next;

        current->next = new_node;
    }
    table->count++;
}

int search(hashtable* table, int key) 
{
    int hash = hash_function(key, table->size);
    node_t* current = table->items[hash];
    while (current != NULL) 
    {
        if (current->key == key) 
            return current->value;

        current = current->next;
    }
    return -1; // Not found
}

int main() 
{
    hashtable* table = create_table(10);

    insert(table, 1, 100);
    insert(table, 2, 200);
    insert(table, 11, 300); // This will cause a collision with key 1

    printf("Value for key 1: %d\n", search(table, 1));
    printf("Value for key 2: %d\n", search(table, 2));
    printf("Value for key 11: %d\n", search(table, 11));

    return 0;
}
