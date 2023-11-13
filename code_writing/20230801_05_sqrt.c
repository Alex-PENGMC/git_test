#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SWAP(a, b) {int temp; temp = b; b = a; a = temp;}

int judge_input_integer(char* arr)
{
    int i;
    for(i = 0; arr[i]; i++)
    {
        if(arr[i] > '9' || arr[i] < '0')
            return -1;
    }


    return atoi(arr);
}


int main(void)
{
    char num_A[10], num_B[10], num_C[10];
    scanf("%s%s%s", num_A, num_B, num_C);

    int num[3];
    num[0] = judge_input_integer(num_A);
    num[1] = judge_input_integer(num_B);
    num[2] = judge_input_integer(num_C);

    int i;    
    for(i = 0; i < 3; i++)
    {
        if(-1 == num[i])
            printf("input is err");
    }

    if(num[0] > num[1])
        SWAP(num[0], num[1]);
    if(num[0] > num[2])
        SWAP(num[0], num[2]);
    if(num[1] > num[2])
        SWAP(num[1], num[2]);

    printf("min:%d  %d  max:%d\n", num[0], num[1], num[2]);
}
