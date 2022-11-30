#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: <input>|./mytee <file_path>\n");
        exit(-1);
    }

    char buf[BUFSIZ], *result;
    int fd = open(argv[1], O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd == -1)
    {
        perror("Error");
        exit(-1);
    }

    // Some code was taken from: https://www.ibm.com/docs/en/i/7.4?topic=functions-fgets-read-string
    while((result = fgets(buf, BUFSIZ, stdin)) != NULL)
    {
        if (write(1, buf, strlen(buf)) == -1)
        {
            perror("Error");
            exit(-1);
        }
        if (write(fd, buf, strlen(buf)) == -1)
        {
            perror("Error");
            exit(-1);
        }
        bzero(buf, BUFSIZ);
    }

    return 0;
}