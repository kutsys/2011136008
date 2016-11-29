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
	int res;
	int bytes_sent = 0;
	char* studentNum = "2011136008";
	char recv_pid[1024];
	char recv_message[1024];
	char buffer[BUFFER_SIZE + 1];

	if(access(FIFO_NAME,F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME,0777);
		if(res != 0)
		{
			fprintf(stderr,"Could not create fifo %s\n",FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	pipe_fd = open(FIFO_NAME,O_WRONLY);

	SendData(pipe_fd , getpid() , studentNum);

    	pipe_fd = open(FIFO_NAME,O_RDONLY);

	recvData(pipe_fd , recv_pid , recv_message);

	printf("Student Number : %s , Name : %s , produce PID : %d, consume PID %s\n ",studentNum, recv_message, getpid(), recv_pid);

	printf("BYE!\n");
}
