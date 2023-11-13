#include <stdio.h>
#include <math.h>

int judge_square_number(int number)
{
    int square_root = (int)sqrt(number);

    if(number == square_root * square_root)
        return 0;

    return -1;
}

int main(void)
{
    int i;
    for(i = 0; i < 1000; i++)
    {
        if(0 == judge_square_number(i + 100))
        {
            if(0 == judge_square_number(i + 100 + 168))
                printf("%d\n", i);
            continue;
        }
    }
}
