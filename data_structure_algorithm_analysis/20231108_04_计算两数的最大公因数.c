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

#define SWAP(A, B) {int temp; temp = A; A = B; B = temp;}

int gcd(int a, int b)
{
    if (!b)
        return a;
    
    return gcd(b, a % b);
}

int main(void)
{
    int M, N;
#if (0)
/*外部读入*/
    printf("输入两个正整数：");
    scanf("%d%d", &M, &N);
    if (M < 0 || N < 0)
    {
        printf("illegal input.\n");
        return ERROR;
    }

#endif

    /*自己生成*/
    srand(time(NULL));
    M = rand() % 100;
    N = rand() % 100;

    if (M < N)
        SWAP(M, N);

    int divisor = 1;

    divisor = gcd(M, N);

    printf("A: %d   B: %d  MAX divisor is %d\n", M, N, divisor);

    return 0;
}