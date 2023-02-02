#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_t
{
    char c;
    struct tree_t* pleft;
    struct tree_t* pright;
}Tree_t, *pTree_t;

typedef struct queue_t
{
    pTree_t insert_pos;
    struct queue_t* pnext;
}Queue_t, *pQueue_t;

void build_binary_tree(pTree_t* root_tree, pQueue_t* queue_head, pQueue_t* queue_tail, char c)
{
    pTree_t tree_new = (pTree_t)calloc(1, sizeof(Tree_t));
    tree_new->c = c;

    pQueue_t queue_new = (pQueue_t)calloc(1, sizeof(Queue_t));
    queue_new->insert_pos = tree_new;
    pQueue_t queue_cur = *queue_head;

    if(NULL == *root_tree)
    {
        *root_tree = tree_new;
        *queue_head = queue_new;
        *queue_tail = queue_new;
    }
    else
    {
        (*queue_tail)->pnext = queue_new;
        *queue_tail = queue_new;
        if(NULL == (*queue_head)->insert_pos->pleft)
        {
            (*queue_head)->insert_pos->pleft = tree_new;
        }
        else if(NULL == (*queue_head)->insert_pos->pright)
        {
            (*queue_head)->insert_pos->pright = tree_new;
            *queue_head = queue_cur->pnext;
            free(queue_cur);
        }
    }
}

void preorder(pTree_t root_tree)
{
    if(root_tree)
    {
        putchar(root_tree->c);
        preorder(root_tree->pleft);
        preorder(root_tree->pright);
    }
}

void midorder(pTree_t root_tree)
{
    if(root_tree)
    {
        midorder(root_tree->pleft);
        putchar(root_tree->c);
        midorder(root_tree->pright);
    }
}

void latorder(pTree_t root_tree)
{
    if(root_tree)
    {
        latorder(root_tree->pleft);
        latorder(root_tree->pright);
        putchar(root_tree->c);
    }
}

int main_20230201_buildbinarytree()
// int main()
{
    pTree_t root_tree = NULL;
    pQueue_t queue_head = NULL, queue_tail = NULL;

    char c;

    while(scanf("%c", &c) != EOF)
    {
        if(c == '\n')
            break;
        build_binary_tree(&root_tree, &queue_head, &queue_tail, c);
    }

    preorder(root_tree);
    printf("\n******************\n");
    midorder(root_tree);
    printf("\n******************\n");
    latorder(root_tree);
    printf("\n******************\n");

    return 0;
}
