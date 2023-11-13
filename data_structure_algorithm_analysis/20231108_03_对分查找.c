#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OK             0
#define ERROR          -1

// #define _DEBUG_ 
#ifdef _DEBUG_
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif

#define NUMBER         100

void SWAP(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

#if (0)
int partition11(int *array, int left, int right)
{
    /*以最右侧为分割元素*/
    int pivot = array[right];

    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        /*当前元素小于分割元素*/
        if (array[j] <= pivot)
        {
            i++;
            SWAP(&array[i], &array[j]);
        }
    }
    SWAP(&array[i + 1], &array[right]);

    return (i + 1);
}
#endif

int partition(int a[], int left, int right)
{
    int i;
    /*用来记录大于标准的下标，默认认为第一个值是大于最后一个值，实际上并不重要*/
    int k = left;
    /*i遍历整个数组*/
    for(i = left; i < right; i++)
    {
        /*如果a[i]小于区分的数值，需要交换k和i的数值，每次发生交换，k移动一次*/
        if(a[i] < a[right])
        {
            SWAP(&a[k], &a[i]);
            k++;
        }
    }
    SWAP(&a[right], &a[k]);

    return k;
}

void quick_array(int *array, int left, int right)
{
    if (left < right)
    {
        int pi = partition(array, left, right);

        quick_array(array, left, pi - 1);
        quick_array(array, pi + 1, right);
    }
}

int main(void)
{
    srand(time(NULL));

    int i, array[NUMBER];

    for (i = 0; i < NUMBER; i++)
        array[i] = rand() % 1000;

    quick_array(array, 0, NUMBER - 1);

    for (i = 0; i < NUMBER; i++)
    {
        printf("%3d ", array[i]);

        if (0 == (i + 1) % 20)
            printf("\n");
    }

    int temp;
    printf("input a interger from 0 to %d: ", NUMBER);
    scanf("%d", &temp);
    if (temp > NUMBER || temp < 0)
    {
        printf("illegal input.\n");
        return ERROR;
    }

    int left = 0, right = NUMBER - 1, mid = (left + right) / 2;

    while (left <= right)
    {
        int mid  = (left + right ) / 2; 
        if (temp == array[mid])
            break;
        if (array[mid] < temp)
            left = mid + 1;
        else if (array[mid] > temp)
            right = mid - 1;
    }
    
    if (left > right)
        printf("Array does not exist the number.\n");
    else
        printf("The number is %d\n", mid);

    return 0;
}

