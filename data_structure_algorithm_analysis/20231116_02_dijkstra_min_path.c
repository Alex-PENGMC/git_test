#include "../pengmc.h"


/*5个城市 9条路线*/
/*
  1  2  1
  1  3  12
  2  3  9
  2  4  3
  3  5  5
  4  3  4
  4  5  13
  4  6  15 
  5  6  4 
*/

#define MAX_NUMBER     (99999999)

int main(void)
{
    int i, j, k;
    int city_number = 6, road_number = 9;
    int matrix[city_number][city_number];
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

    matrix[1][2] = 1,  matrix[1][3] = 12;
    matrix[2][3] = 9,  matrix[2][4] = 3;
    matrix[3][5] = 5,  matrix[4][3] = 4;
    matrix[4][5] = 13, matrix[4][6] = 15;
    matrix[5][6] = 4;

    int dis[city_number], book[city_number], min, u, v;

    for (i = 1; i <= city_number; i++)
        dis[i] = matrix[1][i];

    for (i = 1; i <= city_number; i++)
        book[i] = 0;

    book[1] = 1;

    for (i = 1; i <= city_number - 1; i++)
    {
        min = MAX_NUMBER;
        for (j = 1; j <= city_number; j++)
        {
            if (book[j] == 0 && dis[j] < min)
            {
                min = dis[j];
                u = j;
            }
        }

        book[u] = 1;
        for (v = 1; v <= city_number; v++)
        {
            if (matrix[u][v] < MAX_NUMBER)
            {
                if (dis[v] > dis[u] + matrix[u][v])
                    dis[v] = dis[u] + matrix[u][v];
            }
        }
    }

    for (j = 1; j <= city_number; j++)
        DBG_PRINTF("%10d", dis[j]);
    DBG_PRINTF("\n");

    return 0;
}

