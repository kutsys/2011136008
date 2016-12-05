#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "se_mun.h"

#define SZ_MSG 1024
#define SEMKEY 0x100

struct shared_use_st {

    int mode;
    char message[SZ_MSG];

};

int p(int semid);
int v(int semid);
int initsem(int semkey);

int main()
{
        int running = 1;
        void* shared_memory = (void*)0;
	struct shared_use_st * shared_stuff;
	char buffer[1024];
	int shmid;
	char* studentID = "2011136008";
	char* token;
	char recv_data[2][1024];
	int index = 0;
        int semid, pid = getpid();
 
        if((semid = initsem(SEMKEY)) == -1) exit(1); 
	// prepare send message
	sprintf(buffer,"%d,%s",getpid(),studentID);

	// make shared memory / allocate memory size / and allocate key
	shmid = shmget((key_t)4561,sizeof(struct shared_use_st),0666|IPC_CREAT);

	p(semid);
	// attach shared memory to process
	shared_memory = shmat(shmid,(void*)0,0);
	shared_stuff = (struct shared_use_st *)shared_memory;

	strcpy(shared_stuff->message,buffer);
	
	p(semid);
	v(semid);

	// printf("pro : %s\n",shared_stuff->message);
	token = strtok(shared_stuff->message,",");
	while( token != NULL)
	{
		strcpy(recv_data[index],token);
		token = strtok(NULL,","); 
		index++;
	}
	
	printf("Student Number : %s\nName : %s\nproduce PID : %d\nconsume PID : %s\n",studentID, recv_data[1], getpid(), recv_data[0]);

	
	shmdt(shared_memory);
	exit(EXIT_SUCCESS);

}

