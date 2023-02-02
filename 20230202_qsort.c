#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10
#define M 100

int compare(const void* left, const void* right)
{
    int* p1 = (int*)left;
    int* p2 = (int*)right;

    if(*p1 > *p2)
        return 1;
    else if(*p1 < *p2)
        return -1;
    else  
        return 0;
}

void arr_print(int* arr)
{
    int i;
    for(i = 0; i < N; i++)
    {
        printf("%3d", arr[i]);
    }
    puts("");
}

int main_20230202_qsort()
// int main()
{
    int i;
    int* arr = (int*)malloc(N * sizeof(int));

    time_t start, end;
    srand(time(NULL));
    for(i = 0; i < N; i++)
    {
        arr[i] = rand() % M;
    }

    arr_print(arr);
    start = time(NULL);
    qsort(arr, N, sizeof(int), compare);
    end = time(NULL);
    arr_print(arr);
    printf("use time = %ld\n", end - start);

    return 0;
}
