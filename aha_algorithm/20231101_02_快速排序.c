#include <stdio.h>

#define OK             1
#define ERROR          0

int a[101], n;

void quicksort(int left, int right);

int main(void)
{//test data:10  6 1 2 7 9 3 4 5 10 8

    int i, j, t;

    scanf("%d", &n);

    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    quicksort(0, n - 1);

    for (i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    return 0;
}

void quicksort(int left, int right)
{
    int i, j, t, temp;
    
    if (left > right)
        return;
    
    temp = a[left];
    i    = left;
    j    = right;
    while (i != j)
    {
        while (a[j] >= temp && i < j)
            j--;
        
        while (a[i] <= temp && i < j)
            i++;
        /*交换两侧的数据*/
        if (i < j)
        {
            t    = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
    /*基准数归位*/
    a[left] = a[i] ;
    a[i] = temp;

    quicksort(left, i - 1);
    quicksort(i + 1, right);
}

