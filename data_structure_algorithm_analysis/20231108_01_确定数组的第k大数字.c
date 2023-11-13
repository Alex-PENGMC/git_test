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

#define NUMBER      20

/*一般放.h文件，供外部使用*/
#define SWAP(A, B)  \
{\
    int temp; \
    temp = A; \
    A = B; \
    B = temp;\
}

void quick_sort(int *array, int left, int right);

int partition(int *array, int left, int right);


int main(void)
{
    int array[NUMBER], i;
    srand(time(NULL));

    for (i = 0; i < NUMBER; i++)
    {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
    }
    
    printf("\n");
/*从小到大排序*/
#if (0)
    int j, temp, c;
    for (i = 0; i < NUMBER - 1; i++)
    {
        for (j = i + 1; j < NUMBER; j++)
        {
            if (array[i] > array[j])
                SWAP(array[i], array[j])
        }
    }

    scanf("%d", &c);
    if (c < 0 || c > 1000)
    {
        printf("非法输入。\n");
        return ERROR;
    }
    printf("第K大的数字是: %d\n", array[NUMBER - c]);

#endif

/*法二，先读入k个整数进行降序*/
#if (1)
    int K, j, temp;
    printf("请输入你想要的第K大数字:\n");

    scanf("%d", &K);
    /*输入验证*/
    if (K < 0 || K > NUMBER)
    {
        printf("非法输入。\n");
        return ERROR;
    }
        
    int result[K];

    /*将前K个数字放入数组中*/
    for (i = 0; i < K; i++)
        result[i] = array[i];
    /*对读入数组进行降序排序*/
    quick_sort(result, 0, K - 1);

    for (i = K; i < NUMBER; i++)
    {
        if (array[i] > result[K - 1])
        {
            j = K - 1;
            /*非常精妙的想法，注意这里的边界条件*/
            while (j > 0 && array[i] > result[j - 1])
            {
                /*避免存在越界问题*/
                result[j] = result[j - 1];
                j--;
#if (0)
                for (int mm = 0; mm < K; mm++)
                    printf("%d ", result[mm]);

                printf("\n");
#endif
            }
            result[j] = array[i];
        }
    }

    printf("\n");

    for (i = 0; i < K; i++)
        printf("%d ", result[i]);

    printf("\n第%d大的数字是: %d\n", K, result[K - 1]);

#endif

    return 0;
}

void quick_sort(int *array, int left, int right)
{
    if (left < right)
    {
        int pi = partition(array, left, right);

        quick_sort(array, left, pi - 1);
        quick_sort(array, pi + 1, right);
    }
}

int partition(int *array, int left, int right)
{
    int k = left, i;
    /*这里i不能等于right，因为是拿右边来做分割数*/
    for (i = left; i < right; i++)
    {
        /*下面的小于号和大于号分别表示递增和递减*/
        if (array[i] > array[right])
        {
            SWAP(array[k], array[i]);
            k++;
       }
    }
    SWAP(array[k], array[right]);

    return k;
}

