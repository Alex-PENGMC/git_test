#include <stdio.h>

#define OK             0
#define ERROR          -1

struct node
{
    int x;
    int y;
    int f;
    int s;
};

/*test data*/
// 5 4 
// 0 0 1 0
// 0 0 0 0 
// 0 0 1 0
// 0 1 0 0
// 0 0 0 1
// 1 1 4 3

int main(void)
{
    struct node que[2501];
    int head, tail;
    int a[51][51] = {0}, book[51][51] = {0};
    int next[4][2] = 
    {/*按照顺时针方向遍历所有的节点*/
        {0, 1},  /*right*/
        {1, 0},  /*down*/
        {0, -1}, /*left*/
        {-1, 0}  /*up*/
    };

    int i, j, k, n, m, startx, starty, p, q, tx, ty, flag;
    /*迷宫的行和列*/
    scanf("%d%d", &n, &m);
    /*遍历迷宫，0表示空地，1表示障碍物*/
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= m; j++)
            scanf("%d", &a[i][j]);
    }
    /*表示起始地址和目标地址*/
    scanf("%d%d%d%d", &startx, &starty, &p, &q);

    head = 1;
    tail = 1;
    que[tail].x = startx;
    que[tail].y = starty;
    que[tail].f = 0;
    que[tail].s = 0;
    tail++;
    book[startx][starty] = 1;
    flag = 0;

    while (head < tail)
    {
        for (k = 0; k <= 3; k++)
        {
            tx = que[head].x + next[k][0];
            ty = que[head].y + next[k][1];

            /*防止越界*/
            if (tx < 1 || tx > n || ty < 1 || ty > m)
                continue;
            /*0 == a[tx][ty]：表示是空地，可走
              0 == book[tx][ty]：不在路径中*/
            if (0 == a[tx][ty] && 0 == book[tx][ty])
            {
                /*标记为在路径中*/
                book[tx][ty] = 1; 
                /*新节点入队*/
                que[tail].x = tx;
                que[tail].y = ty;
                que[tail].f = head;
                que[tail].s = que[head].s + 1;
                tail++;
            }

            if (tx == p && ty == q)
            {
                flag = 1;
                break;
            }
        }

        if (1 == flag)
            break;

        head++;
    }
    
    printf("%d ", que[tail - 1].s);

    return 0;
}