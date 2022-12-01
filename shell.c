#include "execute.h"

int main()
{
    char pwd[BUFSIZ], stdbuf[BUFSIZ], *stdptr;
    bzero(pwd, BUFSIZ);
    bzero(stdbuf, BUFSIZ);
    int save_in = dup(STDIN_FILENO);
    int save_out = dup(STDOUT_FILENO);

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
        execute(pwd, &stdptr);
        bzero(pwd, BUFSIZ);
        bzero(stdbuf, BUFSIZ);
        dup2(save_in, STDIN_FILENO);
        dup2(save_out, STDOUT_FILENO);
    }
    return 0;
}