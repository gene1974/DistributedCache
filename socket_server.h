#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

const int MAXSIZE = 4096;

class SocketServer{
public:
    SocketServer(const char* ip, int port);
    bool _init(const char* ip, int port);
    bool listen_to_port();
private:
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[MAXSIZE];
};
