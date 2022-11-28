#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr, cli;

    if (sockfd = socket(AF_INET, SOCK_STREAM, 0) == -1)
    {
        perror("Error");
        exit(-1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Error");
        exit(-1);
    }
    
    char buf[BUFSIZ], *result;
    while((result = fgets(buf, BUFSIZ, stdin)) != NULL)
    {
        if (write(sockfd, buf, strlen(buf)) == -1)
        {
            perror("Error");
            exit(-1);
        }
        bzero(buf, BUFSIZ);
    }

    return 0;
}