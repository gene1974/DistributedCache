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
    bool connect_server(const char* ip, int port);
    bool send_line(const char* sendline);
    bool close_connect();
    bool connect_and_send(const char* ip, int port, const char* sendline);
    bool send_stdin(const char* ip, int port);

private:
    //bool _init(const char* ip, int port);
    bool _connect(const char* ip, int port);
    bool _send(const char* sendline);
    bool _close();

    int   sockfd, n;
    struct sockaddr_in  servaddr;
};


