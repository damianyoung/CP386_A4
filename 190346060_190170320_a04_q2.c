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
void status(Hole *hole, Block *allocated);
int request(int mem);
int release(int mem);

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

void status(Hole *hole, Block *allocated){
	printf("Partitions [Allocated memory = %d]:\n", allocated_mem);
	for(int i=0; i < 50; i++){
		if(allocated[i] != null){
			printf("Address [%d:%d] Process P%d\n", allocated[i].start, allocated[i].end, allocated[i].process);
		}
	}
	printf("\n");
	printf("Holes [Free memory = %d]:\n", max_mem - allocated_mem);
	for(int i=0; i < 50; i++){
		if(hole[i] != null){
			printf("Address [%d:%d] len = %d\n", hole[i].start, hole[i].end, hole[i].length);
		}
	}
}
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
        char algorithm; //Note: for Best-Fit algorithm, so it is redundant
        if(strstr(temp, "Exit") != NULL){
        	running = false;
        }
        /*else if (command != NULL)
        {
        	command = strtok(NULL, " ");
        	mem = atoi(command);
        	command = strtok(NULL, " ");
        	algorithm = command;
        }*/

        if (strstr(temp, "RQ") != NULL)
        {
	    mem = atoi(command);
            if (request(mem) == 0)
            {
                printf("No hole of sufficient size\n");
            }
        }
        else if (strstr(temp, "RL") != NULL)
        {
	    mem = atoi(command[1]); //the number in process
            if (release(mem) == 0)
            {
                printf("No partitions to release\n");
            }
        }
        else if(strstr(temp, "Status") != NULL){
        	status(hole_ptr, available_ptr);
        }
    }
}
