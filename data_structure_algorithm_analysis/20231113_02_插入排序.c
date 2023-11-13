#include "pengmc.h"

void insertionSort(int arr[], int n) 
{
    int i, key, j;
    for (i = 1; i < n; i++) 
    {
        key = arr[i];
        j = i - 1;

        /* 将arr[i]移动到其在前面子数组中的正确位置 */
        while (j >= 0 && arr[j] > key) 
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/* 测试插入排序函数 */
int main() 
{
    int arr[] = {12, 11, 13, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    insertionSort(arr, n);
    print_array(arr, n);

    return 0;
}
