#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>


int dir(char* _dir)
{
    DIR *directory;
    struct dirent *temp;
    if ((directory = opendir(_dir)) == NULL)
    {
        perror("Error");
        return 1;
    }
    
    while((temp = readdir(directory)) != NULL)
    {
        printf("%s\n", temp->d_name);
    }
    if (closedir(directory) == -1)
    {
        perror("Error");
        return 1;
    }
    return 0;
}

int copy(char *file1_path, char *file2_path)
{
    int fd1 = open(file1_path, O_RDONLY);
    int fd2 = open(file2_path, O_WRONLY);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("Error");
        return 1;
    }
    if (ftruncate(fd2,0) == -1)
    {
        perror("Error");
        return 1; 
    }
    char c;
    size_t check1, check2;
    
    while(1) // copy files
    {
        check1 = read(fd1,&c,1);
        if (check1 <= 0)
        {
            if (check1 == -1)
            {
                perror("Error");
                return 1;
            }
            if (check1 == 0)
            {
                break;
            }
        }
        check2 = write(fd2,&c,1);
        if (check2 == -1)
        {
            perror("Error");
            return 1;
        }  
    }
    
    if (close(fd1) == -1 || close(fd2) == -1)
    {
        perror("Error");
        exit(-1);
    }
    return 0;
}

int main()
{
    char pwd[BUFSIZ], stdbuf[BUFSIZ], *result = NULL;
    char *token;
    char *file1_path = (char*)malloc(0);
    char *file2_path = (char*)malloc(0);
    int validation;
    bzero(pwd, BUFSIZ);
    bzero(stdbuf, BUFSIZ);

    while(1)
    {
        file1_path = (char*)realloc(file1_path, 0);
        file2_path = (char*)realloc(file2_path, 0);

        // https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
        if (getcwd(pwd, BUFSIZ) == NULL)
        {
            perror("Error");
            exit(-1);
        }
        printf("%s$ ", pwd);
        if((result = fgets(stdbuf, BUFSIZ, stdin)) == NULL)
        {
            break;
        }
        result[strlen(result)-1] = '\0';
        // DIR
        if (strcmp(result, "DIR") == 0)
        {
            dir(pwd);
            bzero(pwd, BUFSIZ);
            bzero(stdbuf, BUFSIZ);
            continue;
        }
        // COPY
        if (strstr(result, "COPY") != NULL)
        {
            bool valid = true;
            validation = 3;
            while ((token = strtok_r(result, " ", &result)) != NULL)
            {
                if (validation == 3)
                {
                    validation--;
                    continue;
                }
                if (validation == 2)
                {
                    file1_path = (char*)realloc(file1_path, strlen(token)+1);
                    memcpy(file1_path,token,strlen(token));
                    validation--;
                    continue;
                }
                if (validation == 1)
                {
                    file2_path = (char*)realloc(file2_path, strlen(token)+1);
                    memcpy(file2_path,token,strlen(token));
                    validation--;
                    continue;
                }
                if ((token!=NULL) && (validation<=0))
                {
                    valid = false;
                    printf("Usage: COPY <src> <dst>\n");
                    break;
                }
            }
            if (validation > 0 || !valid)
            {
                printf("Usage: COPY <src> <dst>\n");
            }
            else
            {
                copy(file1_path, file2_path);
            }
            bzero(pwd, BUFSIZ);
            bzero(stdbuf, BUFSIZ);
            continue;
        }
        else
        {            
            printf("Unknown command\n");
            bzero(pwd, BUFSIZ);
            bzero(stdbuf, BUFSIZ);
        }
    }

    free(file1_path);
    free(file2_path);
    return 0;
}