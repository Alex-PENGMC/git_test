#include <stdio.h>

typedef struct node
{
    int value;
    struct node *next;
}node_t;

int main(void)
{
    int c;
    node_t *phead, *pcurr, *ptemp;

    phead = NULL;
    while (stdin == '\n')
    {
        scanf("%d", &c);

        ptemp = (node_t*)malloc(sizeof(struct node));
        ptemp->value = c;
        ptemp->next  = NULL;

        if (phead)
            phead = ptemp;
        else
        {
            pcurr->next = ptemp;
            
        }
        
    }
    
}
