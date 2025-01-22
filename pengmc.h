#ifndef PENGMC_H
#define PENGMC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define OK             (0)
#define ERROR          (-1)

#define TRUE           (0)
#define FALSE          (-1)


#define _DEBUG_ 
#ifdef _DEBUG_
    #define DBG_PRINTF(fmt, ...) \
        printf("[DEBUG] [%s:%d] %s(): " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define DBG_PRINTF(...)
#endif


#define SWAP(A, B) do { int temp = A; A = B; B = temp; } while(0)
#define MAX2(A, B) ((A) > (B) ? (A) : (B))
#define MIN2(A, B) ((A) < (B) ? (A) : (B))
#define MAX3(A, B, C) (MAX2((A), MAX2((B), (C))))
#define MIN3(A, B, C) (MIN2((A), MIN2((B), (C))))
#define MID3(A, B, C) (MAX2(MIN2((A), (B)), MIN2(MAX2((A), (B)), (C))))
#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define ABS(A) ((A) < 0 ? -(A) : (A))


#define PRINT_ARRAY(array, length)\
{\
    for (int i = 0; i < (length); i++)\
        DBG_PRINTF("%d ", (array)[i]);\
    DBG_PRINTF("\n");\
}

#endif