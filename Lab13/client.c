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

    int client_len;
    int client_sockfd;

    char send_buf[255];
    char recv_buf[255];
    char studentName[20] = "김경환";

    struct sockaddr_in clientaddr;

    if (argc != 2)
    {
        printf("Usage : ./zipcode_cl [port]\n");
        printf("예    : ./zipcode_cl 4444\n");
        exit(0);
    }

    sprintf(send_buf,"%d,%s",getpid(),studentName);

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientaddr.sin_port = htons(atoi(argv[1]));

    client_len = sizeof(clientaddr);

    if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
    {
        perror("Connect error: ");
        exit(0);
    }

// Read & Write

    write(client_sockfd, send_buf, 255);

    read(client_sockfd, recv_buf, 255);

    printf("Produce Id , Student Number : %s\n",recv_buf);

    close(client_sockfd);
    exit(0);
}
