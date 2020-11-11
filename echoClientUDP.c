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
    struct sockaddr_in fromAddr;
    unsigned short echoServPort;
    unsigned int fromSize;
    char *servIP;
    char *echoString;
    char echoBuffer[ECHOMAX + 1];
    int echoStringLen;
    int respStringLen, sendlen;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> [<Echo Port>]\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];
    echoServPort = atoi(argv[2]);

    echoString = malloc(ECHOMAX);
    memset(echoString, 0, ECHOMAX);

    int next = 1;

    while (next) {
        printf("Enter Test\n");

        fgets(echoString, sizeof echoString, stdin);

        if ((echoStringLen = sizeof(echoString)) > ECHOMAX)
            DieWithError("Echo word too long");

        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("socket() failed");

        memset(&echoServAddr, 0, sizeof(echoServAddr));
        echoServAddr.sin_family = AF_INET;

        // Convert the proposed dotted Internet address servIP
        // to a 4-byte integer, and assign it to sin_addr
        echoServAddr.sin_addr.s_addr = inet_addr(servIP);

        // echoServPort to network-byte order, and assign it to sin_port.
        echoServAddr.sin_port = htons(echoServPort);

        sendlen = sendto(sock, echoString, ECHOMAX, 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));

        if (sendlen < 0)
            DieWithError("sendto() sent a different number of bytes than expected.");

        fromSize = sizeof(fromAddr);
        respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&fromAddr, &fromSize);

        if (respStringLen < 0)
            DieWithError("recvfrom() failed");

        echoBuffer[respStringLen] = '\0';
        printf("Echo Server Addr: %s\n", inet_ntoa(echoServAddr.sin_addr));
        printf("From Server Addr: %s\n", inet_ntoa(fromAddr.sin_addr));
		printf("From Server Port: %d\n", (int) ntohs(fromAddr.sin_port));

        printf("Received: %s\n", echoBuffer);

        char *yes;
        yes = malloc(2);
        printf("Quit (y - yes)? ");
        fgets(yes, sizeof yes, stdin);
        if (*yes == 'y') {
            next = 0;
        }
    }

    close(sock);
    exit(0);
}