#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSGSZ     128


/*
 * Declare the message structure.
 */

typedef struct msgbuf {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;


void main()
{
    int msqid;
    key_t key;
    message_buf rbuf;
    message_buf sbuf;
    int msgflg = IPC_CREAT | 0666;
    size_t buf_length;
    char buffer[MSGSZ];
    char* studentID = "2011136008";
    char* token;
    char recv_data[2][MSGSZ];
    int index = 0;   	
    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = 1234;
    sprintf(buffer,"%d,%s",(int)getpid(),studentID);


    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }

    /*
     * Receive an answer of message type 1.
     */
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    sbuf.mtype = 1;
 
    (void) strcpy(sbuf.mtext, buffer);
    buf_length = strlen(sbuf.mtext) + 1 ;
    
    /*
     * Send a message.
     */
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %s, %d\n", msqid, (int)sbuf.mtype, sbuf.mtext, (int)buf_length);
        perror("msgsnd");
        exit(1);
    }

    /*
     * Print the answer.
     */
    token = strtok(rbuf.mtext,",");
	while( token != NULL)
	{
		strcpy(recv_data[index],token);
		token = strtok(NULL,","); 
		index++;
	}
	printf("Student Number : %s\nName : %s\nproduce PID : %d\nconsume : PID %s\n ",studentID, recv_data[1], getpid(), recv_data[0]);

    exit(0);
}

