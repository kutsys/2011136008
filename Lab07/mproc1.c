#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>


//defines
#define FUNC_COUNT 20


const char SUBPROCPATH[] = {"/home/kyeong-hwan/kutsys/subproc"};

void introduce()
{
	printf(" student ID : 2011136008 name : 김경환\n");
}


int main(void)
{
	pid_t funct_pid;
	char buffer[1024];
	char send1[128];
	char send2[128];
	int i;

	introduce();


	
	for(i = 0; i< 10 ; i++)
	{

		funct_pid = fork();
		switch(funct_pid)
		{
			case -1:
				perror("fork failed"); exit(1);
			case 0: // child
				sprintf(send1,"%d",getpid());
				sprintf(send2,"%d",FUNC_COUNT);
				execl(SUBPROCPATH,SUBPROCPATH,send1,send2,NULL);
				exit(1);
				break;
			default: // parent
				break;

		}	
	}
	// wait for process
	for(i = 0 ; i < 10 ; i++)
	{
		int stat_val;
		wait(&stat_val);
	}
	
	return 0;
}
