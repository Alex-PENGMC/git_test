#include "pengmc.h"


#define TABLE_SIZE 10

// Node structure for each entry in the hash table
typedef struct 
{
    int key;
    int value;
} hash_table_item;

// Hash table array
hash_table_item* hash_table[TABLE_SIZE];

// Simple hash function
int hash_function(int key) 
{
    return key % TABLE_SIZE;
}

// Initialize the hash table
void init_hash_table() 
{
    for (int i = 0; i < TABLE_SIZE; i++) 
        hash_table[i] = NULL;
}

// Insert into the hash table
void insert(int key, int value) 
{
    int index = hash_function(key);
    hash_table_item *item = (hash_table_item*) malloc(sizeof(hash_table_item));
    item->key = key;
    item->value = value;
    hash_table[index] = item;
}

// Search in the hash table
int search(int key) 
{
    int index = hash_function(key);
    if (hash_table[index] != NULL && hash_table[index]->key == key)
        return hash_table[index]->value;

    return -1;
}

// Main function to demonstrate hash table usage
int main() 
{
    init_hash_table();
    insert(1, 10);
    insert(2, 20);
    insert(3, 30);

    DBG_PRINTF("Value at key 2 is %d\n", search(2));
    return 0;
}
