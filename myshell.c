#include "execute.h"

int main()
{
    char pwd[BUFSIZ], stdbuf[BUFSIZ], *stdptr;
    bzero(pwd, BUFSIZ);
    bzero(stdbuf, BUFSIZ);
    int save_in = dup(STDIN_FILENO);
    int save_out = dup(STDOUT_FILENO);

    int sockfd = -1, sockfd6 = -1;
    struct sockaddr_in servaddr, cli;
    bzero(&servaddr, sizeof(servaddr));

    struct sockaddr_in6 servaddr6;
    bzero(&servaddr6, sizeof(servaddr6));
    
    char buf[BUFSIZ];
    bzero(buf, BUFSIZ);

    while(1)
    {
        // https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
        if (getcwd(pwd, BUFSIZ) == NULL)
        {
            perror("Error");
            exit(-1);
        }
        write(STDOUT_FILENO, pwd, strlen(pwd)+2);
        write(STDOUT_FILENO, "$ ", 2);
        
        if (read(STDIN_FILENO, stdbuf, BUFSIZ) < 0)
        {
            break;
        }
        stdbuf[strlen(stdbuf)-1] = '\0';
        stdptr = stdbuf;
        execute(pwd, &stdptr, sockfd, sockfd6, &servaddr, &cli, &servaddr6);
        bzero(pwd, BUFSIZ);
        bzero(stdbuf, BUFSIZ);
        dup2(save_in, STDIN_FILENO);
        dup2(save_out, STDOUT_FILENO);
    }
    
    return 0;
}