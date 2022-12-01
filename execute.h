#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute(char* pwd, char* stdbuf, char** result);
int cmd_no_pipe(char** res);