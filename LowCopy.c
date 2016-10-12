#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdio.h>

int main(char* argv[])
{
    char data[1024];
    int in,out;
    int nread;
    char* output;    
    clock_t newtime = 0;
    clock_t oldtime = 0;
    double ElapsedTime = 0;
    char msg[16] = "Process in";


    in = open("test.zip",O_RDONLY);
    out = open("copied.zip",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);


    write(1,msg,16);
    oldtime = clock();

    while((nread = read(in,data,sizeof(data))) > 0){
	
	write(out,data,nread);

	newtime = clock();
	ElapsedTime = (double)(newtime - oldtime)/CLOCKS_PER_SEC;

	if(ElapsedTime >= 0.5) {
	    write(1,"*",1);
	    oldtime = clock();
	}

    }
    close(in);
    close(out);

}
