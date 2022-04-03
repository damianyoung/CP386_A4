/*
Damian Young: 190346060: damianyoung
Johnson Huynh: 190170320: CodeJohnson0162
link: https://github.com/damianyoung/CP386_A4
*/

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

int *availablePtr;
int *maxPtr;
int *allocatedPtr;
int *needPtr;

int customers = 0;
int resources = 0;

int readFileCustomer(char *fileName);
bool safety(int *available, int *max, int *allocated, int *need);
void findNumCol(char *fileName);
int request(int args[]);
int release(int args[]);
void status(int *available, int *max, int *allocated, int *need);
void *threadRun();

int main(int argc, char *argv[])
{
    findNumCol("sample4_in.txt");
    customers = readFileCustomer("sample4_in.txt");
    if (argc != resources + 1)
    {
        printf("Incorrect command line arguments\n");
        return -1;
    }

    printf("Number of Customers: %d\n", customers);
    printf("Currently available resources: ");
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

    int allocated[customers][resources];
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            allocated[i][j] = 0;
        }
    }

    int max[customers][resources];
    printf("Maximum resources from file:\n");
    FILE *fp = fopen("sample4_in.txt", "r");

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
        letter = strtok(line, ",");
        int j = 0;
        while (letter != NULL)
        {
            max[i][j] = atoi(letter);
            printf("%d ", max[i][j]);
            letter = strtok(NULL, ",");
            j++;
        }
        printf("\n");
        i++;
    }
    printf("\n");
    fclose(fp);

    int need[customers][resources];
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            need[i][j] = max[i][j] - allocated[i][j];
        }
    }

    availablePtr = &available[0];
    maxPtr = &max[0][0];
    allocatedPtr = &allocated[0][0];
    needPtr = &need[0][0];

    char cmd[100];
    bool running = true;

    while (running)
    {
        printf("Enter a command: ");

        fgets(cmd, 100, stdin);
        char *command = strtok(cmd, " ");
        char *temp = command;

        command = strtok(NULL, " ");
        int i = 0;
        int args[resources + 1];
        while (command != NULL)
        {
            args[i] = atoi(command);
            i++;
            command = strtok(NULL, " ");
        }

        if (strstr(temp, "RQ") != NULL)
        {
            printf("\nState is safe, and request is satisfied\n");
            if (request(args) == 0)
            {
                printf("System not in safe state\n");
            }
        }
        else if (strstr(temp, "RL") != NULL)
        {
            printf("\nState is safe, and request is satisfied\n");
            if (release(args) == 0)
            {
                printf("System not in safe state\n");
            }
        }
        else if (strstr(temp, "Status"))
        {
            status(availablePtr, maxPtr, allocatedPtr, needPtr);
        }
        else if (strstr(temp, "Run") != NULL)
        {
            threadRun();
        }
        else if (strstr(temp, "Exit") != NULL)
        {
            running = false;
        }
        else
        {
            printf("Invalid command\n");
        }
    }
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

int request(int args[])
{
    int customer = args[0];
    int requested[resources];
    for (int i = 0; i < resources; i++)
    {
        requested[i] = args[i + 1];
    }

    bool isSafe = true;

    for (int i = 0; i < resources && isSafe == true; i++)
    {
        isSafe = requested[i] <= *(needPtr + (customer * resources) + i);
    }

    if (isSafe == true)
    {
        for (int i = 0; i < resources; i++)
        {
            isSafe = requested[i] <= *(availablePtr + i);
        }
        if (isSafe == true)
        {
            for (int i = 0; i < resources; i++)
            {
                availablePtr[i] -= requested[i];
                *((allocatedPtr + (customer * resources)) + i) += requested[i];
                *((needPtr + (customer * resources)) + i) -= requested[i];
            }
            if (safety(availablePtr, maxPtr, allocatedPtr, needPtr))
            {
                return 1;
            }
            else
            {
                for (int i = 0; i < resources; i++)
                {
                    availablePtr[i] += requested[i];
                    *((allocatedPtr + (customer * resources)) + i) -= requested[i];
                    *((needPtr + (customer * resources)) + i) += requested[i];
                }
                printf("System is not in safe state\n");
                return 0;
            }
        }
        else
        {
            printf("System is not in safe state\n");
            return 0;
        }
    }
    else
    {
        printf("System is not in safe state\n");
        return 0;
    }
}

bool safety(int *available, int *max, int *allocated, int *need)
{
    int work[resources];
    for (int i = 0; i < resources; i++)
    {
        work[i] = *(available + i);
    }
    printf("\n");

    bool finish[resources];
    for (int i = 0; i < resources; i++)
    {
        finish[i] = false;
    }

    int safeSeq[resources];
    int count = 0;
    while (count < resources)
    {
        bool found = false;
        for (int i = 0; i < resources; i++)
        {
            if (finish[i] == false)
            {
                int j;
                for (j = 0; j < resources; j++)
                {
                    if (*((need + i * resources) + j) > work[j])
                    {
                        break;
                    }
                }
                if (j == resources)
                {
                    for (int k = 0; k < resources; k++)
                    {
                        work[k] += *((allocated + i * resources) + k);
                    }
                    finish[i] = true;
                    found = true;
                    safeSeq[count++] = i;
                }
            }
        }
        if (found == false)
        {
            printf("System is not in a safe state\n");
            return false;
        }
    }
}

int release(int args[])
{
    int customer = args[0];
    int released[resources];
    bool isSafe = true;

    for (int i = 0; i < resources; i++)
    {
        released[i] = args[i + 1];
    }

    for (int i = 0; i < resources; i++)
    {
        isSafe = released[i] <= *(allocatedPtr + (customer * resources) + i);
        if (isSafe == true)
        {
            for (int j = 0; j < resources; j++)
            {
                *(availablePtr + j) += released[j];
                *((allocatedPtr + (customer * resources)) + j) -= released[j];
                *((needPtr + (customer * resources)) + j) += released[j];
            }
            return 1;
        }
        else
        {
            printf("System is not in a safe state\n");
            return 0;
        }
    }
    return 0;
}

void status(int *available, int *max, int *allocated, int *need)
{
    printf("\n");
    printf("Available\n");
    for (int i = 0; i < customers; i++)
    {
        printf("%d ", *(available + i));
    }
    printf("\n");
    printf("Max\n");
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *(max + (i * resources) + j));
        }
        printf("\n");
    }
    printf("\n");
    printf("Allocated\n");
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *(allocated + (i * resources) + j));
        }
        printf("\n");
    }
    printf("\n");
    printf("Need\n");
    for (int i = 0; i < customers; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *(need + (i * resources) + j));
        }
        printf("\n");
    }
    printf("\n");
}

void *threadRun()
{
    char seq[100];
    int res[resources];
    printf("Safe sequence is: ");
    fgets(seq, 100, stdin);
    printf("\n");
    char *token = strtok(seq, " ");

    int arg[5];
    int count = 0;
    while (token != NULL)
    {
        arg[count] = atoi(token);
        token = strtok(NULL, " ");
        count++;
    }

    for (int i = 0; i < count; i++)
    {
        printf("Customer/Thread %d\n", arg[i]);
        printf("Allocated resources: ");
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *((allocatedPtr + (arg[i] * resources)) + j));
            res[j] = *((allocatedPtr + (arg[i] * resources)) + j);
        }
        printf("\n");
        printf("Needed: ");
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *((needPtr + (arg[i] * resources)) + j));
        }
        printf("\n");
        printf("Available: ");
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *(availablePtr + j));
        }
        printf("\n");
        printf("Thread has started\n");
        printf("Thread has finished\n");
        for (int j = 0; j < resources; j++)
        {
            *(availablePtr + j) += res[j];
            *((allocatedPtr + (arg[i] * resources)) + j) = 0;
            *((needPtr + (arg[i] * resources)) + j) = *(maxPtr + (arg[i] * resources) + j);
        }
        printf("Thread is releasing resources\n");
        printf("New Available: ");
        for (int j = 0; j < resources; j++)
        {
            printf("%d ", *(availablePtr + j));
        }
        printf("\n");
    }
    return NULL;
}
