#include<unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>

void printdir(char *dir,int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    // 디렉토리 구조체를 파라미터 값에 의해 채운다.
    if((dp=opendir(dir)) == NULL)
    {
	fprintf(stderr,"cannot open directory : %s \n ",dir);
	return;
    }
    chdir(dir);

    // 디렉토리의 모든 엔트리를 읽는다.
    while((entry = readdir(dp)) !=NULL) 
    {
	// 해당 파일의 상태를 채운다.
	lstat(entry->d_name,&statbuf);
	// 만약 디렉토리라면
	if(S_ISDIR(statbuf.st_mode))
	{
	    // 모든 파일은 . 그리고 .. 경로를 가지고 있는데 이를 보여주지 않는다.		


	    if(strcmp(".",entry->d_name) == 0 ||
		    strcmp("..",entry->d_name) ==0) continue;


	    if(strncmp(".",entry->d_name,1))
		printf("%*s%s/\n",depth,"",entry->d_name);
	    
	    // 해당 디렉토리를 재귀적으로 진입한다.
	    // 뎁스만큼 공백을 출력해서 디렉토리를 쉽게 구별 가능하다.
	    if(strncmp(".",entry->d_name,1))
		printdir(entry->d_name,depth+4);
	} 
	// 앞에가 .으로 시작한다면 출력하지 않는다.
	else if(strncmp(".",entry->d_name,1))
	{ 
	    printf("%*s%s/\n",depth,"",entry->d_name);
	}
	else
	{
	
	}
    }
    chdir("..");
    closedir(dp);
}

int main()
{
    printf("Directory scan of /home : \n");
    printdir("/home",0);
    printf("done.\n");
    exit(0);
}

