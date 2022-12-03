#include "dir.h"

int dir(char* _dir)
{
    DIR *directory;
    struct dirent *temp;
    char buf[BUFSIZ] = {'\0'};
    if ((directory = opendir(_dir)) == NULL)
    {
        perror("Error");
        exit(-1);
    }
    
    while((temp = readdir(directory)) != NULL)
    {
        strcat(buf, strcat(temp->d_name, "\n"));
        if (buf[BUFSIZ-1] != '\0')
        {
            if (write(STDOUT_FILENO, buf, BUFSIZ) == -1)
            {
                perror("Error");
                exit(-1);
            }
            bzero(buf, BUFSIZ);
        }
    }
    if (closedir(directory) == -1)
    {
        perror("Error");
        exit(-1);
    }
    if (write(STDOUT_FILENO, buf, BUFSIZ) == -1)
    {
        perror("Error");
        exit(-1);
    }
    return 0;
}