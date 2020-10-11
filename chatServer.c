#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;
int main(int argc, char **argv)
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    char receiveBuff[MAXLINE];
    char sendBuff[MAXLINE];
    time_t ticks;
    int port;
    socklen_t len;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    printf("Connect client at %s:%d\n ", inet_ntop(AF_INET, &servaddr.sin_addr.s_addr, buff, sizeof(buff)), port);
    listen(listenfd, LISTENQ);

    len = sizeof(cliaddr);
    connfd = accept(listenfd, (SA *)&cliaddr, &len);

    printf("Connection from %s, port %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)),
           ntohs(cliaddr.sin_port));

    // printf("Here\n");

    while (1)
    {
        read(connfd, receiveBuff, MAXLINE);
        printf("Client: ");
        fputs(receiveBuff, stdout);
        bzero(receiveBuff, sizeof(receiveBuff));

        // printf("client writing...");
        // printf("\b\b\b\b\b\b");

        printf("\nServer: ");
        fgets(sendBuff, MAXLINE, stdin);
        write(connfd, sendBuff, strlen(sendBuff));
        bzero(sendBuff, sizeof(sendBuff));

    }

    // close(connfd);
}
