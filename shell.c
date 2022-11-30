#include "execute.h"

int main()
{
    char pwd[BUFSIZ], stdbuf[BUFSIZ], *result = NULL;
    bzero(pwd, BUFSIZ);
    bzero(stdbuf, BUFSIZ);

    while(1)
    {
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
        
        execute(pwd, stdbuf, result);
        bzero(pwd, BUFSIZ);
        bzero(stdbuf, BUFSIZ);
    }
    return 0;
}