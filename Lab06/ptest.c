#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>

#define TIME_INTERVAL 2

void ring(int sig){
	struct tm *ctime;
	time_t t;
	time(&t);
	ctime = localtime(&t);

	printf("ptest : %4d-%2d-%3d, %2d:%2d:%2d\n",
		ctime->tm_year+1900, ctime->tm_mon+1, ctime->tm_mday,
		ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

	alarm(TIME_INTERVAL);
}

int main(void)
{
    
    signal(SIGALRM,ring);
    alarm(TIME_INTERVAL);

    while(1);

    return 0;
}
