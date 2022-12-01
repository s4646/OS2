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

int execute(char* pwd, char* stdbuf, char** result)
{
    if (strstr(*result, "|") != NULL)
    {
        // https://stackoverflow.com/questions/4235519/counting-number-of-occurrences-of-a-char-in-a-string-in-c
        int pipes = 0;
        char *pch = strchr(*result, '|');
        while (pch != NULL)
        {
            pipes++;
            pch = strchr(pch + 1, '|');
        }

        // create commands array
        char *commands[pipes+1], *token;
        int i = 0;
        while ((token = strtok_r(*result, "|", result)) != NULL)
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
            if(!fork())
            {
                return cmd(&commands[i]);
            }
            else
            {
                return wait(NULL);
            }
        }
    }
    else // no pipe
    {
        // DIR
        if (strcmp(*result, "DIR") == 0)
        {
            return dir(pwd);
        }
        // COPY
        else if (memcmp(*result, "COPY", 4) == 0)
        {
            return copy(*result);
        }
        else
        {
            if(!fork())
            {
                return cmd(result);
            }
            else
            {
                wait(NULL);
                return 0;
            }
        }
    }
}
