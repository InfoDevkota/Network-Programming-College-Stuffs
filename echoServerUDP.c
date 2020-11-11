#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */

#define ECHOMAX 255

void DieWithError(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClintAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServPort;
	int recvMsgSize;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
	}

	echoServPort = atoi(argv[1]); //string to integer

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); //host to network byte order
	echoServAddr.sin_port = htons(echoServPort); //host to network byte order.

	if (bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() falied");

	for (;;)
	{
		cliAddrLen = sizeof(echoClintAddr);

		if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&echoClintAddr, &cliAddrLen)) < 0)
			DieWithError("recvfrom() falied");

		printf("Handling Client %s\n", inet_ntoa(echoClintAddr.sin_addr));
		printf("Client Port: %d\n", (int) ntohs(echoClintAddr.sin_port));
		printf("Received size = %d\n", recvMsgSize);
		printf("Received message: ");

		puts(echoBuffer);

		if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClintAddr, sizeof(echoClintAddr)) != recvMsgSize)
			DieWithError("sendto() sent a different number of bytes than expected.");
	}
}