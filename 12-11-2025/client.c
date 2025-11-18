#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    char buffer[256], c[20000];
    struct sockaddr_in serv;
    struct hostent *server;

    if (argc < 3)
    {
        printf("Error: No hostname or port number provided.\n");
        printf("Usage: ./client <hostname> <portno>\nExample: ./client localhost 7777\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error: No such host.\n");
        exit(1);
    }

    bzero((char *)&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv.sin_addr.s_addr, server->h_length);
    serv.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        printf("Server not responding...\nTerminating client.\n");
        exit(1);
    }

    printf("Enter the file name with complete path:\n");
    scanf("%s", buffer);

    if (write(sockfd, buffer, strlen(buffer)) < 0)
    {
        perror("Error writing to socket");
    }

    bzero(c, sizeof(c));
    printf("Reading file from server...\n");

    n = read(sockfd, c, sizeof(c) - 1);
    if (n < 0)
    {
        perror("Error reading from socket");
    }

    printf("\nClient: Displaying contents of '%s'\n", buffer);
    fputs(c, stdout);
    printf("\n\n");

    close(sockfd);
    return 0;
}
