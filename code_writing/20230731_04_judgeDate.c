#include <stdio.h>

typedef struct date
{
    int year;
    int month;
    int day;
}date_t;

enum Month
{
    Jan = 1,
    Feb = 2,
    Mar = 3,
    Apr = 4,
    May = 5,
    Jun = 6,
    Jul = 7,
    Aug = 8,
    Sep = 9,
    Oct = 10,
    Nov = 11,
    Dec = 12
};

int main(void)
{
    date_t input_day;
    printf("input date for example:2023 05 15\n");
    scanf("%d%d%d", &input_day.year, &input_day.month, &input_day.day);

    if(input_day.year < 0)
        return -1;
    
    int month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(input_day.year % 400 == 0 || (input_day.year % 4 == 0 && input_day.year % 100 != 0))
        month[1] = 29;

    // if(input_day.month < 1 || input_day.month > 12)
    //     return -2;
    
    switch(input_day.month)
    {
        case Jan:
        case Mar:
        case May:
        case Jul:
        case Aug:
        case Oct:
        case Dec:
            if(input_day.day < 1 || input_day.day > 31)
                return -2;
            break;
        case Apr:
        case Jun:
        case Sep:
        case Nov:
            if(input_day.day < 1 || input_day.day > 30)
                return -3;
            break;
        case Feb:
            if(input_day.year % 400 == 0 || (input_day.year % 4 == 0 && input_day.year % 100 != 0))
            {
                if(input_day.day < 1 || input_day.day > 29)
                    return -4;
            }
            else if(input_day.day < 1 || input_day.day > 28)
                    return -5;
            break;
        default:
            return -6;
            break;
    }

    int i, sum = 0;
    for(i = 0; i < input_day.month - 1; i++)
        sum += month[i];

    sum += input_day.day;

    printf("%d\n", sum);

    return 0;
}