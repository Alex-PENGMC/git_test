#include <stdio.h>
#include <time.h>

// int main(void)
// {
//     time_t calendar = 0;
//     struct tm *time_data;
//     const char *days[]  = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//     const char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August",
//                             "September", "October", "November", "December"};

//     calendar = time(NULL);
//     printf("\n%s", ctime(&calendar));

//     time_data = localtime(&calendar);
//     printf("Today is %s %s %d %d\n", days[time_data->tm_wday], month[time_data->tm_mon],
//                                      time_data->tm_mday, time_data->tm_year + 1900);

//     return 0;
// }

int main(void)
{
    const char *days[7]   = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    const char *month[12] = {"January", "February", "March", "April", "May", "June", "July", "August",
                            "September", "October", "November", "December"};

    const char *Suffix[4] = {"st", "nd", "rd", "th"};
    
    enum sufindex
    {
        st,
        nd,
        rd,
        th
    } sufsel = th;

    struct tm *OurT = NULL;
    time_t Tval = 0;

    /*获取当前时间存入Tval中*/
    Tval = time(NULL);
    /*利用获取到的值生成结构体成员值*/
    OurT = localtime(&Tval);

    switch (OurT->tm_mday)
    {
        case 1: case 21: case 31:
            sufsel = st;
            break;
        case 2: case 22:
            sufsel = nd;
            break;
        case 3: case 23:
            sufsel = rd;
            break;
        default:
            sufsel = th;
            break;
    }

    printf("Today is %s the %d%s %s %d", days[OurT->tm_wday], OurT->tm_mday, 
            Suffix[sufsel], month[OurT->tm_mon], 1900 + OurT->tm_year);

    printf("\nThe time is %d : %d : %d\n", OurT->tm_hour, OurT->tm_min, OurT->tm_sec);

    return 0;
}

