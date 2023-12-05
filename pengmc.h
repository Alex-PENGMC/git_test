#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define OK             ( 0)
#define ERROR          (-1)

#define _DEBUG_ 
#ifdef _DEBUG_
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif

/*int型两个数据交换值*/
#define SWAP(A, B) {int temp; temp = A; A = B; B = temp;}

/*计算一维/二维 int/char数组的长度*/
#define ARRAY_LENGTH_1D(array)  (sizeof(array) / sizeof(array[0]))
#define ARRAY_LENGTH_2D(array)  (sizeof(array) / sizeof(array[0][0]))

/*一维数组的遍历打印*/
#define ARRAY_PRINT(array, length)    \
{                                     \
    for (int i = 0; i < (length); i++)\
        DBG_PRINTF("%d ", (array)[i]);\
    DBG_PRINTF("\n");                 \
}

/*一维int型数组初始化为0*/
#define ARRAY2ZERO(array, length)     \
{                                     \
    for (int i = 0; i < (length); i++)\
        (array)[i] = 0;               \
}

typedef unsigned int   uint;
typedef unsigned char  uchar;

/*打印问题发生的文件名、函数名和行号*/
#define MALLOC_ERROR_FILE_FUNC_LINE   DBG_PRINTF("malloc error file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__)

/*换行*/
#define LINE_FEED   printf("\n")


