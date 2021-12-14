#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

class SocketServer{
public:
    SocketServer(const char* ip, int port);
    bool _init(const char* ip, int port);
    bool listen_to_port(char* (*fun)(char*));

private:
    const static int MAXSIZE = 4096;
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[MAXSIZE];
};
