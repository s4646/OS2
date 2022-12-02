#include "execute.h"
#include "dir.h"
#include "copy.h"

int cmd(char** res)
{
    char* result = *res;
    char *token;
    char * temp = (char*)malloc(strlen(result)+1);
    int counter = 0;
    int i = 0;
    
    memcpy(temp, result, strlen(result)+1);
    // count arguments
    while ((token = strtok_r(temp, " ", &temp)) != NULL)
    {
        counter++;
    }
    char *args[counter + 1];
    args[counter] = NULL;
    
    // build args array
    while ((token = strtok_r(result, " ", &result)) != NULL)
    {
        if (i==0)
        {
            args[i] = calloc(strlen(token) + 5, 1);
            memcpy(args[i], "/bin/", 5);
            args[i] = strcat(args[i], token);
            i++;
        }
        else
        {
            args[i++] = token;
        }
    }

    execve(args[0], args, NULL);
    free(args[0]);
    return 0;
}

int execute(char* pwd, char** stdptr, int sockfd, int sockfd6, struct sockaddr_in* servaddr, struct sockaddr_in* cli, struct sockaddr_in6* servaddr6)
{
    if (strstr(*stdptr, ">")) // output redirection
    {
        char *command = strtok_r(*stdptr, ">", stdptr);
        char *file = strtok_r(*stdptr, ">", stdptr);
        int out = open(strrchr(file, ' ')+1, O_CREAT|O_TRUNC|O_WRONLY, 0600);
        
        int write_out = dup(fileno(stdout));
        dup2(out, fileno(stdout));

        execute(pwd, &command, 0, 0, (struct sockaddr_in*){0}, (struct sockaddr_in*){0}, (struct sockaddr_in6*){0});

        close(out);

        dup2(write_out, fileno(stdout));
        close(write_out);

        return 0;
    }

    if (strstr(*stdptr, "<")) // input redirecion
    {
        char *command = strtok_r(*stdptr, "<", stdptr);
        char *temp = strtok_r(*stdptr, "<", stdptr);
        char *rest = (char*)malloc(strlen(temp)+1);
        memcpy(rest, temp, strlen(temp)+1);

        char *file = strtok_r(rest, " ", &rest);

        int fd = open(file, O_RDONLY);

        char buf[BUFSIZ];
        
        read(fd, buf, BUFSIZ);

        strcat(command, buf);
        strcat(command, rest);

        execute(pwd, &command, 0, 0, (struct sockaddr_in*){0}, (struct sockaddr_in*){0}, (struct sockaddr_in6*){0});
        
        close(fd);

        return 0;
    }
    if (strstr(*stdptr, "}")) // client
    {
        char tempbuf[BUFSIZ];
        bzero(tempbuf, BUFSIZ);

        char *command = strtok_r(*stdptr, "}", stdptr);
        char *ip_port = strtok_r(*stdptr, "}", stdptr);
        
        char *port = strrchr(ip_port, ':')+1;
        char *ip = (char*)malloc(strlen(ip_port)-strlen(port)-1);
        memcpy(ip, ip_port, strlen(ip_port)-strlen(port)-1);

        char *pch = strchr(ip_port, ':');
        pch = strchr(pch + 1, ':');
        if (pch != NULL)
        {
            servaddr6->sin6_family = AF_INET6;
            servaddr6->sin6_port = htons(atoi(port));
            inet_pton(AF_INET6, "::1", &(servaddr6->sin6_addr));

            if (connect(sockfd, (struct sockaddr*)servaddr6, sizeof(*servaddr)) == -1)
            {
                perror("Error");
                exit(-1);
            }

            if (write(sockfd, command, strlen(command)) < 0)
            {
                perror("Error");
                return -1;
            }
            if (read(sockfd, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
            
            if (write(STDOUT_FILENO, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
        }
        else
        {
            servaddr->sin_family = AF_INET;
            servaddr->sin_port = htons(atoi(port));
            servaddr->sin_addr.s_addr = inet_addr(strrchr(ip, ' ')+1);
            if (connect(sockfd, (struct sockaddr*)servaddr, sizeof(*servaddr)) == -1)
            {
                perror("Error");
                exit(-1);
            }

            if (write(sockfd, command, strlen(command)) < 0)
            {
                perror("Error");
                return -1;
            }
            if (read(sockfd, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
            
            if (write(STDOUT_FILENO, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
        }
        free(ip);
        return 0;
    }
    if (strstr(*stdptr, "{")) // server
    {
        int connfd;
        char tempbuf[BUFSIZ], *temptr;
        bzero(tempbuf, BUFSIZ);
        if (strstr(*stdptr, "IP6"))
        {
            char *port = strtok_r(*stdptr, "{", stdptr);
            port = strrchr(port, '.')+1;
            servaddr6->sin6_family = AF_INET6;
            servaddr6->sin6_port = htons(atoi(strrchr(port, ' ')+1));
            servaddr6->sin6_addr = in6addr_any;
            if ((bind(sockfd, (struct sockaddr*)servaddr, sizeof(*servaddr))) == -1)
            {
                perror("Error");
                exit(-1);
            }
            if ((listen(sockfd, 1)) == -1)
            {
                perror("Error");
                exit(-1);
            }
            int len = sizeof(cli);
            connfd = accept(sockfd, (struct sockaddr*)cli, (socklen_t*)&len);
            if (connfd == -1)
            {
                perror("Error");
                exit(-1);
            }

            if (read(connfd, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
        }
        else
        {
            char *port = strtok_r(*stdptr, "{", stdptr);

            servaddr->sin_family = AF_INET;
            servaddr->sin_port = htons(atoi(port));
            servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
            if ((bind(sockfd, (struct sockaddr*)servaddr, sizeof(*servaddr))) == -1)
            {
                perror("Error");
                exit(-1);
            }
            if ((listen(sockfd, 1)) == -1)
            {
                perror("Error");
                exit(-1);
            }
            int len = sizeof(cli);
            connfd = accept(sockfd, (struct sockaddr*)cli, (socklen_t*)&len);
            if (connfd == -1)
            {
                perror("Error");
                exit(-1);
            }

            if (read(connfd, tempbuf, BUFSIZ) < 0)
            {
                perror("Error");
                return -1;
            }
        }
        temptr = tempbuf;

        int save_in = dup(STDOUT_FILENO);
        dup2(connfd, STDOUT_FILENO);
        
        execute(pwd, &temptr, sockfd, sockfd6, (struct sockaddr_in*){0}, (struct sockaddr_in*){0}, (struct sockaddr_in6*){0});
        
        close(connfd);
        dup2(save_in, STDOUT_FILENO);
        close(save_in);
        
        return 0;
    }

    if (strstr(*stdptr, "|") != NULL)
    {
        // https://stackoverflow.com/questions/4235519/counting-number-of-occurrences-of-a-char-in-a-string-in-c
        int pipes = 0;
        char *pch = strchr(*stdptr, '|');
        while (pch != NULL)
        {
            pipes++;
            pch = strchr(pch + 1, '|');
        }

        // create commands array
        char *commands[pipes+1], *token;
        int i = 0;
        while ((token = strtok_r(*stdptr, "|", stdptr)) != NULL)
        {
            commands[i++] = token;
        }

        // https://stackoverflow.com/questions/21914632/implementing-pipe-in-c
        for(i = 0; i < pipes; i++)
        {
            int pfd[2];
            pipe(pfd);

            if (!fork())
            {
                dup2(pfd[1], 1); // remap output back to parent
                close(pfd[1]);
                // DIR
                if (strcmp(commands[i], "DIR") == 0)
                {
                    return dir(pwd);
                }
                // COPY
                else if (memcmp(commands[i], "COPY", 4) == 0)
                {
                    return copy(commands[i]);
                }
                else
                {
                    return cmd(&commands[i]);
                }

            }

            // remap output from previous child to input
            dup2(pfd[0], 0);
            close(pfd[1]);
            wait(NULL);
        }
        // DIR
        if (strcmp(commands[i], "DIR") == 0)
        {
            return dir(pwd);
        }
        // COPY
        else if (memcmp(commands[i], "COPY", 4) == 0)
        {
            return copy(commands[i]);
        }
        else
        {
            int pid = fork();
            if (pid == 0)
            {
                return cmd(&commands[i]);
            }
            else
            {
                wait(NULL);
                return 0;
            }
        }
    }
    else // no pipe
    {
        // DIR
        if (strcmp(*stdptr, "DIR") == 0)
        {
            return dir(pwd);
        }
        // COPY
        else if (memcmp(*stdptr, "COPY", 4) == 0)
        {
            return copy(*stdptr);
        }
        else
        {
            if(!fork())
            {
                return cmd(stdptr);
            }
            else
            {
                wait(NULL);
                return 0;
            }
        }
    }
}
