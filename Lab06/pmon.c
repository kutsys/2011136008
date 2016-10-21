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


// defines
#define TIME_INTERVAL 5
#define CMD_LENGTH 512

static struct termios initial_settings, new_settings;
static int peek_character = -1;

// consts
const char* TARGET = "ptest";

int get_pid_by_process_name(const char *pname)
{
    int pid = -1; 
    char cmd_string[CMD_LENGTH];
    FILE *fp;

    sprintf(cmd_string, "pgrep %s", pname);

    fp = popen(cmd_string, "r");
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%d", &pid);

    fclose(fp);

    if (pid < 1)
	pid = -1;

    return pid;
}

// init to use Keyboard 
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

// return Keyboard interface
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

// non-wait keyboard input
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

// get char in buffer
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
    int pid = get_pid_by_process_name(TARGET);

    if(pid == -1)
	printf("\n[%s]\n>>","is not exist");
    else
	printf("\n[%d][%s]\n>>",pid,"running");

    
    fflush(stdout);

    //restart alarm...
    alarm(TIME_INTERVAL);
}

void kill_process(const char * pname)
{
    int pid = get_pid_by_process_name(pname);
    if(pid != -1)
	kill(pid,SIGKILL);
    else
	printf("\nProcess is not exist...\n");

}

void start_process(const char* pname)
{
    int pid = get_pid_by_process_name(pname);
    if(pid != -1)
    {
	printf("\nProcess is Already Exist...\n");
	return;
    }
    else
	system("./ptest &");
}
void restart_process(const char* pname)
{
    int pid = get_pid_by_process_name(pname);
    if(pid == -1)
    {
	printf("\nnewly Starting...\n");
	start_process(pname);
    }
    else
    {
	kill_process(pname);
	start_process(pname);
    }
}

void safe_close()
{
    int pid = get_pid_by_process_name(TARGET);
    
    if(pid != -1)
	kill(pid,SIGKILL);

    close_keyboard();
    exit(1);
}

void ifout(int sig)
{
    safe_close();
}

int main(void)
{
    int menu = 0;
    int pid = 0;

    // init keyboard & signal settings ...
    init_keyboard();
    signal(SIGALRM,ring);
    signal(SIGINT,ifout);
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
		safe_close();
		break;
	    case 'K':
		kill_process(TARGET);
		break;
	    case 'S':
		start_process(TARGET);
		break;
	    case 'R':
		restart_process(TARGET);
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
