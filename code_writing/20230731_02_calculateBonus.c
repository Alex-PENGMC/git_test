#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int check_data(char* num)
{
    int i;

    for(i = 0; num[i]; i++)
    {
        printf("%c\t",  num[i]);
        if(num[i] > '9' || num[i] < '0')
            return -1;
    }

    return 0;
}


int main(void)
{
    char str[16];
    double bonus;

    printf("input your profit:\n");
    scanf("%s", str);
    if(-1 == check_data(str))
    {
        printf("your input is not profit:\n");
        goto error;
    }

    double input = atoi(str);

    if(input <= 10)
        bonus = input * 0.1;
    else if(input > 10 && input <= 20)
        bonus = 10 * 0.1 + (input - 10) * 0.075;
    else if (input > 20 && input <= 40)
        bonus = 10 * 0.1 + (20 - 10) * 0.075 + (input - 20) * 0.05;
    else if(input > 40 && input <= 60)
        bonus = 10 * 0.1 + (20 - 10) * 0.075 + (40 - 20) * 0.05 + (input - 40) * 0.03;
    else if(input > 60 && input <= 100)
        bonus = 10 * 0.1 + (20 - 10) * 0.075 + (40 - 20) * 0.05 + (60 - 40) * 0.03 + ((input - 60) * 0.0015);
    else
        bonus = 10 * 0.1 + (20 - 10) * 0.075 + (40 - 20) * 0.05 + (60 - 40) * 0.03 + ((100 - 60) * 0.0015) + (input - 100) * 0.01;    

    printf("your bonus is:%.4f\n", bonus);

    return 0;

error:

    return -1;
}