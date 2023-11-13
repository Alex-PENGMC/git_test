#include <stdio.h>

#define OK             0
#define ERROR          -1

int a[10], book[10], total;

void dfs(int step)
{
    int i;
    if (10 == step)
    {
        if (100 * a[7] + 10 * a[8] + a[9] == 100 * a[1] + 10 * a[2] + a[3] + 100 * a[4] + 10 * a[5] + a[6])
        {
            total++;
            printf("%d%d%d + %d%d%d = %d%d%d\n", a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
        }
            
        return;
    }

    for (i = 1; i <= 9; i++)
    {
        if (0 == book[i])
        {
            a[step] = i;
            book[i] = 1;

            dfs(step + 1);
            book[i] = 0;
        }
    }

    return;
}

int main(void)
{
    dfs(1);

    printf("total = %d\n", total / 2);

    return 0;
}