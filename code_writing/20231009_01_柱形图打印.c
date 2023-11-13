#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PAGE_HEIGHT   20
#define PAGE_WIDTH    40

typedef struct barTAG
{
    double value;
    struct barTAG *pnextbar;
}bar;

typedef unsigned int uint;

int bar_chart(bar *pfirstbar, uint page_width, uint page_height, char *title);

int main(void)
{
    /*柱形图第一个结构体*/
    bar firstbar;
    /*柱形图尾指针*/
    bar *plastbar = NULL;
    /*用于存放图表标题和每个bar的长度*/
    char value[80], title[80];

    /*获取图标标题并存入字符串数组中*/
    printf("\nEnter the chart title: ");
    fgets(title, sizeof(title), stdin);

    while (1)
    {
        /*获取用户输入的每个bar的数值或者退出这个循环*/
        printf("Enter the value of the bar, or use quit to end: ");
        fgets(value, sizeof(value), stdin);
        /*将收到的数据与quit相比较，相等表明用户输入结束，需要退出，这里注意多一个‘\n’*/
        if (0 == strcmp(value, "quit\n"))
            break;
        /*首节点*/
        if (!plastbar)
        {
            /*首节点的下一节点为空，首节点同时也是尾节点*/
            firstbar.pnextbar = NULL;
            plastbar = &firstbar;
        }
        else
        {
            /*为尾节点申请空间及失败应对措施*/
            if (!(plastbar->pnextbar = malloc(sizeof(bar))))
            {
                printf("Oops! Couldn't allocate memory\n");
                return -1;
            }
            /*尾节点指向下一个，并将下一个节点的下一节点指向空*/
            plastbar = plastbar->pnextbar;
            plastbar->pnextbar = NULL;
        }
        /*将数据放入链表中*/
        plastbar->value = atof(value);
    }

    bar_chart(&firstbar, PAGE_WIDTH, PAGE_HEIGHT, title);

    /*释放整个链表*/
    bar* phead = &firstbar;
    bar* ptail = &firstbar;
    while (ptail)
    {
        phead = ptail;
        ptail = ptail->pnextbar;
        free(phead);
    }
    /*释放最后一个节点并将两个指针置空*/
    if (!ptail)
    {
        free(ptail);
        phead = NULL;
        ptail = NULL;
    }

    return 0;
}

int bar_chart(bar *pfirstbar, uint page_width, uint page_height, char *title)
{
    /*最好最好不要对传入的指针进行修改，因此一般是重新定义一个同类型的指针，接收传入指针的地址*/

    bar *plastbar = pfirstbar;
    /*定义最大、最小、垂直缩放、位置*/
    double max = 0.0, min = 0.0, vert_scale = 0.0, position = 0.0;
    /*定义bar的数量，bar的宽度，空格，循环计算bars*/
    uint bar_count = 1, barwidth = 0, space = 2, i = 0, bars = 0;

    bool axis = false;

    char *column = NULL, *blank = NULL;
    /*首先将第一个bar的长度作为max和min， 赋初值作用*/
    max = min = plastbar->value;
    /*遍历整个链表，计算bar的数量和max及min实际值*/
    while (NULL != (plastbar = plastbar->pnextbar))
    {
        bar_count++;
        max = (max < plastbar->value) ? plastbar->value : max;
        min = (min > plastbar->value) ? plastbar->value : min;
    }
    /*计算缩放比例*/
    vert_scale = (max - min) / page_height;
    /*每两个柱形图之间间隔为2， 整个画布的大小除以柱形图的数量再减去间格，如果太小就不画了*/
    if ((barwidth = page_width / bar_count - space) < 1)
    {
        printf("\nPage width too narrow.\n");
        return -1;
    }
    /*申请列内存*/
    if (NULL == (column = malloc(barwidth + space + 1)))
    {
        printf("\nFailed to allocate memory in barchart()" " - terminating program.\n");
        exit(1);
    }

    for (i = 1; i < space; i++)
        *(column + i) = ' ';

    for (; i < space + barwidth; i++)
        *(column + i) = '#';

    *(column + i) = '\0';

    if (NULL == (blank = malloc(barwidth + space + 1)))
    {
        printf("\nFailed to allocate memory in barchart()" " - terminating program.\n");
        exit(1);
    }

    for (i = 0; i < space + barwidth; i++)
        *(blank + i) = ' ';
    
    *(blank + i) = '\0';

    printf("^ %s\n", title);
    position = max;

    for (i = 0; i < page_height; i++)
    {
        if (position <= 0.0 && !axis)
        {
            printf("+");
            for (bars = 0; bars < bar_count * (barwidth + space); bars++)
                printf("-");
            printf(">\n");

            axis = true;
            position -= vert_scale;
            continue;
        }
        printf("|");
        plastbar = pfirstbar;

        for (bars = 1; bars <= bar_count; bars++)
        {
            printf("%s", position <= plastbar->value &&
                        plastbar->value >= 0.0 && position > 0.0 || 
                        position >= plastbar->value && 
                        plastbar->value <= 0.0 && position <= 0.0 ? column : blank);
            plastbar = plastbar->pnextbar;
        }
        printf("\n");
        position -= vert_scale;
    }

    if (!axis)
    {
        printf("+");
        for (bars = 0; bars < bar_count * (barwidth + space); bars++)
            printf("-");
        printf(">\n");
    }

    free(blank);
    free(column);

    return 0;
}
