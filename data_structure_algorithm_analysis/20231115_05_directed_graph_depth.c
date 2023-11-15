#include "../pengmc.h"

int book[101], sum, n = 5, e[101][101];

void dfs(int cur)
{
    int i;
    DBG_PRINTF("%d ", cur);
    sum++;
    if (n == sum)
        return;
    
    for (i = 1; i <= n; i++)
    {
        if (e[cur][i] == 1 && book[i] == 0)
        {
            book[i] = 1;
            dfs(i);
        }
    }

    return;
}

int main(void)
{
    int i, j, m = 5;

    // scanf("%d%d", &n, &m);

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
    e[1][5] = e[5][1] = 1;
    e[2][4] = e[4][2] = 1;
    e[3][5] = e[5][3] = 1;

    book[1] = 1;
    dfs(1);

    DBG_PRINTF("\n");

    return 0;
}