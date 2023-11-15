#include "../pengmc.h"

#define MAX_NUMBER    (99999999)

int min_path = MAX_NUMBER, city_number = 5;
int book[101], e[101][101];

void dfs(int cur, int dis)
{
    int j;

    if (dis > min_path)
        return;
    if (cur == city_number)
    {
        if (dis < min_path)
            min_path = dis;
        return;
    }

    for (j = 1; j <= city_number; j++)
    {
        if (e[cur][j] != MAX_NUMBER && book[j] == 0)
        {
            book[j] = 1;
            dfs(j, dis + e[cur][j]);
            book[j] = 0;
        }
    }

    return;
}

int main(void)
{
    int road_number = 8;
    int i, j;
    for (i = 1; i <= city_number; i++)
    {
        for (j = 1; j <= road_number; j++)
        {
            if (i == j)
                e[i][j] = 0;
            else
                e[i][j] = MAX_NUMBER;
        }
    }

    e[1][2] = 2;
    e[1][5] = 10;
    e[2][3] = 3;
    e[2][5] = 7;
    e[3][1] = 4;
    e[3][4] = 4;
    e[4][5] = 5;
    e[5][3] = 3;

    book[1] = 1;
    dfs(1, 0);

    DBG_PRINTF("min_path path: %d\n", min_path);

    return 0;
}