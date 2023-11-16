#include "../pengmc.h"

int main(void)
{
    int dis[10], i, k, n = 5, m = 5, u[10], v[10], w[10];
    int inf = 99999999;
    

    u[1] = 2, v[1] = 3, w[1] = 2;
    u[2] = 1, v[2] = 2, w[2] = -3;
    u[3] = 1, v[3] = 5, w[3] = 5;
    u[4] = 4, v[4] = 5, w[4] = 2;
    u[5] = 3, v[5] = 4, w[5] = 3;

    for (i = 1; i <= n; i++)
        dis[i] = inf;
    
    dis[1] = 0;

    int bak[10], check, flag;

    for (k = 1; k <= n - 1; k++)
    {
        /*进行备份*/
        for (i = 1; i <= n; i++)
            bak[i] = dis[i];
        /*进行松弛*/
        for (i = 1; i <= m; i++)
        {
            if (dis[v[i]] > dis[u[i]] + w[i])
                dis[v[i]] = dis[u[i]] + w[i];
        }
        /*松弛完毕检测dis数组是否有更新,存在更新就继续，否则跳出大循环*/
        check = 0;
        for (i = 0; i <= n; i++)
        {
            if (bak[i] != dis[i])
            {
                check = 1;
                break;
            }
        }

        if (0 == check)
            break;
    }

    flag = 0;
    for (i = 1; i <= m; i++)
    {
        if (dis[v[i]] > dis[u[i]] + w[i])
            flag = 1;
    }

    if (flag == 1)
        DBG_PRINTF("此图存在负权回路");
    else
    {
        for (i = 1; i <= n; i++)
                DBG_PRINTF("%d ", dis[i]);
                
        LINE_FEED;
    }

    return 0;
}

