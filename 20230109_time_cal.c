#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SWAP(a, b) {int s; s = a; a = b; b = s;} //交换两个数值
typedef struct
{
    int year;
    int month;
    int date;
}alex;

#define ERR_START_YEAR  0x01
#define ERR_START_MONTH 0x02
#define ERR_START_DATE  0x03
#define ERR_END_YEAR    0x11
#define ERR_END_MONTH   0x12
#define ERR_END_DATE    0x13

int year_cur, month_cur, date_cur, month_date[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int count[] = {0, 0, 0};

alex start_time, end_time;

void scanf_input()
{
    printf("Please input start and end time: \n");
    printf("For example:\ntime1: 2018 1 2\ntime2: 2019 9 5\n");
    printf("*******************\n");
    printf("time1: ");
    scanf("%d%d%d", &start_time.year, &start_time.month, &start_time.date);
    printf("time2: ");
    scanf("%d%d%d", &end_time.year, &end_time.month, &end_time.date);
}

int check_input()
{
    if(start_time.year < 1)
        return ERR_START_YEAR;

    if(end_time.year < 1)
        return ERR_END_YEAR;

    switch (start_time.month) //判断time1的日期是否会超
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(start_time.date > 31 || start_time.date < 1)
                return ERR_START_DATE;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(start_time.date > 30 || start_time.date < 1)
                return ERR_START_DATE;
            break;
        case 2:
            if(start_time.year % 4 == 0 && start_time.year % 100 != 0 || start_time.year % 400 == 0)
            {
                if(start_time.date > 29 || start_time.date < 1)
                    return ERR_START_DATE;
            }
            else
            {
                if(start_time.date > 28 || start_time.date < 1)
                    return ERR_START_DATE;
            }
            break;
        default:
            return ERR_START_MONTH;
            break;
    }

    switch (end_time.month) //判断time2的日期是否会超
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(end_time.date > 31 || end_time.date < 1)
                return ERR_END_DATE;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(end_time.date > 30 || end_time.date < 1)
                return ERR_END_DATE;
            break;
        case 2:
            if(end_time.year % 4 == 0 && end_time.year % 100 != 0 || end_time.year % 400 == 0)
            {
                if(end_time.date > 29 || end_time.date < 1)
                    return ERR_END_DATE;
            }
            else
            {
                if(end_time.date > 28 || end_time.date < 1)
                    return ERR_END_DATE;
            }
            break;
        default:
            return ERR_END_MONTH;
            break;
    }

    return 0;
}

void exchange_year_month_date()
{
    /*年数1大于年数2，全部交换*/
    if(start_time.year > end_time.year)
    {
        SWAP(start_time.year, end_time.year);
        SWAP(start_time.month, end_time.month);
        SWAP(start_time.date, end_time.date);
    }
    /*年数1等于年数2*/
    if(start_time.year == end_time.year)
    {
        /*月数1大于月数2，交换月数及日期*/
        if(start_time.month > end_time.month)
        {
            SWAP(start_time.month, end_time.month);
            SWAP(start_time.date, end_time.date);
        }
        /*同月且日期1大于日期2，仅交换日期*/
        if(start_time.month == end_time.month && start_time.date > end_time.date)
            SWAP(start_time.date, end_time.date);
    }
}

int calcu_difference()
{
    /* 起始年到中止年前一年总天数 */ 
    for(year_cur = start_time.year; year_cur < end_time.year; year_cur++) 
    {
        if(year_cur % 4 == 0 && year_cur % 100 != 0 || year_cur % 400 == 0)
            count[0] += 366;
        else
            count[0] += 365;
    }
    /* 判断起始年是平年还是闰年 */
    if(start_time.year % 4 == 0 && start_time.year % 100 != 0 || start_time.year % 400 == 0)
        month_date[1] = 29;
    /* 计算起始年整月天数 */
    for(month_cur = 1; month_cur < start_time.month; month_cur++)
        count[1] += month_date[month_cur - 1];
    /* 加上起始年起始月过的天数 */
    if(month_cur == start_time.month)
        count[1] += start_time.date;
    /* 判断终止年是平年还是闰年 */
    if(end_time.year % 4 == 0 && end_time.year % 100 != 0 || end_time.year % 400 == 0)
        month_date[1] = 29;
    /* 计算终止年过了整月天数 */
    for(month_cur = 1; month_cur < end_time.month; month_cur++)
        count[2] += month_date[month_cur - 1];
    /* 加上终止年终止月天数 */
    if(month_cur == end_time.month)
        count[2] += end_time.date;

    return count[0] - count[1] + count[2];
}

// int main_20230109_time_cal()
int main()
{
    int ret = 0, difference = 0;

    scanf_input();

    ret = check_input();
    if(ret != 0)
    {
        printf("ERR CODE:0x%02x\n", ret);
        return 1;
    }

    exchange_year_month_date();

    difference = calcu_difference();

    printf("DATE DIFFERENCE IS %d DAYS.\n", difference);

    return 0;
}
