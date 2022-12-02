#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int execute(char* pwd, char** stdptr, int sockfd, int sockfd6, struct sockaddr_in* servaddr, struct sockaddr_in* cli, struct sockaddr_in6* servaddr6);
int cmd(char** res);