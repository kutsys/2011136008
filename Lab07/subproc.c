#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

int parent_pid;

char* timeToString(struct tm *t) {
	static char s[20];
	
	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
	    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);

	return s;
}

void ifkill(int signo)
{
	kill(parent_pid,SIGALRM);
	exit(1);
}

int test_funct()
{
	static int count = 0;
	char buffer[1024];
	struct tm *t;
	time_t timer;
	char recv1[128]; // pid
	char recv2[128]; // FUNC_COUNT
	int r;	
	int seed;

	count++;
	seed = getpid() - count;

	srand(seed);
	
	r = rand()%10;

	timer = time(NULL);
	t = localtime(&timer);

	sleep(r);

	printf("PID : [%d] Current Time : [%s] Funct count : [%d]\n",
			getpid(),timeToString(t),count);
	fflush(stdout);

	return count;
}

int main(int argc,char* argv[])
{
	int FUNC_COUNT;  // FUNC_COUNT

	(void)signal(SIGTERM,ifkill);
	sscanf(argv[1],"%d",&parent_pid);
	printf("PID OF PARENT : %d \n",parent_pid);
	sscanf(argv[2],"%d",&FUNC_COUNT);
	
	while(1)
	{
		if(test_funct() >= FUNC_COUNT)
		{
			printf("END OF THE FUNC\n");
			if(kill(parent_pid,SIGALRM) == -1)
				printf("Error Send Signal\n");
			exit(1);
		}
	}
	

	return 0;
}
