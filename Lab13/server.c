#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int server_sockfd, client_sockfd;
    int state, client_len;
    int pid;
    char* token;
    char recv_data[2][128];
    int index = 0;

    struct sockaddr_in clientaddr, serveraddr;

    char send_buf[255];
    char recv_buf[255];
    char studentNum[20] = "2011136008";
    if (argc != 2)
    {
        printf("Usage : ./zipcode [port]\n");
        printf("예  : ./zipcode 4444\n");
        exit(0);
    }
    sprintf(send_buf,"%d,%s",getpid(),studentNum);
    state = 0;

    // 주소 파일을 읽어들인다.
    client_len = sizeof(clientaddr);

    // internet 기반의 소켓 생성 (INET)
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error : ");
        exit(0);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    state = bind(server_sockfd , (struct sockaddr *)&serveraddr,
            sizeof(serveraddr));
    if (state == -1)
    {
        perror("bind error : ");
        exit(0);
    }

    state = listen(server_sockfd, 5);
    if (state == -1)
    {
        perror("listen error : ");
        exit(0);
    }
// Read & Write


    client_sockfd = accept(server_sockfd, (struct sockaddr 		*)&clientaddr, &client_len);
    if (client_sockfd == -1)
    {
        perror("Accept error : ");
        exit(0);
    }

    read(client_sockfd, recv_buf, 255);

    write(client_sockfd, send_buf, 255);

    token = strtok(recv_buf,",");
    while( token != NULL)
    {
	strcpy(recv_data[index],token);
	token = strtok(NULL,","); 
	index++;
    }
    printf("Student Number : %s\nName : %s\nproduce PID : %d\nconsume : PID %s\n ",studentNum, recv_data[1], getpid(), recv_data[0]);

    close(client_sockfd);
}
