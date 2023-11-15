#include "../pengmc.h"

int main(void)
{
    int i, j, cur, book[101] = {0}, e[101][101];
    int que[10001], head, tail;

    int n = 5, m = 5;
    /*邻接矩阵的主对角线为0，其余所有值初始化为9999，表示无*/
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
    /*将邻接矩阵的存在边的数值改为1，表明两节点之间存在边*/
    e[1][2] = e[2][1] = 1;
    e[1][3] = e[3][1] = 1;
    e[1][5] = e[5][1] = 1;
    e[2][4] = e[4][2] = 1;
    e[3][5] = e[5][3] = 1;

    head = tail = 1;

    que[tail] = 1;
    tail++;
    book[1] = 1;

    while (head < tail)
    {
        cur = que[head];

        for (i = 1; i <= n; i++)
        {
            if (1 == e[cur][i] && 0 == book[i])
            {
                que[tail] = i;
                tail++;
                book[i] = 1;
            }

            if (tail > n)
                break;
        }
        head++;
    }
    
    for (i = 1; i < tail; i++)
        DBG_PRINTF("%d ", que[i]);

    DBG_PRINTF("\n");

    return 0;
}