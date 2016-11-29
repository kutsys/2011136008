#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX_COUNT 20

void * test_funct(void* arg);

int main()
{
    int res;
	pthread_t threads[10];
	void * thread_result;
	int i;
	int status;

	for( i = 0 ; i < 10 ; i++)
	{
		res = pthread_create(&threads[i], NULL, &test_funct,  (void *)i);

		if(res != 0)
			printf("Thread Create Error!\n");
	}

	for (i = 9; i >= 0; i--)
	{
	        res = pthread_join(threads[i], &thread_result);
		
	}
	exit(1);

}

char* timeToString(struct tm *t) {
    static char s[20];
	
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
	t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
	t->tm_hour, t->tm_min, t->tm_sec);

    return s;
}

void * test_funct(void* arg)
{
    int count = 0;
	int seed;
    struct tm *t;
    time_t timer;
    int r;

	while(count < MAX_COUNT){

	    count++;
	    seed = rand()%1000 + 1;

	    srand(seed);

	    r = rand()%10;

	    timer = time(NULL);
	    t = localtime(&timer);

	    sleep(r);

	    printf("Num : [%d] Current Time : [%s] Funct count : [%d]\n",
		   (int)arg,timeToString(t),count);
	    fflush(stdout);

	}
	pthread_exit("Thread Exit");
}


