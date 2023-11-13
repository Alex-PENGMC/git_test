#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define OK             0
#define ERROR          -1

#define _DEBUG_ 
#ifdef _DEBUG_
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif
#define SWAP(A, B) {int temp; temp = A; A = B; B = temp;}

#define PRINT_ARRAY(array, length)\
{\
    for (int i = 0; i < (length); i++)\
        DBG_PRINTF("%d ", (array)[i]);\
    DBG_PRINTF("\n");\
}
