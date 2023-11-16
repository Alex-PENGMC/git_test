#include "../pengmc.h"

#define N  20

void adjustMaxHeap(int *array, int adjustPos, int arrLen);
void arrHeap(int *array);


int main(void)
{
    srand(time(NULL));

    int i, array[N];

    for (i = 0; i < N; i++)
        array[i] = rand() % 100;

    PRINT_ARRAY(array, N);
    LINE_FEED;
    
    arrHeap(array);

    PRINT_ARRAY(array, N);


    return 0;
}

void adjustMaxHeap(int *array, int adjustPos, int arrLen)
{
    int dad = adjustPos;
    int son = 2 * dad + 1;

    while (son < arrLen)
    {
        if (son + 1 < arrLen && array[son] < array[son + 1])
            son++;

        if (array[dad] < array[son])
        {
            SWAP(array[dad], array[son]);
            dad = son;
            son = 2 * dad + 1;
        }
        else
            break;
    }
}

void arrHeap(int *array)
{
    int i;
    for (i = N / 2 - 1; i >= 0; i--)
        adjustMaxHeap(array, i, N);

    SWAP(array[0], array[N - 1]);

    for (i = N - 1; i > 1; i--)
    {
        adjustMaxHeap(array, 0, i);
        SWAP(array[0], array[i - 1]);
    }
}


