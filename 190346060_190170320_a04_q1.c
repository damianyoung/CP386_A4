#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

int *available_ptr;
int *max_ptr;
int *allocated_ptr;
int *need_ptr;

int customers = 0; // customers
int resources = 0; // rows

int readFileCustomer(char *fileName);
void readFileSeq(char *fileName, int max[customers][resources]);
bool safety(int *available, int *max, int *allocated, int *need);
int sumArr(int arr[], int n);
void findNumCol(char *fileName);
void banker();
void run();
int request(int args[]);
int release(int args[]);
void status(int *available, int *max, int *allocated, int *need);

typedef struct thread
{
    char tid[4];
    unsigned int startTime;
    int state;
    pthread_t handle;
    int ret;
} Thread;

int main(int argc, char *argv[])
{
    findNumCol("sample4_in.txt");
    customers = readFileCustomer("sample4_in.txt");
    if (argc != resources + 1)
    {
        printf("Incorrect command line arguments\n");
        return -1;
    }

    printf("Total # of Customers: %d\n", customers);
    printf("Current number of available resources: \n");
    // Make two for loops below into one loop
    for (int i = 1; i < resources; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("%s\n", argv[resources]);

    int *available = (int *)malloc(resources * sizeof(int));
    for (int i = 1; i < resources; i++)
    {
        available[i - 1] = atoi(argv[i]);
    }
    // Make two for loops above into one loop

    int allocated[customers][resources];
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            allocated[i][j] = 0;
        }
    }

    int max[customers][resources];
    printf("Maximum resources available to each customer:\n");
    readFileSeq("sample4_in.txt", max);

    int need[customers][resources];
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            need[i][j] = max[i][j] - allocated[i][j];
        }
    }

    available_ptr = &available[0];
    max_ptr = &max[0][0];
    allocated_ptr = &allocated[0][0];
    need_ptr = &need[0][0];

    run();
    return 0;
}

void findNumCol(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    char *letter;
    if (!feof(fp))
    {
        char line[100];
        fgets(line, 100, fp);
        letter = strtok(line, ",");
        resources = 0;
        while (letter != NULL)
        {
            resources++;
            letter = strtok(NULL, ",");
        }
    }
    fclose(fp);
}

int readFileCustomer(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    int count = 0;
    struct stat st;
    fstat(fileno(fp), &st);
    char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
    fileContent[0] = '\0';

    while (!feof(fp))
    {
        char line[100];
        fgets(line, 100, fp);
        strcat(fileContent, line);
        count++;
    }

    fclose(fp);
    return count;
}

void readFileSeq(char *fileName, int max[customers][resources])
{
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    char *letter;

    int i = 0;
    while (!feof(fp))
    {
        char line[100];
        fgets(line, 100, fp);
        printf("%s", line);
        letter = strtok(line, ",");
        int j = 0;
        while (letter != NULL)
        {
            max[i][j] = atoi(letter);
            letter = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    printf("\n");
    fclose(fp);
}

// probs gonna just implement this in main later
void run()
{
    char cmd[100];
    bool running = true;

    while (running)
    {
        printf("Enter a command: ");
        fgets(cmd, 100, stdin);
        char *command = strtok(cmd, " ");

        letter = strtok(NULL, " ");
        while (letter != NULL)
        {
            letter = strtok(NULL, " ");
        }
    }
}