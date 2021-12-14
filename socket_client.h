#include <iostream>
#include<cstring>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

class SocketClient{
public:
    bool connect_and_send(const char* ip, int port, const char* sendline);
    bool send_stdin(const char* ip, int port);
    char* send_to_port(const char* ip, int port, const char* sendline);

private:
    //bool _init(const char* ip, int port);
    const static int MAXSIZE = 4096;
    char sendline[MAXSIZE], recvline[MAXSIZE];
    int sockfd, n;
    struct sockaddr_in  servaddr;
};


