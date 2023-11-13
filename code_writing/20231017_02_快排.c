#include <stdio.h>

int a[101], n;

void quicksort(int left, int right)
{
    int i, j, t, tmp;
    if (left > right)
        return;
    
    tmp = a[left];
    i   = left;
    j   = right;

    while (i != j)
    {
        while (a[j] >= tmp && i < j)
            j--;

        while (a[i] <= tmp && i < j)
            i++;

        if (i < j)
        {
            t    = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }

    a[left] = a[i];
    a[i]    = tmp;

    quicksort(left, i - 1);
    quicksort(i + 1, right);
}

int main(void)
{
    int i, j, t;

    scanf("%d", &n);

    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    quicksort(0, n - 1);

    for (i = 0; i < n; i++)
        printf("%d ", a[i]);

    return 0;
}