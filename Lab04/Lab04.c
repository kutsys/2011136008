#include<stdlib.h>
#include<stdio.h>

int main(void)
{
	printf("HOME = %s\n",getenv("HOME"));
	printf("PS1 = %s\n",getenv("PS1"));
	printf("PATH = %s\n",getenv("PATH"));
	printf("LD_LIBRARY_PATH = %s\n",getenv("LD_LIBRARY_PATH"));

	setenv("TEST_ENV", "1234" , 1);
	printf("Value of TEST_ENV = %s\n",getenv("TEST_ENV"));

}
