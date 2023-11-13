#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK             (0)
#define ERROR          (-1)

#define _DEBUG_ 
#ifdef _DEBUG_
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif

#define FILE_NAME "alex.ini"

#define MAX_LENGTH     (265)

typedef enum type
{
    System,
    App
}type_t;

int main(void)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp)
    {
        DBG_PRINTF("File named %s doesn't exist.\n", FILE_NAME);
        return ERROR;
    }

    char string[MAX_LENGTH];
    char temp_line[MAX_LENGTH];
    type_t pattern;

    fgets(temp_line, 256, fp);

    while (fgets(temp_line, 256, fp))
    {
        DBG_PRINTF("%s", string);

        size_t length = strlen(temp_line);
        if (length > 0 && '\n' == temp_line[length - 1])
            strcpy(string, temp_line);
        else
        {
            if (!feof(fp))
            {
                int c;
                while ((c == fgetc(fp)) != '\n' && c != EOF);
            }
            strcpy(string, temp_line);
        }

        if ('#' == string[0] || 0 == strlen(string))
            continue;

        if (strstr(string, "[System]"))
        {
            pattern = System;
            continue;
        }

        if (strstr(string, "[App]"))
        {
            pattern = App;
            continue;
        }

        switch (pattern)
        {
            // case System:
            //     process_system_section(string);
            // break;
            // case App:
            //     process_app_section(string);
            // break;
            default:
            break;
        }


    }

    fclose(fp);

    return 0;
}

