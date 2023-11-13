#include <stdio.h>

int sum(int, int);
int product(int, int);
int difference(int, int);

int main(void)
{
    int a = 10, b = 5, result = 0;
    int (*pfun[3])(int, int);

    pfun[0] = sum;
    pfun[1] = product;
    pfun[2] = difference;

    for ( int i = 0; i < 3; i++)
    {
        result = pfun[i](a, b);
        printf("\nresult = %d", result);
    }

    result = pfun[1](pfun[0](a, b), pfun[2](a, b));
    printf("\n\nThe product of the sum and the difference = %d\n", result);

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
