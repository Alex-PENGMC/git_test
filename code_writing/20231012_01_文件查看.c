#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN        (256)
#define DISPLAY       (80)
#define PAGE_LENTGTH  (20)


int main(int argc, char *argv[])
{
    char filename[MAXLEN];
    unsigned char buffer[DISPLAY / 4 - 1];
    int count = 0, lines = 0;

    FILE *pfile = NULL;

    if (1 == argc)
    {
        printf("Please enter a filename: ");
        fgets(filename, MAXLEN, stdin);

        int len = strlen(filename);
        if ('\n' == filename[len - 1])
            filename[len - 1] = '\0';
    }
    else
        strcpy(filename, argv[1]);

    if (!(pfile = fopen(filename, "rb")))
    {
        printf("Sorry, can't open %s", filename);
        return -1;
    }

    while (!feof(pfile))
    {
        if (count < sizeof(buffer))
            buffer[count++] = (unsigned char)fgetc(pfile);
        else
        {
            for (count = 0; count < sizeof(buffer); count++)
                printf("%02X ", buffer[count]);
            printf("| ");

            for (count = 0; count < sizeof(buffer); count++)
                printf("%c", isprint(buffer[count]) ? buffer[count] : '.');
            printf("\n");
            count = 0;

            if (!(++lines % PAGE_LENTGTH))
            {
                if ('E' == getchar())
                    return 0;
            }
        }
    }
    /*用于打印最后一个buffer内没有满的字符*/
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (i < count)
            printf("%02X ", buffer[i]);
        else
            printf(" ");
    }
    printf("| ");

    for (int i = 0; i < count; i++)
        printf("%c", isprint(buffer[i]) ? buffer[i] : '.');
            
    printf("\n");

    fclose(pfile);

    return 0;
}
