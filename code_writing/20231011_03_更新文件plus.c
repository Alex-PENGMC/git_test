#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN    (30)
char *dirpath = "";
char *file    = "mydata.bin";

struct Record
{
    char name[MAXLEN];
    int age;
};

void listfile(char *filename);
void updatefile(char *filename);
struct Record *getrecord(struct Record *precord);

void getname(char *pname);
void writefile(char *filename, char *mode);
void wirterecord(struct Record *precord, FILE *pFile);

struct Record *readrecord(struct Record *precord, FILE *pFile);
int findrecord(struct Record *precord, FILE *pFile);
void duplicatefile(struct Record *pnewrecord, int index, char *filename, FILE *pFile);

int main(void)
{
    char filename[strlen(dirpath) + strlen(file) + 1];
    strcpy(filename, dirpath);
    strcat(filename, file);

    char answer = 'q';

    while (true)
    {
        printf("\nChoose from the following options:"
               "\nTo list the file contents enter L"
               "\nTo create a new file enter C"
               "\nTo add new records enter A"
               "\nTo update existing records enter U"
               "\nTo delete the file enter D"
               "\nTo quit the program enter Q\n :");
        scanf(" %c", &answer);
        
        switch(tolower(answer))
        {
            case 'l':
                listfile(filename);
                break;
            case 'c':
                writefile(filename, "wb+");
                printf("\nFile creation complete.");
                break;
            case 'a':
                writefile(filename, "ab+");
                printf("\nFile append complete.");
                break;
            case 'u':
                updatefile(filename);
                break;
            case 'd':
                printf("Are you sure you want to delete %s (y or n)?", filename);
                scanf("\n%c", &answer);
                if ('y' == tolower(answer))
                    remove(filename);
                break;
            case 'q':
                printf("Quit the %s program.", filename);
                return 0;
            default:
                printf("Invalid selection. Try again.");
                break;
        }
    }
    return 0;
}

struct Record *getrecord(struct Record *precord)
{
    if (!precord)
    {
        printf("No Record object to store input.");
        return NULL;
    }

    printf("\nEnter a name less than %d characters:", MAXLEN);
    getname(precord->name);

    printf("\nEnter the age of %s: ", precord->name);
    scanf(" %d", &precord->age);

    return precord;
}

void getname(char *pname)
{
    // fflush(stdin);
    fgets(pname, MAXLEN, stdin);
    int len = strlen(pname);
    if ('\n' == pname[len - 1])
        pname[len - 1] = '\0';
}

void wirterecord(struct Record *precord, FILE *pFile)
{
    if (!precord)
    {
        printf("No Record object to write to the file.");
        return;
    }
    if (!pFile)
    {
        printf("No stream pointer for the output file.");
        return;
    }

    size_t length = strlen(precord->name);
    fwrite(&length, sizeof(length), 1, pFile);
    fwrite(precord->name, sizeof(char), length, pFile);
    fwrite(&precord->age, sizeof(precord->age), 1, pFile);
}

struct Record *readrecord(struct Record *precord, FILE *pFile)
{
    if (!precord)
    {
        printf("No Record object to store data from the file.");
        return NULL;
    }
    if (!pFile)
    {
        printf("No stream pointer for the input file.");
        return NULL;
    }

    size_t length = 0;
    fread(&length, sizeof(length), 1, pFile);
    if (feof(pFile))
        return NULL;
    
    if (length + 1 > MAXLEN)
    {
        fprintf(stderr, "\nName too long. Ending program.");
        exit(1);
    }

    fread(precord->name, sizeof(char), length, pFile);
    precord->name[length] = '\0';
    fread(&precord->age, sizeof(precord->age), 1, pFile);

    return precord;
}

void writefile(char *filename, char *mode)
{
    char answer = 'y';
    
    FILE *pFile = fopen(filename, mode);
    if (NULL == pFile)
    {
        fprintf(stderr, "\n File open failed.");
        exit(1);
    }

    do 
    {
        struct Record record;
        wirterecord(getrecord(&record), pFile);

        printf("Do you want to enter another(y or n)? ");
        scanf("\n%c", &answer);

        fflush(stdin);
    } while ('y' == tolower(answer));

    fclose(pFile);
}

void listfile(char *filename)
{
    char format[15];
    sprintf(format, "\n%%-%ds Age:%%4d", MAXLEN);

    FILE *pFile = fopen(filename, "rb");
    if (NULL == pFile)
    {
        printf("Unable to open %s. Verify it exists or not.\n", filename);
        return;
    }

    struct Record record;
    printf("\nThe contents of %s are:", filename);

    while (NULL != readrecord(&record, pFile))
        printf(format, record.name, record.age);
    printf("\n");

    fclose(pFile);
}

void updatefile(char *filename)
{
    char answer = 'y';

    FILE *pFile = fopen(filename, "rb+");
    if (NULL == pFile)
    {
        fprintf(stderr, "\n File open for updating records failed.");
        return;
    }

    struct Record record;
    int index = findrecord(&record, pFile);
    if (index < 0)
    {
        printf("\nRecord not found.");
        return;
    }

    printf("\n%s is aged %d,", record.name, record.age);
    struct Record newrecord;
    printf("You can now enter the new name and age for %s.", record.name);
    getrecord(&newrecord);

    if (strlen(record.name) == strlen(newrecord.name))
    {
        fseek(pFile, -(long)(sizeof(size_t) + strlen(record.name) + sizeof(record.age)), SEEK_CUR);
        wirterecord(&newrecord, pFile);
    }
    else
        duplicatefile(&newrecord, index, filename, pFile);

    printf("File update complete.\n");
}

void duplicatefile(struct Record *pnewrecord, int index, char *filename, FILE *pFile)
{
    char tempname[L_tmpnam];
    if (NULL == tmpnam(tempname))
    {
        printf("\nTemporary file name creation failed.");
        return;
    }
    char tempfile[strlen(dirpath) + strlen(tempname) + 1];
    strcpy(tempfile, dirpath);
    strcpy(tempfile, tempname);
    FILE *ptempfile = fopen(tempfile, "wb+");

    rewind(pFile);

    struct Record record;
    for (int i = 0; i < index; i++)
        wirterecord(readrecord(&record, pFile), ptempfile);

    wirterecord(pnewrecord, ptempfile);
    readrecord(&record, pFile);

    while (readrecord(&record, pFile))
        wirterecord(&record, ptempfile);

    if (EOF == fclose(pFile))
        printf("\n Failed to close %s", filename);
    if (EOF == fclose(ptempfile))
        printf("\n Failed to close %s", tempfile);

    if (!remove(filename))
    {
        printf("\nRemoving the old file failed. Check file in %s", dirpath);
        return;
    }

    if (!rename(tempfile, filename))
        printf("\nRenaming the file copy failed. Check file in %s", dirpath);
}

int findrecord(struct Record *precord, FILE *pFile)
{
    char name[MAXLEN];
    printf("\nEnter the name for the record you wish to find: ");
    getname(name);

    rewind(pFile);
    int index = 0;

    while (true)
    {
        readrecord(precord, pFile);
        if (feof(pFile))
            return -1;
        if (!strcmp(name, precord->name))
            break;
        ++index;
    }

    return index;
}

