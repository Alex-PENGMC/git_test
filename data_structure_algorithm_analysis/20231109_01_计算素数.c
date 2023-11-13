#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define MAX_NUMBER 100

int main(void) 
{
    bool isPrime;
    int primes[MAX_NUMBER / 2]; // 由于质数的数量大约是总数的一半，这里分配足够的空间
    int index = 0; // 质数数组的索引

    // 初始化2是第一个质数
    primes[index++] = 2;

    // 检查3到MAX_NUMBER之间的每个奇数是否为质数
    for (int num = 3; num < MAX_NUMBER; num += 2) 
    {
        isPrime = true;
        // 检查是否有小于等于num平方根的数能整除num
        for (int div = 3; div <= sqrt(num); div += 2) 
        {
            if (num % div == 0) 
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            primes[index++] = num;
    }

    // 输出质数数组
    printf("100以内的质数有：\n");
    for (int i = 0; i < index; ++i)
        printf("%d ", primes[i]);

    printf("\n");

    return 0;
}
