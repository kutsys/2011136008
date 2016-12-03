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
	char buffer[1024];
	int shmid;
	char* studentID = "2011136008";
	char* token;
	char recv_data[2][1024];
	int index = 0;

	// prepare send message
	sprintf(buffer,"%d,%s",getpid(),studentID);

	// make shared memory / allocate memory size / and allocate key
	shmid = shmget((key_t)4561,sizeof(struct shared_use_st),0666|IPC_CREAT);

	// attach shared memory to process
	shared_memory = shmat(shmid,(void*)0,0);
	shared_stuff = (struct shared_use_st *)shared_memory;

	strcpy(shared_stuff->message,buffer);
	shared_stuff->mode = 1;

	while(shared_stuff->mode == 1);

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

