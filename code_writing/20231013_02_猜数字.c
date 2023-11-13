#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE           1
#define FALSE          0
#define OK             1
#define ERROR          0
#define INFEASIBLE    -1
#define OVERFLOW      -2

typedef int Status;

int main(void)
{
    printf("Let's play a game called guess number."
            "Please input your number between 0 to 10000: ");
    char num_ori[16];
    scanf("%s", num_ori);

    int i;
    for (i = 0; i < strlen(num_ori); i++)
    {
        if (!isdigit(num_ori[i]))
        {
            printf("Your input is not a digit.\n");
            return -1;
        }
    }
    
    int num = atoi(num_ori);

    if (num < 0 || num > 10000)
        printf("Your input is out of the range.\n");

    int left = 0, right = 10000, guess = (left + right) / 2;
    int count = 0;
    char answer;

    while (TRUE)
    {
        printf("I guess your input number is %d.\n", guess);

        if (guess == num)
        {
            printf("Finally, I win, take %d times.", count);
            break;
        }
        else
            printf("Your input is bigger than my guess number or little (y/n):\n");
label:
        scanf(" %c", &answer);

        if ('y' == tolower(answer))
            left = guess;
        else if ('n' == tolower(answer))
            right = guess;
        else
        {
            printf("Your inpur is not 'y' or 'n', Please input again.\n");
            goto label;
        }

        guess = (left + right) / 2;
        count++;
    }

    return 0;
}