#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSZ     128
#define CMD_LENGTH 512
#define KEY_NUM	  1234

/*
 * Declare the message structure.
 */


typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

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
void main()
{
    int msqid;
    int msgflg = 0666;
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

    if(get_pid_by_process_name("pro") == -1){
	printf("producer를 먼저 실행해야 합니다\n");
	exit(1);
    }

    key = KEY_NUM;
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
       printf ("%d %d, %s, %d\n", msqid, (int)sbuf.mtype, sbuf.mtext, (int)buf_length);
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

