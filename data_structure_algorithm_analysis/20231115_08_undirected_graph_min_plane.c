#include "../pengmc.h"

struct node
{
    int x; /*城市编号*/
    int s; /*转机次数*/
} ;

int main(void)
{
    struct node que[2501]    ;
    int e[51][51] = {0}, book[51] = {0};
    int head, tail;
    int i, j, n = 5, m = 7, a, b, cur, start, end, flag = 0;

    scanf("%d%d", &start, &end);

    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (i == j)
                e[i][j] = 0;
            else
                e[i][j] = 99999999;
        }
    }

    e[1][2] = e[2][1] = 1;
    e[1][3] = e[3][1] = 1;
    e[2][3] = e[3][2] = 1;
    e[2][4] = e[4][2] = 1;
    e[3][4] = e[4][3] = 1;
    e[3][5] = e[5][3] = 1;
    e[4][5] = e[5][4] = 1;

    head = tail = 1;
    que[tail].x = start;
    que[tail].s = 0;
    tail++;
    book[1]     = start;

    while (head < tail)
    {
        cur = que[head].x;
        for (j = 1; j <= n; j++)
        {
            if (e[cur][j] != 99999999 && book[j] == 0)
            {
                que[tail].x = j;
                que[tail].s = que[head].s + 1;
                tail++;
                book[j] = 1;
            }

            if (que[tail].x == end)
            {
                flag = 1;
                break;
            }
        }

        if (flag == 1)
            break;
        
        head++;
    }
    
    DBG_PRINTF("%d\n", que[tail - 1].s);

    return 0;
}