//有 1、2、3、4、5 五个数字，能组成多少个互不相同且无重复数字的三位数？都是多少？

#include <stdio.h>
#include <string.h> 
int main_mine(void)
{
    int  count = 0;
    int  i, j, k;
    char array[] = {'1', '2', '3', '4', '5'};
    char result[1000][3];

    memset(result, 0, sizeof(result));
    /*从数组中取三个数字出�?*/
    for(i = 0; i < sizeof(array) - 2; i++)
    {
        for(j = i + 1; j < sizeof(array) - 1; j++)
        {
            for(k = j + 1; k < sizeof(array); k++)
            {
                count++;
                result[count][0] = array[i];
                result[count][1] = array[j];
                result[count][2] = array[k];
            }
        }
    }

    /*对这三个数字进行排序*/
    for(i = 0; i < count; i++)
    {
        printf("%c %c %c\n", result[i][0], result[i][1], result[i][2]);
        printf("%c %c %c\n", result[i][0], result[i][2], result[i][1]);
        printf("%c %c %c\n", result[i][1], result[i][0], result[i][2]);
        printf("%c %c %c\n", result[i][1], result[i][2], result[i][0]);
        printf("%c %c %c\n", result[i][2], result[i][0], result[i][1]);
        printf("%c %c %c\n", result[i][2], result[i][1], result[i][0]);
    }

    printf("count = %d\n", 6 * count);

    return 0;
}

int main(void)
{
	int i, j, k, count = 0;
	for(i = 1; i < 5; i++)
	{
		for(j = 1; j < 5; j ++)
		{
			for(k = 1; k < 5; k++)
			{
				if(i != j && j != k && i != k)
				{
					count++;
					printf("%d%d%d\n", i, j, k);	
				}		
			}
		}
	}
	
	printf("count:%d\n", count);	
	
	return 0;
}

