#include <stdio.h>

#define OK             0
#define ERROR          -1

struct node
{
    int x;
    int y;
};

char a[20][20];

int getnum(int i, int j)
{
    int sum, x, y;
    sum = 0;
    /*向上统计*/
    x = i;
    y = j;
    while ('#' != a[x][y])
    {
        if ('G' == a[x][y])
            sum++;
        x--;
    }
    /*向下统计*/
    x = i;
    y = j;
    while ('#' != a[x][y])
    {
        if ('G' == a[x][y])
            sum++;
        x++;
    }
    /*向左统计*/
    x = i;
    y = j;
    while ('#' != a[x][y])
    {
        if ('G' == a[x][y])
            sum++;
        y--;
    }
    /*向右统计*/
    x = i;
    y = j;
    while ('#' != a[x][y])
    {
        if ('G' == a[x][y])
            sum++;
        y++;
    }

    return sum;
}

int main(void)
{
    struct node que[401];
    int head = 1, tail = 1;
    int  book[20][20] = {0};

    int i, j, k, sum, max, mx, my, n, m, startx, starty, tx, ty;

    int next[4][2] = 
    {/*按照顺时针方向遍历所有的节点*/
        {0, 1},  /*right*/
        {1, 0},  /*down*/
        {0, -1}, /*left*/
        {-1, 0}  /*up*/
    };

    // scanf("%d%d%d%d", &n, &m, &startx, &starty);
    m = n = 13;
    startx = starty = 3;

    for (i = 0; i <= n - 1; i++)
        scanf("%s", a[i]);

    que[tail].x = startx;
    que[tail].y = starty;
    tail++;
    book[startx][starty] = 1;

    max = getnum(startx, starty);
    mx  = startx;
    my  = starty;

    int next[4][2] = 
    {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    while (head < tail)
    {
        for (k = 0; k <= 3; k++)
        {
            tx = que[head].x + next[k][0];
            ty = que[head].y + next[k][1];

            if (tx < 0 || tx > n - 1 || ty < 0 || ty > m - 1)
                continue;

            if ('.' == a[tx][ty] && 0 == book[tx][ty])
            {
                book[tx][ty] = 1;
                que[tail].x = tx;
                que[tail].y = ty;
                tail++;

                sum = getnum(tx, ty);

                if (sum > max)
                {
                    max = sum;
                    mx = tx;
                    my = ty;
                }
            }
        }

        head++;
    }
 

    return 0;
}