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


#define MAX_ARRAY_SIZE 10

int main() 
{
    int array[MAX_ARRAY_SIZE];
    int maxSum = 0; // 最大子序列和
    int currentSum = 0; // 当前子序列和

    srand(time(NULL));

    // 随机生成MAX_ARRAY_SIZE个包含正负数的数组元素
    for (int i = 0; i < MAX_ARRAY_SIZE; i++)
        array[i] = rand() % 200 - 100; // 生成范围在-100到99之间的随机数

    // 计算最大连续子序列和
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) 
    {
        currentSum = currentSum + array[i];

        // 如果当前子序列和小于0，重置为0
        if (currentSum < 0)
            currentSum = 0;

        // 更新最大子序列和
        if (currentSum > maxSum) 
            maxSum = currentSum;
    }

    // 打印生成的数组
    printf("生成的随机数组为:\n");
    for (int i = 0; i < MAX_ARRAY_SIZE; i++)
        printf("%d ", array[i]);

    // 打印最大子序列和
    printf("\n最大连续子序列和为: %d\n", maxSum);

    return 0;
}
