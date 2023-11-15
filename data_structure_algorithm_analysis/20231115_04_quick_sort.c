#include "../pengmc.h"

#define NUMBER          (20 )
#define MAX_NUMBER      (100)

int partition(int *array, int left, int right);
void quick_sort(int *array, int left, int right);

int main(void)
{
    srand(time(NULL));
    
    int i, array[NUMBER];

    for (i = 0; i < NUMBER; i++)
    {
        array[i] = rand() % MAX_NUMBER;
        DBG_PRINTF("%d ", array[i]);
    }

    DBG_PRINTF("\n");

    quick_sort(array, 0, NUMBER - 1);

    PRINT_ARRAY(array, NUMBER);

    return 0;
}

void quick_sort(int *array, int left, int right)
{
    if (left < right)
    {
        int par = partition(array, left, right);
        quick_sort(array, left, par - 1);
        quick_sort(array, par + 1, right);
    }
}

int partition(int *array, int left, int right)
{
    int k = left;
    int i;

    for (i = left; i <= right; i++)
    {
        if (array[i] < array[right])
        {
            SWAP(array[i], array[k]);
            k++;
        }
    }

    SWAP(array[right], array[k]);

    return k;
}