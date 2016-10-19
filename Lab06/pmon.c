#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<termios.h>
#include<fcntl.h>
#include<sys/types.h>


/*
    pmon's spec
    
    1. 5초 간격으로 ptest 프로그램의 실행 여부 출력
    2. 존재 여부를 확인하고 존재하면 running , 없으면 not existed
    3. 명령 프롬프트 구현
	1) Q pmon 프로그램 종료
	2) K ptest 의 강제 종료
	3) S ptest 프로그램 실행, 이미 실행중이면 already exist
	4) R ptest 종료 후 재실행, ptest가 없는 경우 , "newly started" 출력
 
*/


#define TIME_INTERVAL 5
static struct termios initial_settings, new_settings;
static int peek_character = -1;

int get_pid_by_process_name(char *process_name)
{
        int pid = -1; 
	char cmd_string[512];
	FILE *fp;

	sprintf(cmd_string, "pgrep %s", process_name);

	fp = popen(cmd_string, "r");
	fseek(fp, 0, SEEK_SET);
	fscanf(fp, "%d", &pid);

	fclose(fp);

	if (pid < 1)
	    pid = -1;

	return pid;
}


void init_keyboard()
{
	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}


void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}

int _kbhit()
{
	unsigned char ch;
	int nread;

	if (peek_character != -1) return 1;
	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0,&ch,1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_settings);
	if(nread == 1) 
	{
	    peek_character = ch;
	    return 1;
	}
	return 0;
}

int _getch()
{
	char ch;

	if(peek_character != -1) 
	{
	    ch = peek_character;
	    peek_character = -1;
	    return ch;
	}
	read(0,&ch,1);

	return ch;
}
int _putch(int c) {
	putchar(c);
	fflush(stdout);
	return c;
}



void ring(int sig)
{
    int pid = get_pid_by_process_name("ptest");

    if(pid == -1)
	printf("\n[%s]\n>>","is not exist");
    else
	printf("\n[%d][%s]\n>>",pid,"running");

    
    fflush(stdout);
    alarm(TIME_INTERVAL);
}

void kill_process(char * pname)
{
    int pid = get_pid_by_process_name(pname);
    if(pid != -1)
	kill(pid,SIGKILL);
    else
	printf("프로세스가 존재하지 않습니다\n");

}

void start_process(char* pname)
{
    int pid = get_pid_by_process_name(pname);
    if(pid != -1)
    {
	printf("프로세스가 이미 존재합니다");
	return;
    }
    else
	system("./ptest");
}
void restart_process(char* pname)
{
    kill_process(pname);
    start_process(pname);

}

int main(void)
{
    int menu = 0;
    int pid = 0;

    init_keyboard();
    signal(SIGALRM,ring);
    alarm(TIME_INTERVAL);

    printf(">>");
    fflush(stdout);
    while(1)
    {
	if(_kbhit())
	{
	    menu = _getch();
	    _putch(menu);
	
	switch(menu)
	{
	    case 'Q':
		close_keyboard();
		exit(1);
		break;
	    case 'K':
		kill_process("ptest");
		break;
	    case 'S':
		start_process("ptest");
		printf("S");
		break;
	    case 'R':
		restart_process("ptest");
		printf("R");
		break;
	    default:
		break;
	}
	printf("\n>>");
	fflush(stdout);
	}
    }
    return 0;
}
