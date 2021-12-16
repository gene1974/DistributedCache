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
    ~SocketServer();
    bool _init(const char* ip, int port);
    char* listen_once();
    bool listen_to_port(char* (*function)(char*));

    char* listen_without_close();
    bool response_and_close(char* sendline);

private:
    char* _ip;
    int _port;

    const static int MAXSIZE = 4096;
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[MAXSIZE];
};
