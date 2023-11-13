#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN    (30)

void listfile(char *filename);

int main(void)
{
    char *filename = "mydata.bin";
    char name[MAXLEN];
    size_t length = 0;
    int age = 0;
    char answer = 'y';

    FILE *pFile = fopen(filename, "wb+");

    do
    {
        fflush(stdin);

        printf("\nEnter a name less than %d characters:", MAXLEN);
        scanf("%s", name);

        printf("Enter the age of %s: ", name);
        scanf("%d", &age);

        length = strlen(name);
        fwrite(&length, sizeof(length), 1, pFile);
        fwrite(name, sizeof(char), length, pFile);
        fwrite(&age, sizeof(age), 1, pFile);

        printf("Do you want to enter another(y or n)? ");
        scanf("\n%c", &answer);
    } while ('y' == tolower(answer));

    fclose(pFile);

    listfile(filename);

    return 0;    
}

void listfile(char *filename)
{
    size_t length = 0;
    int    age    = 0;
    char name[MAXLEN], format[20];
    /*执行完之后，format内容为："\n%-30s Age:%4d"*/
    sprintf(format, "\n%%-%ds Age:%%4d", MAXLEN);

    FILE *pFile = fopen(filename, "rb");
    printf("\nThe contents of %s are:", filename);
    /*读一个结果，这里是长度这个具体的数值，所以赋值给length的地址，若是读一串文件或者数据，这里就需要是数组地址或文件地址*/
    while (1 == fread(&length, sizeof(length), 1, pFile))
    {
        if (length + 1 > MAXLEN)
        {
            printf("\nName too long.");
            exit(1);
        }
        
        fread(name, sizeof(char), length, pFile);
        name[length] = '\0';
        fread(&age, sizeof(age), 1, pFile);
        printf(format, name, age);
    }

    fclose(pFile);

    printf("\n");
}
