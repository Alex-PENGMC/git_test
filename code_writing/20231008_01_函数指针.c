#include <stdio.h>

int sum(int, int);
int product(int, int);
int difference(int, int);

int main(void)
{
    int a = 10, b = 5, result = 0;
    int (*pfun)(int, int);

    pfun = sum;
    result = pfun(a, b);
    printf("\npfun = sum result = %d", result);

    pfun = product;
    result = pfun(a, b);
    printf("\npfun = product result = %d", result);

    pfun = difference;
    result = pfun(a, b);
    printf("\npfun = difference result = %d\n", result);

    return 0;
}

int sum(int x, int y)
{
    return x + y;
}

int product(int x, int y)
{
    return x * y;
}

int difference(int x, int y)
{
    return x - y;
}
