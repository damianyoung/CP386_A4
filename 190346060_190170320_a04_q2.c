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

Block *available_ptr;
Hole *hole_ptr;
int max_mem;
int allocated_mem;

void run();
int request(int mem);
int release(int mem);
void status(Hole *hole, Block *allocated);

typedef struct block
{
    int process;
    int start; //address
    int end;
} Block;

typedef struct hole
{
    int lenght;
    int start; //address
    int end;
} Hole;

int main(int argc, char *args[])
{
	if (argc > 1)
	{
		max_mem = args[1];
		allocated_mem = 0;
		Hole *holes = (Hole *)malloc(50 * sizeof(Hole));
		Block *blocks = (Block *)malloc(50 * sizeof(Block));
		Hole first;
		first.length = max_mem;
		first.start = 0;
		first.end = max_mem -1;
		blocks[0] =
		available_ptr = &blocks[0];
		hole_ptr = &holes[0];
		run();
	}

}

void run()
{
    char cmd[100];
    bool running = true;

    while (running)
    {
        printf("command>");

        scanf("%s", &cmd);
        char *command = strtok(cmd, " ");
        char *temp = command;

        command = strtok(NULL, " ");
        int mem;
        char algorithm;
        if(strstr(temp, "Exit") != NULL){
        	exit(0);
        }
        else if (command != NULL)
        {
        	command = strtok(NULL, " ");
        	mem = atoi(command);
        	command = strtok(NULL, " ");
        	algorithm = command;
        }


        if (strstr(temp, "RQ") != NULL) //tbd
        {
            if (request(mem) == 0)
            {
                printf("System not in safe state\n");
            }
        }
        else if (strstr(temp, "RL") != NULL)
        {
            if (release(mem) == 0)
            {
                printf("System not in safe state\n");
            }
        }
    }
}
