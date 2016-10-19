#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>

#define TIME_INTERVAL 2

void ring(int sig)
{
    time_t _localtime = time(NULL);
    printf("\nlocal time of System : %d\n",(int)_localtime);
    fflush(stdout);
    alarm(TIME_INTERVAL);
}
int main(void)
{
    
    signal(SIGALRM,ring);
    alarm(TIME_INTERVAL);

    while(1);

    return 0;
}
