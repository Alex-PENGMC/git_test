#include <stdio.h>

#define SIZE 128

int reverse_string(char* start, char* end)
{
    char tmp;
    int length = (int)(end - start);
    for (int i = 0; i < length / 2; i++)
    {
        tmp                   = start[i];
        start[i]              = start[length - 1 - i];
        start[length - 1 - i] = tmp;
    }
    return 0;
}

void delete_unexpected_space(char* start, char* end)
{
    int i = 0, j = 0, count = 0;
    int length = (int)(end - start);

    for(i = 0; i < length; i++)
    {
        if((start[i] == ' ') && (start[i + 1] == ' '))
        {
            for(j = i + 1; j < length; j++)
            {
                start[j] = start[j + 1];
            }
            i--;
        }
    }
    if(start[0] == ' ')
    {
        for(j = 0; j < length; j++)
        {
            start[j] = start[j + 1];
        }
    }
}

int main(void)
{
    char str[SIZE];
    char ch;
    int length = 0, i = 0;
    while((ch = getchar()) != '\n')
    {
        str[length++] = ch;
    }

    reverse_string(str, str + length);

    delete_unexpected_space(str, str + length);

    printf("%s\n", str);

    return 0;
}

//          i    am      from    china   hello   world!   