#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define SZ_MSG 1024

struct shared_use_st {

    int mode;
    char message[SZ_MSG];

};


int main()
{
        int running = 1;
        void* shared_memory = (void*)0;
	struct shared_use_st * shared_stuff;
	char buffer[BUFSIZ];
	int shmid;
	char* studentName = "김경환";

	// prepare send message
	sprintf(buffer,"%d,%s",getpid(),studentName);

	// make shared memory / allocate memory size / and allocate key
	shmid = shmget((key_t)4561,sizeof(struct shared_use_st),0666|IPC_CREAT);

	// attach shared memory to process
	shared_memory = shmat(shmid,(void*)0,0);
	shared_stuff = (struct shared_use_st *)shared_memory;

	printf("Produce PID, StudentID : %s\n",shared_stuff->message);
	strcpy(shared_stuff->message,buffer);	
	shared_stuff->mode = 0;

	shmdt(shared_memory);
	exit(EXIT_SUCCESS);

}
