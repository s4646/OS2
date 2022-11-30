#include "execute.h"

int execute(char* result)
{
    int pid = fork();
    char *token;
    char * temp = (char*)malloc(strlen(result)+1);
    int counter = 0;
    int i = 0;
    
    // child process
    if (pid == 0)
    {
        memcpy(temp, result, strlen(result)+1);
        // count arguments
        while ((token = strtok_r(temp, " ", &temp)) != NULL)
        {
            counter++;
        }
        char *args[counter + 1];
        args[0] = malloc(5);
        memcpy(args[0], "/bin/", 5);

        args[counter] = NULL;
        
        // build args array
        while ((token = strtok_r(result, " ", &result)) != NULL)
        {
            if (i==0)
            {
                args[i] = (char*)realloc(args[i], strlen(token) + 5);
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
    }
    
    // parent process
    else
    {
        wait(NULL);
    }
    free(temp);
    return 0;
}
