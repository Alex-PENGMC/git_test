#include <stdio.h>

#define OK             1
#define ERROR          0

#define NUMBER         50

int main(void)
{
    int score[NUMBER];
    int tmp, i = 0, j, k;  // 开始从 i=0

    printf("Enter numbers (up to %d). Press Ctrl+D (Cmd+D on MacOS) to end:\n", NUMBER);
    while (scanf("%d", &tmp) != EOF && i < NUMBER)  // 使用 EOF 来检测输入结束
    {
        score[i++] = tmp;
    }

    // 使用嵌套循环来完全实现冒泡排序
    for (k = 0; k < i - 1; k++) {
        for (j = 0; j < i - k - 1; j++)
        {
            if (score[j] > score[j + 1])
            {
                tmp          = score[j];
                score[j]     = score[j + 1];
                score[j + 1] = tmp;
            }
        }
    }

    for (j = 0; j < i; j++)
        printf("%d ", score[j]);
    
    printf("\n");

    return 0;
}
