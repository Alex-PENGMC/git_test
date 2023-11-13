#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LENGTH = 80;

int main(void)
{
    char mystr[LENGTH];
    int lstr = 0, mychar = 0;
    FILE *pfile = NULL;
    char *filename = "myfile.txt";

    printf("\nEnter an interesting string of less than 80 character:\n");
    fgets(mystr, LENGTH, stdin);

    if (!(pfile = fopen(filename, "w")))
    {
        printf("Error opening %s for writing. Program terminated.", filename);
        exit(1);
    }

    lstr = strlen(mystr);
    for (int i = lstr - 1; i >= 0; i--)
        fputc(mystr[i], pfile);

    fclose(pfile);

    if (!(pfile = fopen(filename, "r")))
    {        
        printf("Error opening %s for reading. Program terminated.", filename);
        exit(1);
    }

    while (EOF != (mychar = fgetc(pfile)))
        putchar(mychar);

    putchar('\n');

    fclose(pfile);
    /*删除文件*/
    remove(filename);

    return 0;
}