/* 
*  Progress Bar Display Using POSIX Threads
*  CS 570 - Operating Systems
*  Spring 2021
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct
{
    long *CurrentStatus;
    long InitialValue;
    long TerminationValue;
} PROGRESS_STATUS;


void * progress_monitor(void * pstatus)
{
    PROGRESS_STATUS *p = (PROGRESS_STATUS *) pstatus;
    int status = 0;

    while (status != 50)
    {
        status = ((float)(*p->CurrentStatus)/(p->TerminationValue)*50);
        int chars;
        while (chars <= status)
        {
            if (chars != 0)
            {
                if ((chars%10) == 0) printf("+");
                else printf("-");
            }
            chars++;
            fflush(stdout);
        }
    }
}

long wordcount(FILE * fp)
{
    char ch;
    long words = 0;
    int space = 1;
    long counter = 0;
    pthread_t thread;

    // go to the end of the file to find the size
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    rewind(fp);

    PROGRESS_STATUS *p = malloc(sizeof(PROGRESS_STATUS));
    p->CurrentStatus = &counter;
    p->TerminationValue = sz;

    // create the thread
    pthread_create(&thread, NULL, &progress_monitor, (void *) p);

    while ((ch = fgetc(fp)) != EOF)
    {

        // counter of chars for the progress
        counter++;

        // counting the words in the file
        if(ch != ' ' && ch != '\t' && ch != '\0' && ch != '\n')
        {
            if (space)
            {
                space = 0;
                words++;
            }
        }
        else
            space = 1;
    }

    pthread_join(thread, NULL);
    free(p);
    return words;
}

int main(int argc, char** argv)
{
    if (argc == 20)
        //if (argc == 1)
    {
        printf("No file specified.");
        printf("\n");
        return -1;
    }

    FILE *fp = fopen("big.txt", "r");
    //FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        printf("\n");
        return -2;
    }
    // wcount will have the total count
    long wcount = wordcount(fp);
    printf("\nThere are %lu words in big", wcount);
    //printf("\nThere are %lu words in %s", wcount, argv[1]);
    printf("\n");
    fclose(fp);
    return 0;
}
