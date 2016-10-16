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
    int nread;
    FILE* in;
    FILE* out;
    clock_t newtime = 0;
    clock_t oldtime = 0;
    double ElapsedTime = 0.0;
    char msg[16] = "Process in";

    in = fopen("test.zip","rb");
    out = fopen("copied.zip","wb");

    write(1,msg,16);
    
    oldtime = clock();

    while((nread = fread(data,sizeof(char),sizeof(data),in)) > 0){
	
	fwrite(data,sizeof(char),nread,out);
	
	newtime = clock();
	ElapsedTime = (double)(newtime - oldtime)/CLOCKS_PER_SEC;

	if(ElapsedTime >= 0.5) {
	    oldtime = clock();
            write(1,"*",1);
	    fflush(stdout);
	}
	

    }
    fclose(in);
    fclose(out);

}
