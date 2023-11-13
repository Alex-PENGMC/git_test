#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_MAX  (20)

struct
{
    char *filename;
    FILE *pfile;
} global = {"myfile.bin", NULL};

struct Date
{
    int day;
    int month;
    int year;
};

typedef struct family
{
    struct Date dob;
    char name[NAME_MAX];
    char pa_name[NAME_MAX];
    char ma_name[NAME_MAX];
}Family;

bool get_person(Family *pfamily);
void getname(char *name);
void show_person_data(void);
void get_parent_dob(Family *pfamily);

int main(void)
{
    Family member;

    if (!(global.pfile = fopen(global.filename, "wd")))
    {
        printf("\nUnable to open %s for writing.\n");
        exit(1);
    }

    while (get_person(&member))
        fwrite(&member, sizeof(member), 1, global.pfile);
    
    fclose(global.pfile);

    show_person_data();

    if (remove(global.filename))
        printf("\nUnable to delete %s.\n", global.filename);
    else
        printf("\nDeleted %s OK.\n", global.filename);

    return 0;
}

bool get_person(Family *temp)
{
    static char more = '\0';

    printf("\nDo you want to enter details of a%s person (Y or N)? ", more != '\0' ? "nother" : "");
    scanf(" %c", &more);

    if ('n' == tolower(more))
        return false;

    printf("\nEnter the name of the person: ");
    getname(temp->name);
    printf("\nEnter %s's date of birth (day month year); ", temp->name);
    scanf("%d%d%d", &temp->dob.day, &temp->dob.month, &temp->dob.year);

    printf("\nWho is %s's father? ", temp->name);
    getname(temp->pa_name);
    printf("\nWho is %s's mother? ", temp->name);
    getname(temp->ma_name);

    return true;

}

void getname(char *name)
{
    fflush(stdin);
    fgets(name, NAME_MAX, stdin);
    int len = strlen(name);
    if ('\n' == name[len - 1])
        name[len - 1] = '\0';
}

void show_person_data(void)
{
    Family member;
    fpos_t current;

    if (!(global.pfile = fopen(global.filename, "rb")))
    {
        printf("\nUnable to open %s for reading.\n", global.filename);
        exit(1);
    }

    while (fread(&member, sizeof(member), 1, global.pfile))
    {
        fgetpos(global.pfile, &current);
        printf("\n\n%s's father is %s, and mother is %s.", member.name, member.pa_name, member.ma_name);
        get_parent_dob(&member);
        fsetpos(global.pfile, &current);
    }

    fclose(global.pfile);
}

void get_parent_dob(Family *pmember)
{
    Family relative;
    int num_found = 0;

    rewind(global.pfile);

    while (fread(&relative, sizeof(Family), 1, global.pfile))
    {
        if (0 == strcmp(pmember->pa_name, relative.name))
        {
            printf("\n Pa wae born on %d/%d/%d.", relative.dob.day, relative.dob.month, relative.dob.year);

            if (2 == ++num_found)
                return;
        }
        else
        {
            if (0 == strcmp(pmember->ma_name, relative.name))
            {
                printf("\n Ma was born on %d/%d/%d.", relative.dob.day, relative.dob.month, relative.dob.year);

                if (2 == ++num_found)
                    return;
            }
        }
    }
}

