#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
    int sockfd, connfd;
    int opt = getopt(argc, argv, "l");
    struct sockaddr_in servaddr, cli;
    bzero(&servaddr, sizeof(servaddr));
    char buf[BUFSIZ], *result;

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error");
        exit(-1);
    }

    // check for -l flag
    if (((opt != 108) && (opt != -1)) || (argc == 1))
    {
        printf("Usage: ./mync <IP> <Port> OR ./mync -l <Port>\n");
        exit(-1);
    }

    // sender
    if (opt == -1)
    {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
        servaddr.sin_addr.s_addr = inet_addr(argv[1]);
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
        {
            perror("Error");
            exit(-1);
        }

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

    // listener
    else
    {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) == -1)
        {
            perror("Error");
            exit(-1);
        }
        if ((listen(sockfd, 5)) == -1)
        {
            perror("Error");
            exit(-1);
        }
        int len = sizeof(cli);
        connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
        if (connfd == -1)
        {
            perror("Error");
            exit(-1);
        }
        int test;
        while(1)
        {
            test = read(connfd, buf, BUFSIZ);
            if (test == -1)
            {
                perror("Error");
                exit(-1);
            }
            if (write(2, buf, strlen(buf)) == -1)
            {
                perror("Error");
                exit(-1);
            }
            bzero(buf, BUFSIZ);
        }
        
        return 0;
    }

}