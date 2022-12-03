#include "copy.h"


int copy_file(char *file1_path, char *file2_path)
{
    int fd1 = open(file1_path, O_RDONLY);
    int fd2 = open(file2_path, O_WRONLY);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("Error");
        exit(-1);
    }
    if (ftruncate(fd2,0) == -1)
    {
        perror("Error");
        exit(-1); 
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
                exit(-1);
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
            exit(-1);
        }  
    }
    
    if (close(fd1) == -1 || close(fd2) == -1)
    {
        perror("Error");
        exit(-1);
    }
    return 0;
}

int copy(char *result)
{
    bool valid = true;
    int validation = 3;
    char *token;
    char *file1_path;
    char *file2_path;

    // input validation
    while ((token = strtok_r(result, " ", &result)) != NULL)
    {
        // COPY
        if (validation == 3)
        {
            validation--;
            continue;
        }
        // <src>
        if (validation == 2)
        {
            file1_path = (char*)calloc(strlen(token)+1, sizeof(char));
            memcpy(file1_path, token, strlen(token));
            validation--;
            continue;
        }
        // <dst>
        if (validation == 1)
        {
            file2_path = (char*)calloc(strlen(token)+1, sizeof(char));
            memcpy(file2_path, token, strlen(token));
            validation--;
            continue;
        }
        if ((token!=NULL) && (validation<=0))
        {
            valid = false;
            printf("Usage: COPY <src> <dst>\n");
            exit(-1);
        }
    }
    if (validation > 0 || !valid)
    {
        printf("Usage: COPY <src> <dst>\n");
        exit(-1);
    }
    else
    {
        int ret = copy_file(file1_path, file2_path);
        free(file1_path);
        free(file2_path);
        return ret;
    }
}