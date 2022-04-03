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

Block *available_ptr;
int next_block_i = 0;
Hole *hole_ptr;
int max_mem;
int allocated_mem;

void run();
void status(Hole *hole, Block *allocated);
int request(int process, int mem);
int release(int process);

typedef struct block
{
    int process;
    int start; //address
    int end;
} Block;

typedef struct hole
{
    int length;
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
	int process;
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
	
	process = atoi(command[1]); //the number in process
        command = strtok(NULL, " ");
        if (strstr(temp, "RQ") != NULL)
        {
	    mem = atoi(command);
            if (request(process, mem) == 0)
            {
                printf("No hole of sufficient size\n");
            }
	    else
            	printf("Successfully allocated %d to process P%d\n", mem, process);
        }
        else if (strstr(temp, "RL") != NULL)
        {
	    printf("releasing memory for process P%d\n", process);
            if (release(process) == 0)
            {
                printf("No partitions to release\n");
            }
	    else
            	printf("Successfully released memory for process P%d\n", process);
        }
        else if(strstr(temp, "Status") != NULL){
        	status(hole_ptr, available_ptr);
        }
    }
}

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

int request(int process, int mem){
	int pass = 0;
	int smallest_mem = max_mem;
	int smallest_i = null;
	for(int i=0; i < 50; i++){
		if(hole_ptr[i] != null && mem <= hole_ptr[i].length && hole_ptr[i].length <= smallest_mem){
			smallest_i = i;
			smallest_mem = hole_ptr[i].length;
			pass = 1;
		}
	}
	if(pass == 1){
		Block new;
		new.process = process;
		new.start = hole_ptr[smallest_i].start;
		new.end = new.start + mem-1;
		available_ptr[next_block_i] = new;
		next_block_i++;
		hole_ptr[smallest_i].length = hole_ptr[smallest_i].length - mem;
		hole_ptr[smallest_i].start += mem;
		if(hole_ptr[smallest_i].length == 0){
			hole_ptr[smallest_i] = null;
		}
		allocated_mem += mem;
	}
	return pass;
}

int release(int process){
	int pass = 0;
	int target_i = null;
	int merged = 0;
	for(int i=0; i < 50; i++){
		if(available_ptr[i] != null && available_ptr[i].process == process){
			target_i = i;
			pass = 1;
		}
	}
	if(pass == 1){
		allocated_mem -= (available_ptr[target_i].end - available_ptr[target_i].start)+1;
		
		for(int i=0; i < 50; i++){
			if(hole_ptr[i] != null && merged == 0){
				if(available_ptr[target_i].start -1 == hole_ptr[i].end){
					hole_ptr[i].length += (available_ptr[target_i].end-available_ptr[target_i].start+1);
					hole_ptr[i].end = available_ptr[target_i].end;
					merged = 1;
				}
				else if(available_ptr[target_i].end +1 == hole_ptr[i].start){
					hole_ptr[i].length += (available_ptr[target_i].end-available_ptr[target_i].start+1);
					hole_ptr[i].start = available_ptr[target_i].start;
					merged = 1;
				}
			}
			//if merged, check if it was surrounded on both sides
			if(hole_ptr[i] != null && merged == 1){
				if(hole_ptr[i].start -1 == hole_ptr[i-1].end){
					hole_ptr[i-1].length += hole_ptr[i].length;
					hole_ptr[i-1].end = hole_ptr[i].end;
					hole_ptr[i] = null;
				}
			}
		}
		available_ptr[target_i] = null;
	}
	
	return pass;
}
