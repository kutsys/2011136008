#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define MAX_COUNT 20

pthread_mutex_t work_mutex;
void * test_funct(void* arg);
int step[10];
int complete = 0;

void introduce()
{
	printf("2011136008 김경환 \n");
}


int sumofstep(int _step[10])
{
	int i;
	int sum = 0;
	for (i = 0; i<10 ;i++)
		sum += _step[i];

	return sum;
}

void initstep(int* _step,int length)
{
	int i;
	for(i =0;i<length;i++)
		_step[i] = 0;
}


int main()
{
    int res;
	pthread_t threads[10];
	void * thread_result;
	int i;
	int status;

	introduce();
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
	int thread_index = (int)arg;


	while(count < MAX_COUNT){

	    count++;
	
	    seed = rand()%1000 + 1;

	    srand(seed);

	    r = rand()%10;

	    timer = time(NULL);
	    t = localtime(&timer);

	    sleep(r);

	    printf("Num : [%d] Current Time : [%s] Funct count : [%d]\n",
		   thread_index,timeToString(t),count);
	    fflush(stdout);

        pthread_mutex_lock(&work_mutex);
			step[thread_index] = 1;
			if(sumofstep(step) == 10)
			{
				complete = 1;
				initstep(step,10);
			}
		pthread_mutex_unlock(&work_mutex);

			while(step[thread_index] ==1);
	}
	pthread_exit("Thread Exit");
}


