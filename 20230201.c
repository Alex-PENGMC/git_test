#include <stdio.h>

typedef struct test
{
    char seq;
    char cmd[32];
}test_t;

int main()
{
	// int array[11] = {9,1,4,7,3,-1,0,5,8,-1,6};
	// int length = sizeof(array) / sizeof(array[0]); 
    // int i, j;
    // for (i = 0; i < length - 1; i++)
    // {
    //     for (j = i + 1; j < length; j++)
    //     {
    //         if (array[i] > array[j])
    //         {
    //             int tmp;
    //             tmp = array[i];
    //             array[i] = array[j];
    //             array[j] = tmp;
    //         }
    //     }
    // }

	// for (i = 0; i < length; i++)
	// 	printf("%d ", array[i]);

    test_t mm = {0};
    mm.seq++;
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("%c ", mm.seq++);
    }

    printf("\n   this is the end.\n");

	return 0;
}
