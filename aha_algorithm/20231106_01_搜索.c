#include <stdio.h>

#define OK             0
#define ERROR          -1

int a[10], book[10], n;

void dfs(int step)
{
    int i;
    if (step == n + 1)
    {
        for (i = 1; i <= n; i++)
            printf("%d ", a[i]);
        printf("\n");

        return;
    }

    for (i = 1; i <= n; i++)
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
    scanf("%d", &n);

    dfs(1);

    return 0;
}