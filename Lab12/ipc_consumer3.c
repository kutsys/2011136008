#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    message_buf rbuf;
    char buffer[MSGSZ];
    char* studentName = "김경환";
    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = 1234;
    sprintf(buffer,"%d,%s",getpid(),studentName);

    // IPC_CREATE 를 인자로 넣으면 메시지 큐를 생성한다.

    if ((msqid = msgget(key, msgflg )) < 0) {

        perror("msgget");
        exit(1);
    }

    /*
     * We'll send message type 1
     */
     
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

      
   if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
   }
   printf("Produce Id , StudentID : %s\n", rbuf.mtext);

    exit(0);
}

