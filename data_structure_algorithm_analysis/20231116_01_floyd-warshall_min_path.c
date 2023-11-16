#include "../pengmc.h"


/*4个城市 8条路线*/
/*1  2  2
  1  3  6
  1  4  4
  2  3  3
  3  1  7
  3  4  1
  4  1  5
  4  3  12 */

#define MAX_NUMBER     (99999999)

int main(void)
{
    int i, j, k;
    int city_number = 4, road_number = 8;
    int matrix[city_number][city_number], book[city_number * city_number + 1];
    for (i = 1; i <= city_number; i++)
    {
        for (j = 1; j <= city_number; j++)
        {
            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = MAX_NUMBER;
        }
    }

    matrix[1][2] = 2,  matrix[1][3] = 6;
    matrix[1][4] = 4,  matrix[2][3] = 3;
    matrix[3][1] = 7,  matrix[3][4] = 1;
    matrix[4][1] = 5,  matrix[4][3] = 12;

    /*允许经过的顶点*/
    for (k = 1; k <= city_number; k++)
    {   /*出发的起始顶点*/
        for (i = 1; i <= city_number; i++)
        {   /*出发的终点顶点*/
            for (j = 1; j <= city_number; j++)
            {
                if (matrix[i][j] > matrix[i][k] + matrix[k][j])
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
            }
        }
    }

    for (i = 1; i <= city_number; i++)
    {
        for (j = 1; j <= city_number; j++)
            DBG_PRINTF("%-5d", matrix[i][j]);
        DBG_PRINTF("\n");
    }

    return 0;
}

