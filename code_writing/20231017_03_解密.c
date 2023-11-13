#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main1(void)
{
    char num[20];
    strcpy(num, "631758924");
    int i;

    char qq_num[20] = {0};

    for (i = 0; i < 9; i++) 
    {
        if (1 == i % 2)
        {
            strcat(qq_num, &num[i]);

        }
    }


    printf("1:%s\n", qq_num);
    printf("2:%d ", atoi(qq_num));

    printf("\n");

    return 0;
}

int main(void)
{
    int a;

    // printf("%ld  %ld\n", sizeof(int), sizeof(a));

    return 0;
}