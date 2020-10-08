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
	printf("Server is waiting connections at prot *** %d ***\n", port);
	listen(listenfd, LISTENQ);
	while (1)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *)&cliaddr, &len);

        while ((n = read(connfd, receiveBuff, MAXLINE)) > 0)
        {
            receiveBuff[n] = 0;
            printf("client message: ");
            // printf("%s", receiveBuff);
            if (fputs(receiveBuff, stdout) == EOF)
            {
                    printf("fputs error");
            }
            if (n < 0)
                printf("read error");

            write(connfd, receiveBuff, strlen(receiveBuff));
            printf("\n");

            exit(0);

        }

		close(connfd);
	}
}
