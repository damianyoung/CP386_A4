#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

typedef struct Customer
{
    int customer_id;
    int item1;
    int item2;
    int item3;
    int item4;
} Customer;

int readFile(char *fileName, Customer *customer);
int safetyAlgorithm(int customerCount);
void requestResource(int threadID, int item1, int item2, int item3, int item4);
void releaseResource(int threadID, int item1, int item2, int item3, int item4);
void printValues(int customerCount);
void runProgram(int customerCount);
void *runThread(void *threadID);

int available[5] = {0, 0, 0, 0, 0};

Customer *customerMax = NULL;
Customer *customerAlloc = NULL;
Customer *customerNeed = NULL;

int main(int argc, char *argv[])
{
    char *filename = "sample4_in.txt";

    if (argc != 5)
    {
        printf("4 values must be entered to continue");
    }

    return 0;
}