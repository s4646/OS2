#include "dir.h"

int dir(char* _dir)
{
    DIR *directory;
    struct dirent *temp;
    if ((directory = opendir(_dir)) == NULL)
    {
        perror("Error");
        return -1;
    }
    
    while((temp = readdir(directory)) != NULL)
    {
        printf("%s\n", temp->d_name);
    }
    if (closedir(directory) == -1)
    {
        perror("Error");
        return -1;
    }
    return 0;
}