#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*******************************************
*  编译使用-lm指定链接到math库，否则报错      *
*  gcc 20231010_04_判断质数.c -o aaa -lm    *
*                                          *
********************************************/

// const unsigned int MEM_PRIMES = 100;
typedef unsigned long long ull;
#define  MEM_PRIMES 100
int test_prime(ull N);
void put_prime(void);
int check(ull buffer[], size_t count, ull N);

struct
{
    char *filename;         /*文件名*/
    FILE *pfile;            /*文件指针*/
    int nrec;               /*质数有多少个100*/
    ull primes[MEM_PRIMES]; /*质数数组*/
    size_t index;           /*数组内质数下标*/
} global = {
    "myfile.bin",
    NULL,
    0,
    {2ULL, 3ULL, 5ULL},
    3
};

int main(void)
{
    ull trial = 5ULL;
    unsigned long num_primes = 3UL;
    unsigned long total      = 0UL;

    printf("How many primes would you like? ");
    scanf("%lu", &total);
    total = total < 4UL ? 4UL : total;

    while (num_primes < total)
    {
        trial += 2ULL;
        if (test_prime(trial))
        {
            global.primes[global.index++] = trial;
            num_primes++;

            if (MEM_PRIMES == global.index)
            {
                if (!(global.pfile = fopen(global.filename, "ab")))
                {
                    printf("\nUnable to open %s to append\n", global.filename);
                    exit(1);
                }
                fwrite(global.primes, sizeof(ull), MEM_PRIMES, global.pfile);
                fclose(global.pfile);
                global.index = 0U;
                global.nrec++;
            }
        }
    }

    if (total > MEM_PRIMES)
        put_prime();
    if (global.index)
    {
        for (size_t i = 0; i < global.index; i++)
        {
            if (0 == i % 5)
                printf("\n");
            printf("%12llu", global.primes[i]);
        }
    }
    printf("\n");

    if (total > MEM_PRIMES)
    {
        if (remove(global.filename))
            printf("\nFailed to delete %s \n", global.filename);
        else
            printf("\nFile %s deleted.\n", global.filename);
    }

    return 0;
}

int test_prime(ull N)
{
    ull buffer[MEM_PRIMES];

    int k = 0;

    if (global.nrec > 0)
    {
        if (!(global.pfile = fopen(global.filename, "rb")))
        {
            printf("\nUnable to open %s to read\n", global.filename);
            exit(1);
        }

        for (size_t i = 0; i < global.nrec; i++)
        {
            fread(buffer, sizeof(long long), MEM_PRIMES, global.pfile);
            if ((k = check(buffer, MEM_PRIMES, N)) >= 0)
            {
                fclose(global.pfile);
                return k;
            }
        }
        fclose(global.pfile);
    }

    return check(global.primes, global.index, N);
}

/*判断质数*/
int check(ull buffer[], size_t count, ull N)
{
    ull root_N = (ull)(1.0 + sqrt(N));

    for (size_t i = 0; i < count; i++)
    {
        /*表示刚好整除，不是质数*/
        if (0ULL == N % buffer[i])
            return 0;
        /*表示不存在，是质数*/
        if (buffer[i] > root_N)
            return 1;
    }

    return -1;
}

/*遍历整个二维数组并输出*/
void put_prime(void)
{
    ull buffer[MEM_PRIMES];
    if (!(global.pfile = fopen(global.filename, "rb")))
    {
        printf("\nUnable to open %s to read primes for output.\n", global.filename);
        exit(1);
    }

    for (size_t i = 0U; i < global.nrec; i++)
    {
        fread(buffer, sizeof(ull), MEM_PRIMES, global.pfile);
        for (size_t j = 0; j < MEM_PRIMES; j++)
        {
            if (0U == j % 5)
                printf("\n");
            printf("%12llu", buffer[j]);
        }
    }
    fclose(global.pfile);
}

