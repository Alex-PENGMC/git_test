#include "pengmc.h"

#define NUMBER    10

int main(void)
{
    srand(time(NULL));
    
    int i, array[NUMBER];

    for (i = 0; i < NUMBER; i++)
        array[i] = rand() % 100;

    PRINT_ARRAY(array, NUMBER);

    int j, temp;

    for (i = 1; i < NUMBER; i++)
    {
        temp = array[i];
        j = i - 1;

        while (j >= 0 && temp < array[j])
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;
    }

    PRINT_ARRAY(array, NUMBER);

    return 0;
}