#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF
#define TEN_MEG ( 1024*1024*10 )
#define CMD_LENGTH 512

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

void SendData(int _pipe_fd , int _pid , char _message[1024])
{
	int bytes_sent 	= 0;	
	int res 	= 0;
	char _send_buf[1024];

	memset(_send_buf,'\0',sizeof(_send_buf));
	sprintf(_send_buf,"%d,%s",_pid,_message);

	if(_pipe_fd != -1)
	{
		while(bytes_sent < TEN_MEG)
		{
			res = write(_pipe_fd,_send_buf,BUFFER_SIZE);
			if(res ==-1)
			{
				fprintf(stderr,"Write error on pipe\n");
				exit(EXIT_FAILURE);
			}
			bytes_sent += res;
		}
		(void)close(_pipe_fd);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}
void recvData(int _pipe_fd , char* _pid , char* _message)
{
	char recv_data[2][BUFFER_SIZE + 1];
	char buffer[BUFFER_SIZE + 1];
	char* token;
	int index = 0;
	int bytes_read = 0;
	int res = 0;

	if(_pipe_fd != -1)
	{
		do
		{
			res = read(_pipe_fd,buffer,BUFFER_SIZE);
			bytes_read += res;
		} while(res>0);
		(void)close(_pipe_fd);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
	// tokenlize part
	token = strtok(buffer,",");
	while( token != NULL)
	{
		strcpy(recv_data[index],token);
		token = strtok(NULL,","); 
		index++;
	}

	strcpy(_pid,recv_data[0]);
	strcpy(_message,recv_data[1]);
}


int main()
{
	int pipe_fd;
	char * name = "kim Kyeong Hwan";
	char send_buf[BUFFER_SIZE + 1];
	char recv_pid[1024];
	char recv_message[1024];

	if(get_pid_by_process_name("pro") == -1){
	    printf("producer를 먼저 실행해야 합니다\n");
	    exit(1);
        }

	pipe_fd = open(FIFO_NAME,O_RDONLY);

	recvData(pipe_fd , recv_pid , recv_message);

	printf("Produce Pid : %s\n",recv_pid);
	printf("Student Num : %s\n",recv_message);


    	pipe_fd = open(FIFO_NAME,O_WRONLY);

	SendData(pipe_fd , getpid() , name);
	printf("BYE!\n");
}
