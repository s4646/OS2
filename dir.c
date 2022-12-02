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
        write(STDOUT_FILENO, strcat(temp->d_name, "\n"), strlen(temp->d_name)+2);
    }
    if (closedir(directory) == -1)
    {
        perror("Error");
        return -1;
    }
    return 0;
}